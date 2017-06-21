#include "ControlPanelController.h"
#include <math.h>

#include "UiDefines.h"

#include <Logger/Logger.h>

#include "SolverPacket1.pb.h"

#include "Rpc/RpcDefines.h"

#include "smtp1/smtpclient.h"
#include "smtp1/mimetext.h"

#define TIMER_INTERVAL 5000
#define TIMERCHECK_INTERVAL 5000
#define TIMERCHECK_INTERVAL_DETECTED 2000

#define TIMERINTERVAL_KEY "ControlPanel"

ControlPanelController::ControlPanelController(int tabId, QString title, QObject *parent)
	: QObject(parent),
	  m_tabId(tabId),
	  m_title(title)
{
	m_view = NULL;
	m_dbManager = NULL;
	m_currentFreq = 20;
	m_currentFreqFromPlot = 20;

	m_workCheckList = false;
	m_isDopplerSearch = false;

	m_startFreq = 0;
	m_finishFreq = 0;
	m_dbStation = NULL;
	m_rpcFlakonClient = NULL;
	m_mainStation = NULL;
	m_listsDialog = NULL;
	m_itDetected = -1;

	init();

	//connect(coordinateCounterThread, SIGNAL(started()), m_coordinatesCounter, SLOT(initSolver()));
	//connect(m_coordinatesCounter, SIGNAL(signalFinished()), coordinateCounterThread, SLOT(quit()));
	//connect(this, SIGNAL(threadTerminateSignal()), coordinateCounterThread, SLOT(quit()));
	//connect(this, SIGNAL(threadTerminateSignal()), m_coordinatesCounter, SLOT(deleteLater()));

	connect(this, SIGNAL(signalSetCentralFreq(double)), this, SLOT(setCentralFreqValueInternal(double)));

	m_solverResultListTimer = new QTimer(this);
	m_solverResultListTimer->setInterval(1000);
	connect(m_solverResultListTimer, SIGNAL(timeout()), this, SLOT(slotClearSolverResult()));
	m_solverResultListTimer->start();
}

ControlPanelController::~ControlPanelController()
{
}

void ControlPanelController::init()
{
	QSettings timerSettings("./ARM_R.ini", QSettings::IniFormat, this);
	timerSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	QStringList childKeys = timerSettings.childGroups();

	foreach (QString key, childKeys) {
		if(key == TIMERINTERVAL_KEY) {
			timerSettings.beginGroup(/*childKeys.first()*/key);
			m_timerInterval = timerSettings.value("timerInterval", TIMER_INTERVAL).toInt();
			m_timerCheckInterval = timerSettings.value("timerCheckInterval", TIMERCHECK_INTERVAL).toInt();
			m_timerCheckIntervalDopler = timerSettings.value("timerCheckIntervalDopler", TIMERCHECK_INTERVAL).toInt();
			m_timerCheckIntervalDetected = timerSettings.value("timerCheckIntervalDetected", TIMERCHECK_INTERVAL_DETECTED).toInt();
			timerSettings.endGroup();
			break;
		}
	}
}


void ControlPanelController::setLocationSetupController(LocationSetupWidgetController *controller)
{
	m_setupController = controller;

	connect(m_view, SIGNAL(signalReceiveSpectrums(bool)), m_setupController, SLOT(slotSetReceiveSpectrums(bool)));
	connect(this, SIGNAL(signalReceiveSpectrums(bool)), m_setupController, SLOT(slotSetReceiveSpectrums(bool)));
	connect(m_view, SIGNAL(signalChangeMode(int)), m_setupController, SLOT(slotChangeMode(int)));
	connect(m_view, SIGNAL(signalPanorama(bool)), m_setupController, SLOT(slotPanorama(bool)));
	connect(m_view, SIGNAL(signalSetStartFreq(int)), m_setupController, SLOT(slotSetStartFreq(int)));
	connect(m_view, SIGNAL(signalSetEndFreq(int)), m_setupController, SLOT(slotSetEndFreq(int)));
	connect(m_view, SIGNAL(signalSystemMerge(bool)), m_setupController, SLOT(slotSystemMerge(bool)));
	connect(m_view, SIGNAL(signalConvolution(bool)), m_setupController, SLOT(slotSetConvolution(bool)));
	connect(m_view, SIGNAL(signalDoppler(bool)), m_setupController, SLOT(slotSetDoppler(bool)));
	connect(m_view, SIGNAL(signalHumps(bool)), m_setupController, SLOT(slotSetHumps(bool)));
	connect(this, SIGNAL(signalSetComonFreq(double)), m_setupController, SLOT(slotOnSetCommonFreq(double)));


	connect(m_setupController, SIGNAL(signalSettingsChanged()), this, SLOT(setLocationSettings()));

	setLocationSettings();
}

