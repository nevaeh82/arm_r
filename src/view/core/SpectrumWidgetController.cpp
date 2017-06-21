#include <QTime>
#include <QPainter>
#include <QGLPixelBuffer>
#include <QGLFormat>

#include "SpectrumWidgetController.h"
#include "SpectrumWidget.h"

#include "Rpc/RpcDefines.h"

#include "DBStation/StationHelper.h"

#include <qwt_plot_item.h>

#define SONOGRAMM_THINNING 1000
#define SONOGRAM_LINES 60

#define THINNING_TIMEOUT 5000

SpectrumWidgetController::SpectrumWidgetController(QObject *parent) : QObject(parent)
{
	m_sonogramLimit = SONOGRAM_LINES;
	m_timming = 0;
	m_timmingGlobal = 50;
	m_timmingCount = 0;
	m_timmingCurPos = 0;
	m_sonogramReady = true;
	m_sonogramTime = QTime::currentTime();

	m_isReady = true;

	m_globalThreshold = 0;

	m_lastSpectrumVal = 0;

	m_selectionUpFlag = false;

	m_alarm = false;
	m_initGraph = false;

	m_dbManager = NULL;
	m_rpcFlakonClient = NULL;

	m_prm300WidgetController = NULL;

	m_current_frequency = 20*TO_MHZ;
	m_autoSearch = false;

	m_specStatus = true;

	m_rett = -100;

	m_enableCorrelation = false;
	m_bandwidth = 0;
	m_pointCount = 0;
	m_isComplex = false;

	m_peakVisible = false;

	m_id = 0;
	m_tab = NULL;

	m_graphicsWidget = NULL;
	m_graphicsContextMenu = NULL;

	m_controlPanelController = NULL;
	nextClearState = false;

	m_spectrumShow = true;
	correlationFlag = false;

	m_controlPanelMode = -1;
	m_stopFlag = true;

	m_sigDialog = NULL;

	m_isScreenShot = false;
	m_isRecord = false;

	connect(this, SIGNAL(signalVisible(bool)), this, SLOT(onVisible(bool)));

	connect(this, SIGNAL(onCorrelationStateChangedSignal(bool)), this, SLOT(onCorrelationStateChangedSlot(bool)));

	connect(&m_sonogramWatcher, SIGNAL(finished()), this, SLOT(onSonogramDataReady()));

	delTimer = new QTimer(this);

	connect(this, SIGNAL(onSetZeroFreq(double)), this, SLOT(setZeroFrequencyInternal(double)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalDataArrived(QString,QVariant)), this, SLOT(onDataArrivedInternal(QString,QVariant)), Qt::QueuedConnection);
	connect(this, SIGNAL(onSetCurrentFrequency(double)), this, SLOT(setCurrentFrequencyInternal(double)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalClearSonogram()), this, SLOT(clearSonogramInternal()));

	m_qwtVector = new QVector<QwtPoint3D>;
	m_qwtCurve = new QwtPlotSpectroCurve;
	m_qwtCurve->setPenWidth(3);
	m_qwtCurve->setRenderThreadCount(4);
	m_qwtCurve->setItemInterest(QwtPlotItem::LegendInterest, false);
	m_qwtCurve->setItemInterest(QwtPlotItem::ScaleInterest, false);
	m_qwtCurve->setRenderHint(QwtPlotItem::RenderAntialiased, false);
	m_qwtColorMap = new ColorMap;

	readRecordSettings();
}

SpectrumWidgetController::~SpectrumWidgetController()
{
	m_sonogramWatcher.cancel();
	m_sonogramWatcher.waitForFinished();

	delete m_qwtVector;
	delete m_qwtCurve;
	delete m_qwtColorMap;
}

void SpectrumWidgetController::appendView(SpectrumWidget* view)
{
	m_view = view;
	m_sigDialog = new SignalDetectedDialog(m_view);
	connect(m_sigDialog, SIGNAL(onStopSpectrum()), this, SLOT(onSpectrumStop()));

	init();
}

void SpectrumWidgetController::setTab(ITabSpectrum *tab)
{
	m_tab = tab;
}

void SpectrumWidgetController::setId(const int id)
{
	m_id = id;
}

void SpectrumWidgetController::setPlatformId(const int id)
{
	m_platformId = id;
}

void SpectrumWidgetController::setChannelId(const int id)
{
	m_channelId = id;
}

void SpectrumWidgetController::setSpectrumName(const QString &name)
{
	m_name = name;
	m_view->setSpectrumName(name);

	if(m_prm300WidgetController) {
		m_prm300WidgetController->setName(m_name);
		m_prm300WidgetController->setId(m_id);
	}
}

void SpectrumWidgetController::setup()
{
	QString fileNameSpec = "2.pcm";

	QFile fl(fileNameSpec);
	if (!fl.open(QIODevice::ReadOnly)){
		return;
	}

	unsigned int PointCount = (fl.size()/sizeof(float));
	if (PointCount < 5) {
		QMessageBox::critical(m_view, tr("File error"), tr("Very small file or file not found"));
		return;
	}
	float* spectrum = new float[PointCount];
	fl.read((char*)spectrum,PointCount*sizeof(float));
	fl.close();

	m_graphicsWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

	QVector<double> vecy;
	QVector<double> vecx;
	for(uint i = 0; i < PointCount; i++)
	{
		vecx.append((double)i);
		vecy.append((double)spectrum[i]);
	}

	delete[] spectrum;
}

