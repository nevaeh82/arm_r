#include "CorrelationWidgetDataSource.h"

#include "Rpc/RpcDefines.h"

#include "Interfaces/ICorrelationWidget.h"

#include "math.h"

#define TIME_WAIT_CORRELATION 5000
#define SLEEP_MODE_TIMEOUT 5000

CorrelationWidgetDataSource::CorrelationWidgetDataSource(ITabManager* tabManager, int id1, int id2, QObject *parent)
	: BaseDataSource(parent)
	, m_needSetup(false)
	, m_corrGraphWgt(nullptr)
	, m_cf(0)
	, m_start(0)
	, m_end(0)
	, m_sleepModeProcess(true)
{
	m_id1 = id1;
	m_id2 = id2;

	m_tabManager = tabManager;

	m_mapPeaksCorrelation = new float[1];
	m_mapSpectrumCorelation = new float[1];
	m_mapBandwidthCorelation = 0;

//	correlationStateTimer = new QTimer(this);
//	correlationStateTimer->setInterval(TIME_WAIT_CORRELATION);
//	connect(correlationStateTimer, SIGNAL(timeout()), this, SLOT(correlationTimerOff()));

	m_sleepModeTimer = new QTimer(this);
	m_sleepModeTimer->setInterval(SLEEP_MODE_TIMEOUT);
	m_sleepModeTimer->setSingleShot(true);
	connect(m_sleepModeTimer, SIGNAL(timeout()), SLOT(onSleepModeSlot()));

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));

}

CorrelationWidgetDataSource::~CorrelationWidgetDataSource()
{
	//log_debug("<<<<<<<<<<<<");
}

void CorrelationWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	emit onMethodCalledSignal(method, data);
}

Q_DECLARE_METATYPE(float*)
void CorrelationWidgetDataSource::setCorData(quint32 point1, quint32 point2, const QVector<QPointF>& points, float veracity, double doppler)
{
    QString base = m_tabManager->getStationName(point1);//m_id);
    QString second = m_tabManager->getStationName(point2);

    emit dopplerStatus( QString("%1-%2").arg(base).arg(second), doppler );

	if(m_corrGraphWgt && !m_corrGraphWgt->isGraphicVisible()) {
		emit onDrawComplete();
		return;
	}


	float* spCorrelation = m_mapSpectrumCorelation;
	float* peaksCorrelation = m_mapPeaksCorrelation;
	double bCor = m_mapBandwidthCorelation;
	int pointCount = points.size();

	qreal startx = points.at(0).x();
	qreal endx = points.at(points.size() - 1).x();
	double bandwidth = endx - startx;

	if(bCor != bandwidth)
	{
		bCor = bandwidth;
		m_mapBandwidthCorelation = bCor;
		delete[] spCorrelation;
		spCorrelation = new float[pointCount];

		delete[] peaksCorrelation;
		peaksCorrelation = new float[pointCount];
		m_needSetup = true;
	}


	for(int i = 0; i < points.size(); i++) {
		spCorrelation[i] = points.at(i).y();

		if((m_startxCor != startx) || (spCorrelation[i] > peaksCorrelation[i]))
		{
			peaksCorrelation[i] = spCorrelation[i];
		}
	}

	m_mapSpectrumCorelation = spCorrelation;
	m_mapPeaksCorrelation = peaksCorrelation;

	if(m_startxCor != startx)
	{
		m_startxCor = startx;
	}

	QList<QVariant> list;
	QVariant spCorrelationVariant = QVariant::fromValue(spCorrelation);
	QVariant peaksCorrelationVariant = QVariant::fromValue(peaksCorrelation);
	list.append(spCorrelationVariant);
	list.append(peaksCorrelationVariant);

	if(m_needSetup)
	{
		QVariant pointCountVariant(pointCount);
		QVariant bCorVariant(bCor);
		QVariant isComplexVariant(true);

		list.append(pointCountVariant);
		list.append(bCorVariant);
		list.append(isComplexVariant);

		m_needSetup = false;
	}

	QVariant labelBase(base);
	QVariant labelSecond(second);

	list.append(labelBase);
	list.append(labelSecond);

	list.append(QVariant(veracity));
	list.append(QVariant(doppler));

	QVariant data(list);
	onDataReceived(RPC_SLOT_SERVER_SEND_CORRELATION, data);

	m_sleepModeProcess = false;
}

void CorrelationWidgetDataSource::correlationTimerOff()
{
	onCorrelationStateChanged(false);
}

void CorrelationWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
	if( RPC_METHOD_CONFIG_RDS_ANSWER ) {

		RdsProtobuf::ServerMessage sMsg;
		if( !parseServerMessage(data.toByteArray(), sMsg) ) {
			return;
		}

		if( isServerLocationShot(sMsg) ) {
			RdsProtobuf::ServerMessage_OneShotData_LocationData lMsg = getServerLocationShot( sMsg );

			RdsProtobuf::ServerMessage_OneShotData_LocationData_Plot correlationPlot;
			RdsProtobuf::Convolution convolution;

			int plotCount = lMsg.detector_status_size();

			if(!lMsg.convolution_size() && m_panelController) {
				m_panelController->onCorrelationStateChanged(false);
				return;
			}

			if( !findPlot(plotCount, lMsg.convolution_plot(), correlationPlot) ) {
				return;
			}

			if( !findConvolution(lMsg.convolution(), convolution) ) {
				return;
			}

			QVector<QPointF> points;

			if( (m_cf != lMsg.central_frequency()) ||
				(m_start != lMsg.range().start()) ||
				(m_end != lMsg.range().end()) ) {
				m_corrGraphWgt->clearDopler();
			}

			m_cf = lMsg.central_frequency();
			m_start = lMsg.range().start();
			m_end = lMsg.range().end();

			if(m_panelController) {
				m_panelController->setCorrelationFrequencyValue((m_start+m_end)/2);
			}

			float veracity = convolution.delay_accuracy();
			double doppler;

			if( convolution.has_doppler() ) {
				doppler = convolution.doppler();
			} else {
				doppler = std::numeric_limits<double>::quiet_NaN();
				log_debug("NO DOPPLER VALUE - IM SET NAN");
			}

			for( int k = 0; k<correlationPlot.data_size(); k++ ) {
				points.append( QPointF( correlationPlot.axis_x_start() + (correlationPlot.axis_x_step()*k),
										correlationPlot.data(k) ) );
			}

			setCorData( m_id1, m_id2, points, veracity, doppler );
			onCorrelationStateChanged(true);
		}
	}
}

void CorrelationWidgetDataSource::onSleepModeSlot()
{
    m_sleepModeProcess = true;
}

void CorrelationWidgetDataSource::sendCommand(int)
{
}

void CorrelationWidgetDataSource::onCorrelationStateChanged( bool isEnabled )
{
	foreach (ICorrelationListener* listner, m_correlationListeners) {
		listner->onCorrelationStateChanged(isEnabled);
	}

	if(m_panelController) {
		m_panelController->onCorrelationStateChanged(true);
	}
}

void CorrelationWidgetDataSource::registerCorrelationReceiver(ICorrelationListener* obj)
{
	m_correlationListeners.append(obj);
}

void CorrelationWidgetDataSource::deregisterCorrelationReceiver(ICorrelationListener* obj)
{
	m_correlationListeners.removeAll(obj);
}

void CorrelationWidgetDataSource::setCorrGraphWidget(ICorrelationWidget *wgt)
{
	m_corrGraphWgt = wgt;
}

bool CorrelationWidgetDataSource::findPlot(const int channelCount,
										   const ::google::protobuf::RepeatedPtrField< ::RdsProtobuf::ServerMessage_OneShotData_LocationData_Plot >& plots,
										   RdsProtobuf::ServerMessage_OneShotData_LocationData_Plot& outPlot)
{
	for(int i = 0; i < plots.size(); i++) {
		int id1=0;
		int id2=0;

		RdsProtobuf::ServerMessage_OneShotData_LocationData_Plot plot = plots.Get(i);

		getIds(plot.index(), channelCount, id1, id2);

		if( id2 != m_id2 || id1 != m_id1 ) {
			continue;
		} else {
			outPlot = plot;
			return true;
		}
	}

	return false;
}

bool CorrelationWidgetDataSource::findConvolution(const ::google::protobuf::RepeatedPtrField< ::RdsProtobuf::Convolution >& convList,
												  RdsProtobuf::Convolution& convolution)
{
	for(int i = 0; i < convList.size(); i++)  {

		int tmp1 = convList.Get(i).first_detector_index();
		int tmp2 = convList.Get(i).second_detector_index();

		if( m_id1 != convList.Get(i).first_detector_index() ||
			m_id2 != convList.Get(i).second_detector_index() ) {
			continue;
		} else {
			convolution = convList.Get(i);
			return true;
		}
	}

	return false;
}

void CorrelationWidgetDataSource::getIds(const int pos, const int chCnt, int& id1, int& id2)
{
	id1 = 0;
	id2 = 0;
	int listPos = 0;
	int inc = 1;
	int position = pos+1;

	QList<int> ids;
	for(int k = 0; k < chCnt; k++) {
		ids.append( k );
	}

	for(int i = 0; i < position; i++) {
		if((listPos+1) > (chCnt-1)) {
			listPos=inc;
			inc+=1;
		}

		id1 = ids.at( inc-1 );
		id2 = ids.at( listPos+1 );

		listPos++;
	}

	return;
}