void ControlPanelController::appendView(ControlPanelWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(autoSearchCheckedSignal(bool)), this, SLOT(onAutoSearchStateChangedSlot(bool)));

	connect(m_view, SIGNAL(onPanoramaEnable(bool,int,int)), this, SLOT(onPanoramaStateChangedSlot(bool, int, int)));
	connect(m_view, SIGNAL(onPanoramaEnable(bool,int,int)), this, SIGNAL(onStateChanged()));

	connect(m_view, SIGNAL(commonFreqChangedSignal(double)), this, SLOT(onCommonFrequencyChangedSlot(double)));
	connect(m_view, SIGNAL(signalManualMode()), this, SLOT(slotManualMode()));
	connect(m_view, SIGNAL(signalScanMode(int,int)), this, SLOT(slotScanMode(int,int)));
	connect(m_view, SIGNAL(signalCheckMode()), this, SLOT(slotCheckMode()));
	connect(m_view, SIGNAL(signalViewMode()), this, SLOT(slotViewMode()));
	connect(m_view, SIGNAL(signalViewAreaMode()), this, SLOT(slotViewAreaMode()));
	connect(m_view, SIGNAL(signalViewAreaDopplerMode()), this, SLOT(slotViewAreaDopplerMode()));

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotChangeFreq()));
	connect(&m_timerCheck, SIGNAL(timeout()), this, SLOT(slotCheckModeSetFreq()));

	connect(m_view, SIGNAL(signalDown1Mhz()), this, SLOT(slotDown1MHz()));
	connect(m_view, SIGNAL(signalDown10Mhz()), this, SLOT(slotDown10MHz()));
	connect(m_view, SIGNAL(signalDown100Mhz()), this, SLOT(slotDown100MHz()));
	connect(m_view, SIGNAL(signalUp1Mhz()), this, SLOT(slotUp1MHz()));
	connect(m_view, SIGNAL(signalUp10Mhz()), this, SLOT(slotUp10MHz()));
	connect(m_view, SIGNAL(signalUp100Mhz()), this, SLOT(slotUp100MHz()));

	connect(m_view, SIGNAL(signalSystemMerge(bool)), this, SIGNAL(signalSystemMerge(bool)));

	connect(this, SIGNAL(signalSetComonFreq(double)), m_view, SLOT(slotChangeCommonFreq(double)));

	connect(this, SIGNAL(setCorrelationStatus(QString)), this, SLOT(changeCorrelationStatus(QString)));
	connect(this, SIGNAL(setCorrelationStatusActive(bool)), this, SLOT(changeCorrelationStatusActive(bool)));

	connect(m_view, SIGNAL(onSleepMode(bool)), this, SLOT(onSetSleepMode(bool)));
}

void ControlPanelController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void ControlPanelController::setDbStationController(IDBStation *dbStationController)
{
	m_dbStation = dbStationController;
}

void ControlPanelController::setRpcFlakonClient(RpcFlakonClientWrapper *rpcFlakonClient)
{
	m_rpcFlakonClient = rpcFlakonClient;
	m_rpcFlakonClient->registerReceiver(this);
}

void ControlPanelController::setMapStations(QMap<int, Station *> stationsMap)
{
	m_stationsMap = stationsMap;
}

void ControlPanelController::onCorrelationStateChanged(const bool isEnabled)
{
	emit setCorrelationStatusActive(isEnabled);
}

void ControlPanelController::setCorrelationFrequencyValue(double value)
{
	emit setCorrelationStatus(QString::number(value) + QString(tr(" MHz")));
}