void SpectrumWidgetController::setControlPrmState(bool state)
{
	m_view->setControlPrmState(state);
}

Q_DECLARE_METATYPE(float*)
void SpectrumWidgetController::onDataArrived(const QString &method, const QVariant &arg)
{
	emit signalDataArrived(method, arg);
}

void SpectrumWidgetController::onDataArrivedInternal(const QString &method, const QVariant &arg)
{
	if (RPC_SLOT_SERVER_SEND_POINTS == method) {
		QList<QVariant> list = arg.toList();

		setIsReady(false);

		m_alarmMutex.lock();
		if(m_alarm) {
			m_alarm = false;
			m_alarmMutex.unlock();
			emit onDrawComplete();
			return;
		}
		m_alarm = false;
		m_alarmMutex.unlock();

		if(list.isEmpty()) {
			//It is no signal)
			slotSetStatus(false);
			emit onDrawComplete();
			return;
		}

		float* spectrum = list.at(0).value<float*>();
		float* spectrumPeakHold = (float*)list.at(1).value<float*>();
		int index = list.at(2).toInt();

		QTime cur = QTime::currentTime();
		if (list.count() == 3){
			m_initGraph = false;
			if(m_graphicsWidget->HasSelection()) {
				m_selectionUpFlag = true;
			} else {
				m_selectionUpFlag = false;
			}
			setSignal(spectrum, spectrumPeakHold, index);
		} else {
			int pointCount = list.at(3).toInt();
			double bandwidth = list.at(4).toDouble();
			bool isComplex = list.at(5).toBool();

			setSignalSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
		}

		QRegion reg = m_view->visibleRegion();
		if(reg.isEmpty() || reg.boundingRect().width() < 30
				|| reg.boundingRect().height() < 30) {
			emit onDrawComplete();
		}

		slotSetStatus(true);
		updateDBAreas();

		return;
	}

	if( method == RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH ) {
		setDetectedAreasUpdate(arg.toByteArray());
		updateDBAreas();
	}

}

void SpectrumWidgetController::onPlotReady()
{
	emit onDrawComplete();

	if(m_selectionUpFlag ) {
		updateSelection();
	}
}

void SpectrumWidgetController::clearSonogram()
{
	emit signalClearSonogram();
}

void SpectrumWidgetController::setCurrentWorkFrequency(const double cf)
{
	emit onSetCurrentFrequency(cf);
}

void SpectrumWidgetController::clearSonogramInternal()
{
	m_sonogramData.clear();
}

void SpectrumWidgetController::setRpcFlakonClient(RpcFlakonClientWrapper *rpcClient)
{
	m_rpcFlakonClient = rpcClient;
	m_prm300WidgetController->setRpcFlakonClient(m_rpcFlakonClient);
}

void SpectrumWidgetController::onCorrelationStateChanged(const bool isEnabled)
{
	emit onCorrelationStateChangedSignal(isEnabled);
}

void SpectrumWidgetController::onCorrelationStateChangedSlot(const bool isEnabled)
{
	correlationFlag = isEnabled;
}

void SpectrumWidgetController::onSonogramDataReady()
{

	m_sonogramMutex.lock();
	QPixmap out = m_sonogramPixmap;
	m_sonogramMutex.unlock();

	m_view->sonogramUpdate( out );

	m_sonogramReady = true;
}

void SpectrumWidgetController::updateDBAreas()
{
	m_graphicsWidget->ClearAllDetectedAreas();

	QList<StationsFrequencyAndBandwith> list;
	bool ret = m_dbStationController->getFrequencyAndBandwidthByCategory("Black", list, true);
	setDetectedAreas(2, list);

	ret = m_dbStationController->getFrequencyAndBandwidthByCategory("White", list, true);
	setDetectedAreas(1, list);

	setDetectedAreasUpdateOnPlot();
}

double SpectrumWidgetController::getCenterSelection()
{
	return m_centerFreqSelTemp;
}

Prm300ControlWidgetController *SpectrumWidgetController::getPrmController()
{
	return m_prm300WidgetController;
}

void SpectrumWidgetController::setLocationSetupWidgetController(LocationSetupWidgetController *controller)
{
	m_setupController = controller;

	connect(this, SIGNAL(onDrawComplete()), m_setupController, SLOT(slotPlotDrawComplete()));
	connect(this, SIGNAL(onDrawComplete()), this, SLOT(slotPlotDrawComplete()));

	connect(m_setupController, SIGNAL(signalSelectionUpdate()), this, SLOT(slotUpdateSelection()));
}

void SpectrumWidgetController::setAlarm(bool val)
{
	m_alarmMutex.lock();
	m_alarm = val;
	m_alarmMutex.unlock();
}

void SpectrumWidgetController::setReceiverStatus(bool state)
{
	m_prm300WidgetController->setChannelState(state);
}

void SpectrumWidgetController::setReceiverSettings(const RdsProtobuf::ReceiverSettings& settings)
{
	m_prm300WidgetController->setSettings(settings);
}

