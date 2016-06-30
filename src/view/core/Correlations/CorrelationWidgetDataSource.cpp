#include "CorrelationWidgetDataSource.h"

#include "Rpc/RpcDefines.h"

#include "Interfaces/ICorrelationWidget.h"

#define TIME_WAIT_CORRELATION 5000

CorrelationWidgetDataSource::CorrelationWidgetDataSource(IGraphicWidget* correlationWidget, ITabManager* tabManager, int id1, int id2, QObject *parent)
	: BaseDataSource(parent)
	, m_needSetup(false)
{
	m_correlationWidget = correlationWidget;

	m_id1 = id1;
	m_id2 = id2;

	m_tabManager = tabManager;

	m_mapPeaksCorrelation = new float[1];
	m_mapSpectrumCorelation = new float[1];
	m_mapBandwidthCorelation = 0;

	correlationStateTimer = new QTimer(this);
	correlationStateTimer->setInterval(TIME_WAIT_CORRELATION);
	connect(correlationStateTimer, SIGNAL(timeout()), this, SLOT(correlationTimerOff()));

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));

}

void CorrelationWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	emit onMethodCalledSignal(method, data);
}

Q_DECLARE_METATYPE(float*)
void CorrelationWidgetDataSource::setCorData(quint32 point1, quint32 point2, const QVector<QPointF>& points, bool /*isComplex*/)
{
	//log_debug( QString("Points size: %1").arg(points.size() ) );

	QString base = m_tabManager->getStationName(point1);//m_id);
	QString second = m_tabManager->getStationName(point2);

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

	QVariant data(list);
	onDataReceived(RPC_SLOT_SERVER_SEND_CORRELATION, data);
}

void CorrelationWidgetDataSource::correlationTimerOff()
{
	onCorrelationStateChanged(false);
}

void CorrelationWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
	/// TODO this is hack for visible and unvisible widget. Need to refactor architechture
	if (RPC_SLOT_SERVER_SEND_CORRELATION == method) {

		QDataStream stream( data.toByteArray() );

		QVector<QPointF> points;
		quint32 point1, point2;

		stream >> point1 >> point2;

//		if( point1 == m_id ) {
//			m_correlationWidget->setVisible(false);
//			return;
//		}

		if( point2 != m_id2 || point1 != m_id1 ){
			return;
		}

		float trds;
		float veracity;

		stream >> trds;
		stream >> veracity;

		stream >> points;

		m_correlationWidget->setVisible(true);

		setCorData( point1, point2, points, true );

		onCorrelationStateChanged(true);
	}
}

void CorrelationWidgetDataSource::sendCommand(int)
{
}

void CorrelationWidgetDataSource::onCorrelationStateChanged( bool isEnabled )
{
	foreach (ICorrelationListener* listner, m_correlationListeners) {
		listner->onCorrelationStateChanged(isEnabled);
	}

	correlationStateTimer->start();
}

void CorrelationWidgetDataSource::registerCorrelationReceiver(ICorrelationListener* obj)
{
	m_correlationListeners.append(obj);
}

void CorrelationWidgetDataSource::deregisterCorrelationReceiver(ICorrelationListener* obj)
{
	m_correlationListeners.removeAll(obj);
}