void ControlPanelController::setCentralFreqValue(double freq)
{
	emit signalSetCentralFreq(freq);
}

void ControlPanelController::setCentralFreqValueInternal(double freq)
{
	m_view->setCentralFreqValue(freq);

	if(m_currentFreq != freq) {
		m_solverResultList.clear();
		m_view->changeQualityStatus( 0 );
	}

	m_currentFreq = freq;
	m_currentFreqFromPlot = freq;
}

void ControlPanelController::slotSolverResult(QByteArray data)
{
	int quality = 0;
	bool isMoving = false;
	float movingFreq = 0;

	if(data.isEmpty()) {
		m_view->changeQualityStatus( 0 );
		m_solverResultList.clear();
	} else {

		SolverProtocol::Packet pkt;
		pkt.ParseFromArray(data.data(), data.size());

		m_solverResultList.clear();
		m_solverResultListTimer->start();

		SolverProtocol::Packet_DataFromSolver_SolverSolution solPkt;

		if(pkt.datafromsolver().has_solution_manual_altitude()) {
			solPkt = pkt.datafromsolver().solution_manual_altitude();

			for( int i = 0; i < solPkt.trajectory_size(); i++ ) {
				solverResult res;

				res.dateTime = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).datetime();
				res.freq = solPkt.trajectory(i).central_frequency();
				res.state = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).state();
				if(res.state == 1) {
					isMoving = true;
					movingFreq = res.freq;
				}

				res.quality = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).quality();
				quality |= res.quality;

				res.lat = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lat();
				res.lon = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lon();

				m_solverResultList.append( res );
			}
		}

		if(pkt.datafromsolver().has_solution_automatic_altitude()) {
			solPkt = pkt.datafromsolver().solution_automatic_altitude();

			for( int i = 0; i < solPkt.trajectory_size(); i++ ) {
				solverResult res;

				res.dateTime = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).datetime();
				res.freq = solPkt.trajectory(i).central_frequency();
				res.state = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).state();
				if(res.state == 1) {
					isMoving = true;
					movingFreq = res.freq;
				}

				res.quality = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).quality();
				quality |= res.quality;

				res.lat = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lat();
				res.lon = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).coordinates().lon();

				m_solverResultList.append( res );
			}
		}

		if(pkt.datafromsolver().has_solverresponse()) {
			if( pkt.datafromsolver().solverresponse().has_maxallowablerangessdv() ) {
				float skoMax = pkt.datafromsolver().solverresponse().maxallowablerangessdv();
				emit skoChanged(skoMax);
			}
		}

		m_view->changeQualityStatus( quality, isMoving, movingFreq );

		//		SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate mEst;
		//		if( mEst.ParseFromArray(data, data.size()) ) {
		//			int test = mEst.quality();
		//			m_view->changeQualityStatus( mEst.quality() );

		//			int freq = mEst.targetspeed_acc();

		//			//if(freq == m_currentFreq) {
		//				m_solverResult.dateTime = QDateTime::currentMSecsSinceEpoch();
		//				m_solverResult.quality = (int)mEst.quality();
		//				m_solverResult.state = (int)mEst.state();
		//				m_solverResult.freq = freq;
		//			//}
		//		}
	}

	emit signalReadyToScreenShot();
}

void ControlPanelController::onShowPanoramaControl(bool isOn)
{
	m_view->showPanoramaControl(isOn);
}

void ControlPanelController::slotClearSolverResult()
{
	//m_solverResultList.clear();
	m_view->changeQualityStatus( 0 );
}

bool ControlPanelController::checkSolverResult(int freq)
{
	foreach (solverResult res, m_solverResultList) {
		if(abs(res.freq - freq) < 2) {
			return true;
		}
	}

	return false;
}

int ControlPanelController::findResultInList(ControlPanelController::solverResult res)
{
	int index = 0;
	foreach (StationsFrequencyAndBandwith freq, m_listOfFreqs) {
		if( fabs(freq.frequency - res.freq) < 2 ) {
			return index;
		}

		index++;
	}
	return -1;
}