void SpectrumWidgetController::slotSetStatus(bool stat)
{
	m_specStatus = stat;
	m_view->setNoSignal(m_specStatus);

	//m_sonogramData.clear();
}

void SpectrumWidgetController::slotUpdateSelection()
{
	tmpSelection.start.setX( m_setupController->getCurrentLocation().range().start() );
	tmpSelection.end.setX( m_setupController->getCurrentLocation().range().end() );

	if( !m_graphicsWidget->isActivated() && !m_initGraph ) {
		return;
	}

	if(fabs(tmpSelection.start.x()-m_current_frequency/TO_MHZ) > 10 ||
			fabs(tmpSelection.end.x()-m_current_frequency/TO_MHZ) > 10) {
		return;
	}

	m_graphicsWidget->SetSelection(tmpSelection.start.x()*TO_MHZ, 0, tmpSelection.end.x()*TO_MHZ, 0);
}

void SpectrumWidgetController::updateSelection()
{
	slotUpdateSelection();
}

void SpectrumWidgetController::setAnalysisSelection(double start, double end)
{
	//m_graphicsWidget->ClearAllDetectedAreas();
	updateDBAreas();

	if( start == 0 && end == 0) {
		return;
	}

	m_graphicsWidget->SetDetectedAreas(5, start*TO_MHZ, 0, end*TO_MHZ, 0, false);
	//	/m_graphicsWidget->ZoomOutFull();
}

QString SpectrumWidgetController::getSpectrumName() const
{
	return m_view->getSpectrumName();
}

QWidget *SpectrumWidgetController::getWidget() const
{
	return m_view;
}

void SpectrumWidgetController::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	//	if(m_stopFlag == false)
	//	{
	//		emit onDrawComplete();
	//		return;
	//	}

	m_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;
	m_mux.unlock();

	m_sonogramData.clear();

	setFFTSetup(spectrum, spectrum_peak_hold);
}

void SpectrumWidgetController::setSonogramSetup(const QQueue<QList<double> >& sonogramData)
{
	QTime timeO = QTime::currentTime();
	int width = m_pointCount;

	//QGLPixelBuffer px(width, data.size()/10);
	QImage img(width, sonogramData.size(), QImage::Format_ARGB32);
	QPainter* painter;
	img.fill(Qt::transparent);

	/*	if(px.isValid()) {
		painter = new QPainter(&px);
	} else */if(!img.isNull()){
		painter = new QPainter(&img);
	}

	//bool isStart = painter->begin()
	bool tmp = painter->isActive();
	QPaintEngine::Type type = painter->paintEngine()->type();

	if(!tmp) {
		return;
	}

	QwtInterval interval(-100, 0);

	double time = 0;
	double ts = sonogramData.size();

	for(double i = 0; i < sonogramData.size(); i++) {
		for(double j = 0; j < sonogramData.at(i).size(); j++) {
			painter->setPen( m_qwtColorMap->color(interval, sonogramData.at(i).at(j)) );
			//painter->drawLine( QPointF((j), time*10), QPointF((j), (time+1)*10) );
			painter->drawPoint( j, time );
		}
		time+=1;
	}

	m_sonogramMutex.lock();
	m_sonogramPixmap = QPixmap::fromImage(img);
	m_sonogramPixmap.save("C:\\a\\aimg.png");
	m_sonogramMutex.unlock();

	log_debug(QString("Appending time %1").arg(timeO.msecsTo(QTime::currentTime())));
	timeO = QTime::currentTime();

}

