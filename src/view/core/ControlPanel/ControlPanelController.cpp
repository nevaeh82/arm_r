#include "ControlPanelController.h"

#include "UiDefines.h"

#include <Logger/Logger.h>

#include "Rpc/RpcDefines.h"

#define TIMER_INTERVAL 5000
#define TIMERCHECK_INTERVAL 5000
#define TIMERCHECK_INTERVAL_DETECTED 2000
#define TIMERINTERVAL_KEY "ControlPanel"

ControlPanelController::ControlPanelController(QObject *parent)
	: QObject(parent)
{
	m_view = NULL;
	m_dbManager = NULL;
	m_currentFreq = 20;
	m_currentFreqFromPlot = 20;

	m_workCheckList = false;

	m_startFreq = 0;
	m_finishFreq = 0;
	m_dbStation = NULL;
	m_rpcFlakonClient = NULL;
	m_mainStation = NULL;
	m_listsDialog = NULL;

	init();

	connect(this, SIGNAL(signalSetCentralFreq(double)), this, SLOT(setCentralFreqValueInternal(double)));
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

	if(data.isEmpty()) {
		m_view->changeQualityStatus( 0 );
	} else {

		SolverProtocol::Packet pkt;
		pkt.ParseFromArray(data.data(), data.size());

		m_solverResultList.clear();

		SolverProtocol::Packet_DataFromSolver_SolverSolution solPkt;
		if(pkt.datafromsolver().has_solution_manual_altitude()) {
			solPkt = pkt.datafromsolver().solution_manual_altitude();

			for( int i = 0; i < solPkt.trajectory_size(); i++ ) {
				solverResult res;

				res.dateTime = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).datetime();
				res.freq = solPkt.trajectory(i).central_frequency();
				res.state = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).state();
				res.quality = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).quality();
				quality |= res.quality;

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
				res.quality = solPkt.trajectory(i).motionestimate( solPkt.trajectory(i).motionestimate_size() - 1 ).quality();
				quality |= res.quality;

				m_solverResultList.append( res );
			}
		}

		m_view->changeQualityStatus( quality );
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
}