void ControlPanelController::setResponseFreq(quint32 freq)
{
	m_currentFreq = freq;
	m_view->slotChangeCommonFreq(freq);
}
void ControlPanelController::onPanoramaStateChangedSlot(bool isEnabled, int start, int end)
{
	foreach (IControlPanelListener* receiver, m_receiversList) {
		receiver->onGlobalPanoramaEnabled(isEnabled, start, end);
	}
}

void ControlPanelController::onAutoSearchStateChangedSlot(bool isEnabled)
{
	foreach (IControlPanelListener* receiver, m_receiversList) {
		receiver->onGlobalAutoSearchEnabled(isEnabled);
	}
}

void ControlPanelController::changeFrequency(double value)
{
	m_currentFreq = value;
	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::updatePanorama(const bool enable, const double &start, const double &end)
{
	m_view->updatePanorama(enable, start, end);
	onPanoramaStateChangedSlot( enable, start, end );
}

void ControlPanelController::onCommonFrequencyChangedSlot(double value)
{
	m_currentFreq = value;

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::onBandWidthChangedSlot(int start, int end)
{
	m_dbManager->updatePropertyForAllObjects(DB_PANORAMA_START_PROPERTY, start);
	m_dbManager->updatePropertyForAllObjects(DB_PANORAMA_END_PROPERTY, end);
}

void ControlPanelController::setManualMode()
{
	m_view->applyManualMode();
}

void ControlPanelController::slotManualMode()
{
	if(m_timerCheck.isActive()) {
		m_timerCheck.stop();
	}
	if(m_timer.isActive()) {
		m_timer.stop();
	}

	m_solverResultList.clear();
	m_listsDialog->clearDetectFreq(m_tabId);
	m_listsDialog->clearWorkList(m_tabId);
}

void ControlPanelController::slotScanMode(int start, int finish)
{
	if(m_timerCheck.isActive())
		m_timerCheck.stop();

	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("Black", m_listOfFreqsBlack);
	m_listsDialog->clearDetectFreq(m_tabId);

	m_startFreq = start;
	m_currentFreq = start;
	m_finishFreq = finish;
	m_timer.start(m_timerInterval);
}

void ControlPanelController::slotCheckMode()
{	
	if(m_timer.isActive()) {
		m_timer.stop();
	}

	m_isFollowMode = false;
	m_isDopplerSearch = false;
	m_listOfFreqs.clear();
	m_IdDetetcted.clear();

	m_listsDialog->clearDetectFreq(m_tabId);
	m_listsDialog->clearWorkList(m_tabId);

	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("White", m_listOfFreqs);
	log_debug(QString("data for check = %1").arg(m_listOfFreqs.count()));
	m_itCheckMode = m_listOfFreqs.begin();
	m_itDetected = -1;

	m_listsDialog->setWorkList(m_listOfFreqs, m_tabId, m_title);

	slotCheckModeSetFreq();
	m_timerCheck.start(m_timerCheckInterval);
}

void ControlPanelController::slotViewMode()
{
	slotCheckMode();
	m_isFollowMode = true;
	m_isDopplerSearch = false;
}

void ControlPanelController::slotViewAreaDopplerMode()
{
	m_view->enableDopler();

	slotViewAreaMode();
	m_isDopplerSearch = true;
	m_timerCheck.start(m_timerCheckIntervalDopler);
}

void ControlPanelController::slotViewAreaMode()
{
	if(m_timer.isActive()) {
		m_timer.stop();
	}

	if(m_timerCheck.isActive()) {
		m_timerCheck.stop();
	}

	m_isFollowMode = false;
	m_isDopplerSearch = false;

	m_listOfFreqs.clear();
	m_IdDetetcted.clear();

	m_listsDialog->clearDetectFreq(m_tabId);
	m_listsDialog->clearWorkList(m_tabId);

	m_listsDialog->getFrequencyAndBandwidthByWhiteAreas(m_listOfFreqs);
	m_dbStation->getFrequencyAndBandwidthByCategory("Black", m_listOfFreqsBlack);

	QList<StationsFrequencyAndBandwith> m_tmpList;

	bool inwork = false;

	foreach (StationsFrequencyAndBandwith dataBlack, m_listOfFreqsBlack) {

		if(inwork) {
			m_listOfFreqs.swap(m_tmpList);
			m_tmpList.clear();
		}

		foreach (StationsFrequencyAndBandwith data, m_listOfFreqs) {

			inwork = true;

			QRectF rBlack( dataBlack.frequency - (dataBlack.bandwidth/2),
						   0, dataBlack.bandwidth, 1 );

			QRectF rDat( data.frequency - (data.bandwidth/2),
						 0, data.bandwidth, 1 );

			QPainterPath p1;
			p1.addRect(rBlack);

			QPainterPath p2;
			p2.addRect(rDat);

			if(rDat.contains(rBlack)) {
				QRectF resultLeft(QPointF(rDat.x(), 0), QPointF(rBlack.x(), 1));
				QRectF resultRight(QPointF(rBlack.x()+rBlack.width(), 0),
								   QPointF(rDat.x()+rDat.width(), 1));
				StationsFrequencyAndBandwith tmpData1;
				StationsFrequencyAndBandwith tmpData2;

				tmpData1.frequency = resultLeft.center().x();
				tmpData1.bandwidth = resultLeft.width();
				tmpData1.isChecked = true;
				tmpData1.stationName = "";

				tmpData2.frequency = resultRight.center().x();
				tmpData2.bandwidth = resultRight.width();
				tmpData2.isChecked = true;
				tmpData2.stationName = "";

				m_tmpList.append(tmpData1);
				m_tmpList.append(tmpData2);
				continue;
			}


			QPainterPath rsz = p2-p1;
			QRectF result = rsz.boundingRect();
			if(!result.isNull()) {
				StationsFrequencyAndBandwith tmpData1;

				tmpData1.frequency = result.center().x();
				tmpData1.bandwidth = result.width();
				tmpData1.isChecked = true;
				tmpData1.stationName = "";

				m_tmpList.append(tmpData1);
			}

		}
	}

	if(!m_tmpList.isEmpty()) {
		m_listOfFreqs.swap(m_tmpList);
	}
	m_tmpList.clear();

	m_itCheckMode = m_listOfFreqs.begin();
	m_itDetected = -1;

	m_listsDialog->setWorkList(m_listOfFreqs, m_tabId, m_title);

	slotCheckModeSetFreq();
	m_timerCheck.start(m_timerCheckInterval);


	m_isFollowMode = true;
}

void ControlPanelController::slotChangeFreq()
{
	if(m_currentFreq >= m_finishFreq)
	{
		m_currentFreq = m_startFreq;
	} else if(m_currentFreq < m_startFreq) {
		m_currentFreq = m_startFreq;
	} else {
		m_currentFreq += INTERVAL;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	changeFrequency(m_currentFreq);
}

void ControlPanelController::checkDopplerResult()
{
	//log_debug("Check >>> ");
	if(!m_isFollowMode) {
		return;
	}

	double listFreq = (int)(*m_itCheckMode).frequency;
	int currentFreq = m_setupController->currentFrequency();

	foreach (QString key, m_dopplerMap.uniqueKeys()) {
		QList<double> vals = m_dopplerMap.values(key);
		int isMoving = true;
		double averageDopler = 0;
		int falseCounter = 0;

		for(int i = 0; i<vals.size()-1; i+=2) {
			double val1 = vals.at(i);
			double val2 = vals.at(i+1);
			averageDopler += (val1+val2)/2;
			averageDopler = averageDopler/2;

			if( val1<10 && val2<10 ) {
				falseCounter++;
			}
		}

		//log_debug(QString("Doppler stat!  False cnt %1   all cnt %2").arg(falseCounter).arg(vals.size()));

		double persentage = (double)falseCounter / (double)vals.size();

		if(persentage < 0.2) {
			isMoving = true;
		} else {
			isMoving = false;
		}

		//log_debug(QString("Doppler Perserntage!  %1   %2").arg(key).arg(QString::number( persentage, 'f', 6 )));

		if(isMoving) {
			emit signalDopplerDetect(tr("Detect average Dopler: %1 on frequency: %2    Correlation on channel:  %3")
									 .arg(averageDopler).arg(currentFreq).arg(key));
			m_view->setDoplerMessage(tr("Detect average Dopler: %1 on frequency: %2 mHz      Correlation:  %3")
									 .arg(averageDopler).arg(currentFreq).arg(key));
		} else {
			m_view->setDoplerMessage("");
		}
	}
}

void ControlPanelController::checkSolverResult()
{
	//log_debug("Check >>> ");
	if(!m_isFollowMode) {
		return;
	}

	double listFreq = (int)(*m_itCheckMode).frequency;

	if(!m_workCheckList) {
		return;
	}

	//m_IdDetetcted.clear();
	m_listsDialog->clearDetectFreq(m_tabId);
	bool toRemove = true;

	if(!m_solverResultList.isEmpty()) {
		foreach (solverResult res, m_solverResultList) {
			int index = findResultInList(res);
			if( index >= 0 && !m_IdDetetcted.contains(index) ) {
				m_IdDetetcted.append(index);
			}

			if( fabs(listFreq-res.freq) < 2 ) {
				toRemove = false;
			}
		}
	}

	if(toRemove) {
		solverResult res;
		res.freq = listFreq;
		int index = findResultInList(res);
		m_IdDetetcted.removeAll(index);
	}

	foreach (int index, m_IdDetetcted) {
		m_listsDialog->addDetectFreq(m_listOfFreqs.at(index).frequency, index, m_tabId);
	}
}

bool ControlPanelController::slotIncCheckMode()
{
	if(m_isDopplerSearch) {
		checkDopplerResult();
	} else {
		checkSolverResult();
	}

	m_solverResultList.clear();

	if( !m_IdDetetcted.isEmpty() ) {
		m_itDetected++;

		if(m_itDetected >= m_IdDetetcted.size()) {
			m_itDetected = -1;
		} else {
			return false;
		}
	}


	for(int i = 0; i<m_listOfFreqs.size() + 1; i++) {
		m_itCheckMode++;

		if(m_itCheckMode == m_listOfFreqs.end())
		{
			m_itCheckMode = m_listOfFreqs.begin();
		}

		bool state = (*m_itCheckMode).isChecked;

		if(!state)
		{
			continue;
		} else {
			return true;
		}
	}

	return false;
}

void ControlPanelController::slotCheckModeSetFreq()
{
	m_mainStation = NULL;
	double freq;
	double band;

	if(m_listOfFreqs.size() == 0) {
		return;
	}

	//log_debug(QString("Detected list sz: %1  ").arg(m_IdDetetcted.size()));
	for(int i = 0; i<m_IdDetetcted.size(); i++) {
		//log_debug(QString("Detected list ind: %1  ").arg(m_IdDetetcted.at(i)));
	}


	if( !slotIncCheckMode() ) {

		if( !m_IdDetetcted.isEmpty() ) {

			if(m_listOfFreqs.size() > m_IdDetetcted.at(m_itDetected) ) {
				freq = m_listOfFreqs.at( m_IdDetetcted.at(m_itDetected) ).frequency;
				band = m_listOfFreqs.at( m_IdDetetcted.at(m_itDetected) ).bandwidth;
				m_setupController->slotOnSetCommonFreq((freq), band);
				m_listsDialog->setDetectPointer(m_itDetected, m_tabId);
				m_workCheckList = false;
				m_timerCheck.start(m_timerCheckIntervalDetected);
			}
		}

		//No checked signals

		return;
	}

	freq = (*m_itCheckMode).frequency;
	band = (*m_itCheckMode).bandwidth;

	m_setupController->slotOnSetCommonFreq((freq), band);

	if(m_view->isMaitenance()) {
		m_timerCheck.start(1000);
	} else if(m_isDopplerSearch) {
		m_timerCheck.start(m_timerCheckIntervalDopler);
	} else {
		m_timerCheck.start(m_timerCheckInterval);
	}


	m_workCheckList = true;
	m_listsDialog->setCheckPointer(m_listOfFreqs.indexOf(*m_itCheckMode), m_tabId);
}

void ControlPanelController::slotDown1MHz()
{
	//	if (NULL == m_dbManager) {
	//		return;
	//	}

	if(m_currentFreq > MINIMUM_FREQ)
	{
		m_currentFreq = m_currentFreq - 1;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotDown10MHz()
{
	//	if (NULL == m_dbManager) {
	//		return;
	//	}

	if(m_currentFreq > MINIMUM_FREQ + 10)
	{
		m_currentFreq = m_currentFreq - 10;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotDown100MHz()
{
	//	if (NULL == m_dbManager) {
	//		return;
	//	}

	if(m_currentFreq > MINIMUM_FREQ + 100)
	{
		m_currentFreq = m_currentFreq - 100;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotUp1MHz()
{
	//	if (NULL == m_dbManager) {
	//		return;
	//	}

	if(m_currentFreq < MAXIMUM_FREQ)
	{
		m_currentFreq = m_currentFreq + 1;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);

	//	/sendMail();
}

void ControlPanelController::slotUp10MHz()
{
	//	if (NULL == m_dbManager) {
	//		return;
	//	}

	if(m_currentFreq < MAXIMUM_FREQ + 10)
	{
		m_currentFreq = m_currentFreq + 10;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotUp100MHz()
{
	//	if (NULL == m_dbManager) {
	//		return;
	//	}

	if(m_currentFreq < MAXIMUM_FREQ + 100)
	{
		m_currentFreq = m_currentFreq + 100;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::changeCorrelationStatus(QString correlationStatus)
{
	m_view->changeCorrelationStatus(correlationStatus);
}

void ControlPanelController::changeCorrelationStatusActive(bool isActive)
{
	m_view->changeCorrelationStatusActive(isActive);

//	if( m_solverResultList.isEmpty() ) {
//		m_view->changeQualityStatus( 0 );
//		return;
//	}
}

void ControlPanelController::setLocationSettings()
{
	RdsProtobuf::ClientMessage_OneShot_Location msg = m_setupController->getCurrentLocation();

	m_view->setLocationSettings(msg);
}

void ControlPanelController::onMethodCalled(const QString &method, const QVariant &argument)
{
	if (method == RPC_SLOT_SERVER_SEND_QUALITY_STATUS) {
		int status;
		QDataStream stream(argument.toByteArray());

		stream >> status;

		m_view->changeQualityStatus(status);
	}
	if (method == RPC_SLOT_SERVER_SEND_SETTINGS) {
		CommonParams::Parameters msg;
		QByteArray data = argument.toByteArray();
		msg.ParseFromArray( data.data(), data.size() );
		if(msg.serverid() != m_tabId)
		{
			return;
		}
//		if(msg.has_startfreq())
//		{
//			m_view->setStartFreq(msg.startfreq());
//		}
//		if(msg.has_finishfreq())
//		{
//			m_view->setStartFreq(msg.finishfreq());
//		}
		if(msg.has_mergesysctrl())
		{
			m_view->setSystemMerge(msg.mergesysctrl());
		}
		if(msg.has_mode())
		{
			m_view->setMode(msg.mode());
		}
//		if(msg.has_panorama())
//		{
//			m_view->setPanorama(msg.panorama());
//		}
//		if(msg.has_sleep())
//		{
//			m_view->setSleepMode(msg.sleep());
//		}
//		if(msg.has_spectrums())
//		{
//			m_view->setReceiveSpectrums(msg.spectrums());
//		}
	}
}

void ControlPanelController::stopLocationRequest()
{
	m_view->setReceiveSpectrums(false);
	setSpectrumReveive(false);
}

void ControlPanelController::setSpectrumReveive(bool val)
{
	m_view->setReceiveSpectrums(val);
}

bool ControlPanelController::isPanorama(double &start, double &end)
{
	return m_view->getParoramaState(start, end);
}

void ControlPanelController::setListDialog(ListsDialog *dlg)
{
	m_listsDialog = dlg;
}

void ControlPanelController::onSetSleepMode(bool val)
{
	m_setupController->setSleepMode(val);
}

void ControlPanelController::slotDopplerStatus(QString name, double doppler)
{
	if(!_isnan(doppler)) {
		//doppler = qrand()%20;
		m_dopplerMap.insertMulti(name, doppler);
		//log_debug(QString("insertDoppler %1 %2").arg(name).arg(doppler));
	}
}

void ControlPanelController::setSolverConnectState(bool b)
{
	if(m_view) {
		m_view->setSolverConnectState(b);
	}
}