void SpectrumWidgetController::readRecordSettings()
{
	QSettings aSettings("./Tabs/record.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	m_isScreenShot = aSettings.value("Common/screenshot", true).toBool();
	m_isRecord = aSettings.value("Common/record", false).toBool();
}

void SpectrumWidgetController::setFFTSetup(float* spectrum, float* spectrum_peak_hold)
{
	//	if(m_stopFlag == false)
	//	{
	//		return;
	//	}

	if(m_graphicsWidget->HasSelection()) {
		m_selectionUpFlag = true;
	} else {
		m_selectionUpFlag = false;
	}

	m_graphicsWidget->SetSpectrumVisible(2, m_peakVisible);

	float maxv = 0.0;
	float minv = 0.0;
	m_mux.lock();

	m_graphicsWidget->SetAutoscaleY(false);
	m_graphicsWidget->SetAlign(0);

	m_isComplex = false;

	m_graphicsWidget->Setup(m_isComplex, m_bandwidth, tr("Level"),
							spectrum, m_pointCount, spectrum_peak_hold, m_pointCount, false, false, minv, maxv);

	QVariant value = m_dbManager->getPropertyValue(getSpectrumName(), DB_FREQUENCY_PROPERTY);
	setZeroFrequency(value.toDouble());

	//m_graphicsWidget->ZoomOutFull();

	//	QVector<double> vecy;
	//	QVector<double> vecx;
	//	for(int i = 0; i < m_pointCount; i++)
	//	{
	//		vecx.append((double)i);
	//		vecy.append((double)spectrum[i]);
	//	}

	m_initGraph = true;
	m_mux.unlock();
}

void SpectrumWidgetController::setSignal(float *spectrum, float *spectrum_peak_hold, int index)
{
	if(!m_spectrumShow)
		return;

	m_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;

	m_graphicsWidget->SetAutoscaleY(false);
	m_graphicsWidget->SetAlign(0);
	m_mux.unlock();

	QList<double> m_sonogramLine;

	if( m_view->isSonogram() ) {
		for(int i = 0; i < m_pointCount; i++) {
			m_sonogramLine.append( spectrum[i] );
		}

		if(index != 0) {
			if( m_sonogramData.size() > 0 ) {
				m_sonogramData.takeLast();
			}
		}

		if(m_sonogramData.size() > m_sonogramLimit) {
			m_sonogramData.enqueue( m_sonogramLine );
			m_sonogramData.dequeue();
		} else {
			m_sonogramData.enqueue( m_sonogramLine );
		}

		if(!m_sonogramWatcher.isRunning()) {
			QFuture<void> f = QtConcurrent::run(this, &SpectrumWidgetController::setSonogramSetup, m_sonogramData);
			m_sonogramWatcher.setFuture( f );
		}
	} else {
		m_sonogramData.clear();
	}

	if(m_controlPanelMode == 3)
	{
		m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
		//m_graphicsWidget->ZoomOutFull();
		return;
	}

	QList<OverthresholdBand> listOverthreshold;

	if(m_view->isThreshold())
	{
		QList<StationsFrequencyAndBandwith> list;

		double m_threshold = 0;

		if(m_controlPanelMode == 2)
		{
			bool ret = m_dbStationController->getFrequencyAndBandwidthByCategory("White", list );
		}


		double start = 0;
		double finish = 0;
		bool state = true;
		int count = 0;
		for(int i = 0; i < m_pointCount; i++)
		{
			double overthreshold = ((m_current_frequency / TO_MHZ) -10) + ((m_bandwidth / TO_MHZ)/m_pointCount)*i;
			if(m_view->isGlobalThreshold()) {
				m_threshold = m_globalThreshold;
			} else {
				m_threshold = m_thresholdList.value(qRound(overthreshold), 0);
			}

			//listOverthreshold.append(overthreshold);

			if((spectrum[i] > m_threshold) && (m_rett != -99))
			{
				if(state == true)
				{
					start = overthreshold;
					state = false;
				}

				m_rett = 0;
				finish = overthreshold;
			}
			else {
				if(state == false)
				{
					OverthresholdBand over;
					over.startFreq = start;
					over.finishFreq = overthreshold;
					listOverthreshold.append(over);
					state = true;
				}
			}
		}
	}


	QList<StationsFrequencyAndBandwith> listBlack;
	bool retBlack = m_dbStationController->getFrequencyAndBandwidthByCategory("Black", listBlack);

	QList<OverthresholdBand> newList;
	QList<OverthresholdBand>::iterator i = listOverthreshold.begin();
	while (i != listOverthreshold.end())
	{
		OverthresholdBand cur = *i;
		bool j = false;
		foreach (StationsFrequencyAndBandwith st, listBlack)
		{
			double lower = st.frequency - st.bandwidth/2;
			double upper = st.frequency + st.bandwidth/2;
			if( cur.startFreq > lower && cur.finishFreq < upper)
			{
				j = true;
				i = listOverthreshold.erase(i);
				if(i == listOverthreshold.end())
				{
					break;
				} else {
					continue;
				}
			}
		}
		if(j != true)
			++i;
	}

	if(!listOverthreshold.isEmpty())
	{
		//setSpectrumShow(false);
		m_stopFlag = false;

		m_sigDialog->setNewFreq(m_name, listOverthreshold);
		m_sigDialog->show();

		int result = m_sigDialog->exec();

		listOverthreshold.clear();
		m_rett = -100;

		//		if(result == QDialog::Accepted){
		//			setSpectrumShow(true);
		//			m_stopFlag = true;

		//		}
		//		else {
		//			setSpectrumShow(false);
		//			m_stopFlag = false;
		//			return;
		//		}
	}

	//TODO: Write signals values over m_threshold. Task 6186
	//	if(!m_overthreshold.isEmpty()) {

	//		setSpectrumShow(false);
	//		m_stopFlag = false;

	//		m_sigDialog->setFrequency(m_name, m_overthreshold);

	//		int result = m_sigDialog->exec();

	//		m_overthreshold.clear();
	//		m_rett = -100;

	//		if(result == QDialog::Accepted){
	//			setSpectrumShow(true);
	//			m_stopFlag = true;

	//		}
	//		else {
	//			setSpectrumShow(false);
	//			m_stopFlag = false;
	//			return;
	//		}
	//	}

	m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);

	//m_sonogramWidget->replot();

	double startHz = 0;
	double endHz = 0;
	double y1;
	double y2;
	m_graphicsWidget->GetCurrentViewport(startHz, y1, endHz, y2);

	double dHz = fabs(startHz - endHz);

	//	//zoom Hack
	if(dHz >= 19900000) {
		m_graphicsWidget->ZoomOutFull();
	}

}

void SpectrumWidgetController::setDefModulation(QString modulation)
{
	m_graphicsWidget->SetLabel(m_centerFreqDefModulation*1000, modulation);
}

bool SpectrumWidgetController::isGraphicVisible()
{
	return m_view->isVisible();
}

quint32 SpectrumWidgetController::getId()
{
	return m_id;
}