bool ControlPanelController::checkSolverResult(int freq)
{
	foreach (solverResult res, m_solverResultList) {
		if(abs(res.freq -freq) < 2) {
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
//	if (NULL == m_dbManager) {
//		return;
//	}

	m_currentFreq = value;

	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::onBandWidthChangedSlot(int start, int end)
{
	m_dbManager->updatePropertyForAllObjects(DB_PANORAMA_START_PROPERTY, start);
	m_dbManager->updatePropertyForAllObjects(DB_PANORAMA_END_PROPERTY, end);
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
	m_listsDialog->clearDetectFreq();
	m_listsDialog->clearWorkList();
}

void ControlPanelController::slotScanMode(int start, int finish)
{
	if(m_timerCheck.isActive())
		m_timerCheck.stop();

	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("Black", m_listOfFreqsBlack);
	m_listsDialog->clearDetectFreq();

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
	m_listOfFreqs.clear();
	m_IdDetetcted.clear();

	m_listsDialog->clearDetectFreq();
	m_listsDialog->clearWorkList();

	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("White", m_listOfFreqs);
	log_debug(QString("data for check = %1").arg(m_listOfFreqs.count()));
	m_itCheckMode = m_listOfFreqs.begin();
	m_itDetected = m_IdDetetcted.begin();

	m_listsDialog->setWorkList(m_listOfFreqs);

	slotCheckModeSetFreq();
	m_timerCheck.start(m_timerCheckInterval);
}

void ControlPanelController::slotViewMode()
{
	slotCheckMode();
	m_isFollowMode = true;

//	if(m_timerCheck.isActive())
//		m_timerCheck.stop();
//	if(m_timer.isActive())
//		m_timer.stop();

//	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("White", m_listOfFreqs);
//	log_debug(QString("data for leading = %1").arg(m_listOfFreqs.count()));
//	m_itCheckMode = m_listOfFreqs.begin();
//	slotCheckModeSetFreq();
//	m_timerCheck.start(m_timerCheckInterval);

//	QString ControlPanelSettingsPath = QCoreApplication::applicationDirPath();
//	ControlPanelSettingsPath.append("./Scan/CheckTimer.ini");
//	QSettings ControlPanelSettings( ControlPanelSettingsPath, QSettings::IniFormat );

//	int interval = ControlPanelSettings.value("timer", 3000).toInt();
//	m_timerCheck.start(interval);
}

void ControlPanelController::slotChangeFreq()
{
	if(m_currentFreq >= m_finishFreq)
	{
		m_currentFreq = m_startFreq;
	} else {
		m_currentFreq += INTERVAL;
	}
	//m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	changeFrequency(m_currentFreq);
}

void ControlPanelController::checkSolverResult()
{
	//log_debug("Check >>> ");
	double listFreq = (int)(*m_itCheckMode).frequency;

	if(!m_workCheckList) {
		return;
	}

	m_IdDetetcted.clear();
	m_listsDialog->clearDetectFreq();
	bool toRemove = true;

	if(!m_solverResultList.isEmpty()) {
		//m_listsDialog->clearDetectFreq();
		foreach (solverResult res, m_solverResultList) {
			int index = findResultInList(res);
			if( index >= 0 && !m_IdDetetcted.contains(index) ) {
				m_IdDetetcted.append(index);
			}

			int tVal = fabs(listFreq-res.freq);
			if( fabs(listFreq-res.freq) < 2 ) {
				toRemove = false;
			}
		}
		//m_itDetected = m_IdDetetcted.begin();
	}

	if(toRemove) {
		solverResult res;
		res.freq = listFreq;
		int index = findResultInList(res);
		m_IdDetetcted.removeAll(index);
	}

	foreach (int index, m_IdDetetcted) {
		m_listsDialog->addDetectFreq(m_listOfFreqs.at(index).frequency, index);
	}
}

bool ControlPanelController::slotIncCheckMode()
{
	checkSolverResult();

	m_solverResultList.clear();

	if( !m_IdDetetcted.isEmpty() ) {
		m_itDetected++;

		if(m_itDetected == m_IdDetetcted.end()) {
			//m_itDetected = m_IdDetetcted.begin();
		} else {
			return true;
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

	log_debug(QString("Detected list sz: %1  ").arg(m_IdDetetcted.size()));
	for(int i = 0; i<m_IdDetetcted.size(); i++) {
		log_debug(QString("Detected list ind: %1  ").arg(m_IdDetetcted.at(i)));
	}


	if( !slotIncCheckMode() ) {

		if( !m_IdDetetcted.isEmpty() ) {

			if(m_listOfFreqs.size() >= *m_itDetected ) {
				freq = m_listOfFreqs.at( *m_itDetected ).frequency;
				band = m_listOfFreqs.at( *m_itDetected ).bandwidth;
				m_setupController->slotOnSetCommonFreq(int(freq+0.5), band);
				m_listsDialog->setDetectPointer(m_IdDetetcted.indexOf(*m_itDetected));
				m_workCheckList = false;
				m_timerCheck.start(m_timerCheckIntervalDetected);
				log_debug(QString("Detected list set id:%1 freq:%2 %3").arg(*m_itDetected)
																		.arg(int(freq+0.5))
						  .arg(m_itDetected == m_IdDetetcted.end()));
			}
		}

		//No checked signals

		return;
	}


	if( !m_IdDetetcted.isEmpty() ) {
		if (m_itDetected != m_IdDetetcted.end()) {
			if( *m_itDetected < 0 ||
				m_listOfFreqs.size()<= *m_itDetected ) {
				m_itDetected = m_IdDetetcted.begin();
			}
			int indVal = *m_itDetected;

			if( !m_IdDetetcted.contains(indVal) ) {
				log_debug(QString("Not contains %1 goto begin").arg(indVal));
				m_itDetected = m_IdDetetcted.begin();
			}

			freq = m_listOfFreqs.at( *m_itDetected ).frequency;
			band = m_listOfFreqs.at( *m_itDetected ).bandwidth;
			m_setupController->slotOnSetCommonFreq(int(freq+0.5), band);
			m_listsDialog->setDetectPointer(m_IdDetetcted.indexOf(*m_itDetected));
			m_workCheckList = false;
			m_timerCheck.start(m_timerCheckIntervalDetected);

			log_debug(QString("Detected list set id:%1 freq:%2 %3").arg(*m_itDetected)
																	.arg(int(freq+0.5))
					  .arg(2));

			return;
		} /*else {
			int test = *m_itDetected;
//			m_itDetected = m_IdDetetcted.begin();
//			m_itDetected--;
			int k = 0;
		}*/
	}

	freq = (*m_itCheckMode).frequency;
	band = (*m_itCheckMode).bandwidth;

	m_setupController->slotOnSetCommonFreq(int(freq+0.5), band);
	m_timerCheck.start(m_timerCheckInterval);
	m_workCheckList = true;
	m_listsDialog->setCheckPointer(m_listOfFreqs.indexOf(*m_itCheckMode));

	log_debug(QString("Check list set id:%1 freq:%2 %3").arg(0)
															.arg(int(freq+0.5))
			  .arg(0));
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

	if( m_solverResultList.isEmpty() ) {
		m_view->changeQualityStatus( 0 );
		return;
	}

	if(!m_solverResultList.isEmpty()) {

//		int test = abs(QDateTime::currentMSecsSinceEpoch() - m_solverResultList.at(0).dateTime);

//		if(abs(QDateTime::currentMSecsSinceEpoch() - m_solverResultList.at(0).dateTime) > 20000) {
//			m_view->changeQualityStatus( 0 );
//			return;
//		}
	}
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

bool ControlPanelController::sleepMode() const
{
	return m_view->sleepMode();
}

void ControlPanelController::setSleepMode(bool val)
{
	m_view->setSleepMode(val);
	m_setupController->setSleepMode(val);
}

void ControlPanelController::onSetSleepMode(bool val)
{
	m_setupController->setSleepMode(val);
}

void ControlPanelController::setSolverConnectState(bool b)
{
	if(m_view) {
		m_view->setSolverConnectState(b);
	}
}