quint32 SpectrumWidgetController::getPlatformId()
{
	return m_platformId;
}

quint32 SpectrumWidgetController::getChannelId()
{
	return m_channelId;
}

void SpectrumWidgetController::setLabelName(QString base, QString second)
{
	m_graphicsWidget->SetLabel(m_bandwidth/2, base);
}

void SpectrumWidgetController::setDetectedAreasUpdate(const QByteArray &vecBA)
{
	if(m_autoSearch == false){
		return;
	}

	QDataStream stream(vecBA);

	m_pointVector.clear();
	stream >> m_pointVector;

	//m_graphicsWidget->ClearAllDetectedAreas();
	//setDetectedAreasUpdateOnPlot();
}

void SpectrumWidgetController::setDetectedAreasUpdateOnPlot()
{
	QVector<QPointF>::iterator it;
	for(it = m_pointVector.begin(); it != m_pointVector.end(); ++it) {
		m_graphicsWidget->SetDetectedAreas(3, (*it).x()*TO_MHZ, 0, (*it).y()*TO_MHZ, 0, false);
	}
}

void SpectrumWidgetController::slotAddToList(double start, double end)
{
	double center = start + fabs(start-end);

	emit signalAddToList(m_name, center, fabs(start-end));
}

void SpectrumWidgetController::slotContinueAnalysis(bool)
{
	//  m_controlPanelController->onEnableMode(2, true);
}

void SpectrumWidgetController::setSpectrumShow(bool state)
{
	m_spectrumShow = state;
	//emit signalSpectrumEnable(state);

	m_view->slotSetEnableSpactrum(state);
	m_stopFlag = state;
}

void SpectrumWidgetController::setDetectedAreas(int mode, const QList<StationsFrequencyAndBandwith>& list)
{
	double zeroFreq = m_current_frequency - m_bandwidth/2;

	foreach (StationsFrequencyAndBandwith value, list) {
		double beginFreq = (value.frequency - value.bandwidth/2)*TO_MHZ;
		double endFreq = (value.frequency + value.bandwidth/2)*TO_MHZ;
		if(endFreq < zeroFreq)
			continue;
		if(beginFreq > zeroFreq + m_bandwidth)
			continue;

		if(beginFreq < zeroFreq)
			beginFreq = zeroFreq;
		if(endFreq > zeroFreq + m_bandwidth)
			endFreq = zeroFreq + m_bandwidth;

		m_graphicsWidget->SetDetectedAreas(mode, beginFreq, 0, endFreq, 0, false);
	}
}

void SpectrumWidgetController::setZeroFrequency(double val)
{
	emit onSetZeroFreq(val);
}

void SpectrumWidgetController::setZeroFrequencyInternal(double val)
{
	if(/*m_stopFlag == false ||*/ val < 0)
	{
		return;
	}

	double tmp = (m_graphicsWidget->ZeroFrequencyHz()/TO_MHZ + 10);

	if(val == (m_graphicsWidget->ZeroFrequencyHz()/TO_MHZ + 10)) {
		return;
	}

	m_dbManager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, val);

	double zeroFreq = val*TO_MHZ - /*m_bandwidth/2*/10000000;

	//If not activated spectrum - laggy counting of zero frequency ???
	if(m_graphicsWidget->isActivated()) {
		m_graphicsWidget->SetZeroFrequencyHz(zeroFreq);
	}

	updateDBAreas();

	//	QList<StationsFrequencyAndBandwith> list;
	//	bool ret = m_dbStationController->getFrequencyAndBandwidthByCategory("Black", list );
	//	setDetectedAreas(2, list);

	//	ret = m_dbStationController->getFrequencyAndBandwidthByCategory("White", list );
	//	setDetectedAreas(1, list);
}

void SpectrumWidgetController::setCurrentFrequencyInternal(double val)
{
	m_current_frequency = val*TO_MHZ;

	if(m_view->isThreshold() && m_view->isGlobalThreshold()) {
		int freq = m_current_frequency/TO_MHZ;
		for (int i=freq-10; i<=freq+10; i+=10) {
			m_graphicsWidget->SetLabel(m_globalThreshold, "redLine", QString::number(i));
		}
	}
}

void SpectrumWidgetController::setVisible(const bool isVisible)
{
	emit signalVisible(isVisible);
}

void SpectrumWidgetController::onVisible(const bool b) {
	m_view->setVisible(b);
}

void SpectrumWidgetController::setAutoSearch(bool enabled)
{
	m_view->setAutoSearch(enabled);

	if(!enabled) {
		m_pointVector.clear();
	}
}

void SpectrumWidgetController::setPanorama(bool enabled)
{
	m_view->setPanorama(enabled);
}

void SpectrumWidgetController::setSelection(double start, double end)
{
	double start_x, start_y, end_x, end_y;
	m_graphicsWidget->GetCurrentViewport(start_x, start_y, end_x, end_y);
	if( (start*TO_MHZ > start_x) && (end*TO_MHZ < end_x) ) {
		m_graphicsWidget->SetSelection(start*TO_MHZ, 0, end*TO_MHZ, 0);
	}
}

void SpectrumWidgetController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
	m_prm300WidgetController->setDbManager(m_dbManager);
}

void SpectrumWidgetController::setDbStationController(DBStationController *controller)
{
	m_dbStationController = controller;
}

void SpectrumWidgetController::setControlPanelController(ControlPanelController *controller)
{
	m_controlPanelController = controller;
	connect(m_controlPanelController, SIGNAL(signalReadyToScreenShot()), this, SLOT(slotOnScreenShot()));
}

void SpectrumWidgetController::init()
{
	m_graphicsWidget = m_view->getGraphicsWidget();

	//	QwtPlotSvgItem* itm;
	//	itm->attach(m_sonogramWidget);
	//itm->loadData()

	//myPlot->setCanvasBackground(new QBrush(*image));

	//m_qwtCurve->attach(m_sonogramWidget);

	m_graphicsWidget->SetZoomOutMaxOnDataSet(true);
	m_graphicsWidget->SetAutoscaleY(true);
	m_graphicsWidget->SetAlign(2);

	//m_graphicsWidget->ZoomOutFull();
	m_graphicsWidget->SetHorizontalLabel(tr("Hz"));

	m_graphicsContextMenu = new QMenu(m_graphicsWidget);
	m_graphicsContextMenu->addAction(new QAction(tr("Add to white list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Add to black list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Determine signal"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Record signal"),this) );
	//	m_graphicsContextMenu->addAction(new QAction(tr("Enable correlation"),this) );
	//	m_graphicsContextMenu->addAction(new QAction(tr("Disable correlation"),this) );
	//	m_graphicsContextMenu->addAction(new QAction(tr("Cleanup text fields"),this) );

	connect(m_graphicsContextMenu->actions().at(0),SIGNAL(triggered()),SLOT(addToWhiteList()));
	connect(m_graphicsContextMenu->actions().at(1),SIGNAL(triggered()),SLOT(addToBlackList()));
	connect(m_graphicsContextMenu->actions().at(2),SIGNAL(triggered()),SLOT(recognizeSignal()));
	connect(m_graphicsContextMenu->actions().at(3),SIGNAL(triggered()),SLOT(recordSignal()));

	//connect(m_graphicsContextMenu->actions().at(5),SIGNAL(triggered()),SLOT(clearLabels()));
	connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(slotIsShowContextMenu()));

	m_graphicsWidget->setContextMenu(m_graphicsContextMenu);

	connect(m_graphicsWidget, SIGNAL(SelectionCleared()), this, SLOT(slotSelectionCleared()));
	connect(m_graphicsWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(m_graphicsWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));
	connect(m_graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(slotDoubleClicked(double, double)));
	connect(this, SIGNAL(signalCurSelChanged(int)), m_graphicsWidget, SLOT(slotCurSelectionChanged(int)));
	connect(m_graphicsWidget, SIGNAL(plotReady()), this, SLOT(onPlotReady()));

	connect(m_view, SIGNAL(setPanoramaSignal(bool)), this, SLOT(slotSetEnablePanorama(bool)));
	connect(m_view, SIGNAL(setAutoSearchSignal(bool)), this, SLOT(slotAutoSearch(bool)));

	connect(m_view, SIGNAL(selectionTypeChangedSignal(bool)), this, SLOT(slotSelectiontypeChanged(bool)));
	connect(m_view, SIGNAL(onGlobalThresholdToggled(bool)), this, SLOT(slotGlobalThreshold(bool)));

	connect(m_view, SIGNAL(requestDataSignal(bool)), this, SLOT(slotRequestData(bool)));
	connect(this, SIGNAL(signalSpectrumEnable(bool)), m_view, SLOT(slotEnableKM(bool)));

	connect(m_view, SIGNAL(setShowPeaksSignal(bool)), this, SLOT(slotShowPeaks(bool)));
	connect(m_view, SIGNAL(setShowControlPRM(bool)), this, SLOT(slotShowControlPRM(bool)));

	connect(m_view, SIGNAL(signalManualThreshold(double)), this, SLOT(slotSelectionFinishedRedLine(double)));

	m_prm300WidgetController = new Prm300ControlWidgetController(m_view->getSpectrumName(), this);
	m_prm300WidgetController->appendView(m_view->getPrm300Widget());

	m_prm300WidgetController->setRpcFlakonClient(m_rpcFlakonClient);
}

void SpectrumWidgetController::slotSetEnablePanorama(bool state)
{
	m_tab->setPanorama(state);
}

void SpectrumWidgetController::slotAutoSearch(bool state)
{
	m_autoSearch = state;

	if (!m_autoSearch) {
		m_graphicsWidget->ClearAllDetectedAreas();
	}
}

void SpectrumWidgetController::slotSelectiontypeChanged(bool state)
{
	if(state) {
		if(m_view->isGlobalThreshold()) {
			int freq = m_current_frequency / TO_MHZ;
			for (int i=freq-10; i<=freq+10; i+=10) {
				m_graphicsWidget->SetLabel(m_globalThreshold, "redLine", QString::number(i));
			}
			m_view->setThresholdValue(m_globalThreshold);
		} else {
			QMap<int, int> thresholdList;
			int cf = 10*qRound(m_current_frequency / (TO_MHZ*10));
			int res = m_dbStationController->getThresholdByFrequencyAndStation(m_name, cf, thresholdList);

			if(res && !thresholdList.isEmpty()) {
				m_thresholdList.clear();
				m_thresholdList = thresholdList;
			}

			foreach (int freq, thresholdList.keys()) {
				m_graphicsWidget->SetLabel(thresholdList.value(freq, 0), "redLine", QString::number(freq));
				if(freq == cf) {
					m_view->setThresholdValue(m_thresholdList.value(freq, 0));
				}
			}

		}
	} else {
		m_graphicsWidget->SetLabel(0, "resetRedLine");
		m_thresholdList.clear();
	}

	if(state){
		emit signalCurSelChanged(4);
		return;
	}

	emit signalCurSelChanged(1);
}

void SpectrumWidgetController::slotGlobalThreshold(bool val)
{
	m_graphicsWidget->SetLabel(0, "resetRedLine");
	slotSelectiontypeChanged(true);
}

void SpectrumWidgetController::slotRequestData(bool state)
{
	if(m_spectrumShow == state)
		return;

	setSpectrumShow(state);


	//	int data[4] = {0, 1, 2, 3};
	//	if(state){
	//		emit signalRequestData(m_id, 0, &data[0], 4);
	//	}
}

/// add selection to white list
void SpectrumWidgetController::addToWhiteList()
{
	StationData data;
	data.stationName = m_view->getSpectrumName();
	data.port = 155;
	data.category = "White";
	data.signalType = "Unknown";
	data.frequency = (tmpSelection.end.x() + tmpSelection.start.x()) / 2;
	data.bandwidth= (tmpSelection.end.x() - tmpSelection.start.x());
	data.checked = 0;
	data.comment = "";


	StationData dataExist = m_dbStationController->
			getStationData( data.stationName, data.port, data.frequency, data.bandwidth );

	if( !dataExist.stationName.isNull() ) {
		StationHelper::showStationExistsWarning( dataExist, m_view );
		return;
	}

	int result = m_dbStationController->addStationData(data);

	if( result < 1 ) {
		StationHelper::showStationWarning( result, m_view );
		return;
	}

	emit signalAddSelToLists(1);

	m_tab->updateDbStationsLists();
}

/// add selection to black list
void SpectrumWidgetController::addToBlackList()
{
	StationData data;
	data.stationName = m_view->getSpectrumName();
	data.port = 155;
	data.category = "Black";
	data.signalType = "Unknown";
	data.frequency = (tmpSelection.end.x() + tmpSelection.start.x()) / 2;
	data.bandwidth= (tmpSelection.end.x() - tmpSelection.start.x());
	data.checked = 0;
	data.comment = "";

	StationData dataExist = m_dbStationController->
			getStationData( data.stationName, data.port, data.frequency, data.bandwidth );

	if( !dataExist.stationName.isNull() ) {
		StationHelper::showStationExistsWarning( dataExist, m_view );
		return;
	}

	int result = m_dbStationController->addStationData(data);

	if( result < 1 ) {
		StationHelper::showStationWarning( result, m_view );
		return;
	}

	emit signalAddSelToLists(2);
	m_tab->updateDbStationsLists();

}

/// signal for flakon to recognize signal
void SpectrumWidgetController::recognizeSignal()
{
	m_controlPanelController->stopLocationRequest();
	delTimer->singleShot(500, this, SLOT(slotRecognizeSignal()));
}

void SpectrumWidgetController::slotRecognizeSignal()
{
	m_setupController->requestAnalysis(m_id);
}

void SpectrumWidgetController::recordSignalById(int id)
{
	if(m_id == id) {
		recordSignal();
	}
}

void SpectrumWidgetController::recordSignal()
{
	m_controlPanelController->stopLocationRequest();
	delTimer->singleShot(500, this, SLOT(slotRecordSignal()));
}

void SpectrumWidgetController::slotRecordSignal()
{
	m_setupController->requestRecord(m_id);
}

void SpectrumWidgetController::onSpectrumStop()
{
	setSpectrumShow(false);
}

void SpectrumWidgetController::slotOnScreenShot()
{
	QList<ControlPanelController::solverResult> lst = m_controlPanelController->getSolverResultList();
	foreach (ControlPanelController::solverResult res, lst) {
		if((m_current_frequency/TO_MHZ - res.freq) < 3) {
			if( !m_screenShotMapTime.contains(res.freq) ) {
				m_screenShotMapTime.insert(res.freq, QTime::currentTime());
				m_recordMapTime.insert(res.freq, QTime::currentTime());

				if(m_isScreenShot) {
					m_view->screenshotSpectrum(res.freq);
				}

				if(m_isRecord) {
					recordSignal();
				}
			} else {
				QTime sTime = m_screenShotMapTime.value(res.freq);
				if( sTime.msecsTo(QTime::currentTime()) > 60000 ) {
					readRecordSettings();
					if(m_isScreenShot) {
						m_view->screenshotSpectrum(res.freq);
					}
					m_screenShotMapTime.insert(res.freq, QTime::currentTime());
				}

				QTime sTime1 = m_recordMapTime.value(res.freq);
				int tmp = sTime1.msecsTo(QTime::currentTime());
				if( sTime1.msecsTo(QTime::currentTime()) > 60000*30 ) {
					readRecordSettings();
					if(m_isRecord) {
						recordSignal();
					}
					m_recordMapTime.insert(res.freq, QTime::currentTime());
				}
			}
		}
	}
}

void SpectrumWidgetController::slotPlotDrawComplete()
{
	m_isReady = true;
}

void SpectrumWidgetController::slotSelectionCleared()
{
}

void SpectrumWidgetController::slotSelectionFinished(double x1, double y1, double x2, double y2)
{
	///TODO: fix

	double x1count = x1 / TO_MHZ;
	double x2count = x2 / TO_MHZ;

	/// To HZ
	x1 =  x1count;
	x2 = x2count;
	//	x1 = TO_MHZ2;
	//	x2 = TO_MHZ2;

	m_centerFreqSelTemp = (x1count + x2count)/2;
	m_bandwidhtTemp = qAbs(x2count-x1count);

	SpectrumSelection selection;
	selection.start = QPointF(x1, y1);
	selection.end = QPointF(x2, y2);

	//if no selection
	if(x1 == 0 && x2 ==0 && y1 == 0 && y2 == 0) {
		if(!nextClearState) {
			nextClearState = true;
		}
		else {
			//			double coordinateZero = (double)0;
			//			tmpSelection.start = QPointF(coordinateZero, coordinateZero);
			//			tmpSelection.end = QPointF(coordinateZero, coordinateZero);
		}
	}
	//if any selection exist
	else {
		tmpSelection = selection;
		nextClearState = false;
	}

	m_tab->setSelectedArea(selection);
}

void SpectrumWidgetController::slotSelectionFinishedRedLine(double y)
{
	if(m_view->isGlobalThreshold()) {
		m_globalThreshold = y;

		if(m_view->isGlobalThreshold()) {
			int freq = m_current_frequency/TO_MHZ;
			for (int i=freq-10; i<=freq+10; i+=10) {
				m_graphicsWidget->SetLabel(m_globalThreshold, "redLine", QString::number(i));
			}
			m_view->setThresholdValue(m_globalThreshold);
		}

	} else {

		int cf = qRound(m_current_frequency/(TO_MHZ));
		int width = m_view->getThresholdWidth();

		for(int i = 0; i<width; i+=2) {
			if(i == 0) {
				bool res = m_dbStationController->setThresholdByFrequencyAndStation(m_name, cf, y);
				m_thresholdList.insert(cf, y);
			} else {
				int cnt = i/2;
				int cf1 = cf-cnt;
				int cf2 = cf+cnt;

				m_dbStationController->setThresholdByFrequencyAndStation(m_name, cf1, y);
				m_thresholdList.insert(cf1, y);

				m_dbStationController->setThresholdByFrequencyAndStation(m_name, cf2, y);
				m_thresholdList.insert(cf2, y);
			}
		}

		m_graphicsWidget->SetLabel(0, "resetRedLine");
		foreach (int freq, m_thresholdList.keys()) {
			m_graphicsWidget->SetLabel(m_thresholdList.value(freq, 0), "redLine", QString::number(freq));
			if(freq == cf) {
				m_view->setThresholdValue(m_thresholdList.value(freq, 0));
			}
		}

	}
}

void SpectrumWidgetController::slotIsShowContextMenu()
{
	QList<QPointF> selectionList = m_graphicsWidget->GetAllHorSelections();
	if(selectionList.isEmpty()
			|| (selectionList.count() == 1 && selectionList.first().x() == 0
				&& selectionList.first().y() == 0))
		m_graphicsWidget->contextMenu()->setEnabled(false);
	else
		m_graphicsWidget->contextMenu()->setEnabled(true);
}

void SpectrumWidgetController::slotDoubleClicked(double a, double b)
{
	tmpSelection.start.setX( m_setupController->getCurrentLocation().range().start() );
	tmpSelection.end.setX( m_setupController->getCurrentLocation().range().end() );

	//Fix fail
	if( ( (m_graphicsWidget->ZeroFrequencyHz()/TO_MHZ) > tmpSelection.start.x() &&
		  (m_graphicsWidget->ZeroFrequencyHz()/TO_MHZ) > tmpSelection.end.x() ) ||
			( (m_graphicsWidget->ZeroFrequencyHz()/TO_MHZ) < tmpSelection.start.x() &&
			  (m_graphicsWidget->ZeroFrequencyHz()/TO_MHZ) < tmpSelection.end.x() )
			) {
		return;
	}

	//Create selection back when doubleClicked
	m_graphicsWidget->SetSelection(tmpSelection.start.x()*TO_MHZ, 0, tmpSelection.end.x()*TO_MHZ, 0);
	m_mux.lock();
	slotSelectionFinished(tmpSelection.start.x()*TO_MHZ, 0, tmpSelection.end.x()*TO_MHZ, 0);
	m_mux.unlock();
	nextClearState = false;

	emit doubleClickedSignal(m_id);
}

void SpectrumWidgetController::slotShowPeaks(bool visible)
{
	m_graphicsWidget->SetSpectrumVisible(2, visible);
	m_peakVisible = visible;
}

void SpectrumWidgetController::slotShowControlPRM(bool state)
{
	m_tab->setShowControlPrm(state);
	switch(state)
	{
	case true:
		m_view->getPrm300Widget()->show();
		break;

	default:
		m_view->getPrm300Widget()->hide();
		break;
	}
}
