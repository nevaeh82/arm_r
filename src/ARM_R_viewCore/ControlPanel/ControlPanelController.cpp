#include "ControlPanelController.h"

#include "UiDefines.h"

#include <Logger.h>

ControlPanelController::ControlPanelController(QObject *parent)
	: QObject(parent)
{
	m_view = NULL;
	m_dbManager = NULL;
    m_currentFreq = 0;
    m_startFreq = 0;
    m_finishFreq = 0;	
	m_dbStation = NULL;
	m_rpcFlakonClient = NULL;
	m_mainStation = NULL;
}

ControlPanelController::~ControlPanelController()
{

}

void ControlPanelController::appendView(ControlPanelWidget *view)
{
	m_view = view;

	connect(m_view, SIGNAL(autoSearchCheckedSignal(bool)), this, SLOT(onAutoSearchStateChangedSlot(bool)));
	connect(m_view, SIGNAL(panoramaCheckedSignal(bool)), this, SLOT(onPanoramaStateChangedSlot(bool)));
	connect(m_view, SIGNAL(commonFreqChangedSignal(int)), this, SLOT(onCommonFrequencyChangedSlot(int)));
	connect(m_view, SIGNAL(bandwidthChangedSignal(int,int)), this, SLOT(onBandWidthChangedSlot(int,int)));
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

	connect(this, SIGNAL(signalSetComonFreq(int)), m_view, SLOT(slotChangeCommonFreq(int)));
	connect(this, SIGNAL(setCorrelationStatus(QString)), this, SLOT(changeCorrelationStatus(QString)));
	connect(this, SIGNAL(setCorrelationStatusActive(bool)), this, SLOT(changeCorrelationStatusActive(bool)));

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
	emit setCorrelationStatus(" " + QString::number(value) + QString(" MHz"));
}

void ControlPanelController::onPanoramaStateChangedSlot(bool isEnabled)
{
	foreach (IControlPanelListener* receiver, m_receiversList) {
		receiver->onGlobalPanoramaEnabled(isEnabled);
	}
}


void ControlPanelController::onAutoSearchStateChangedSlot(bool isEnabled)
{
	foreach (IControlPanelListener* receiver, m_receiversList) {
		receiver->onGlobalAutoSearchEnabled(isEnabled);
	}
}

void ControlPanelController::onCommonFrequencyChangedSlot(int value)
{
	if (NULL == m_dbManager) {
		return;
	}

	m_currentFreq = value;

	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);
}

void ControlPanelController::onBandWidthChangedSlot(int start, int end)
{
	m_dbManager->updatePropertyForAllObjects(DB_PANORAMA_START_PROPERTY, start);
    m_dbManager->updatePropertyForAllObjects(DB_PANORAMA_END_PROPERTY, end);
}

void ControlPanelController::slotManualMode()
{
	if(m_timerCheck.isActive())
		m_timerCheck.stop();
	if(m_timer.isActive())
		m_timer.stop();
}

void ControlPanelController::slotScanMode(int start, int finish)
{
	if(m_timerCheck.isActive())
		m_timerCheck.stop();

    m_startFreq = start;
    m_currentFreq = start;
    m_finishFreq = finish;
    m_timer.start(5000);
}

void ControlPanelController::slotCheckMode()
{	
	if(m_timer.isActive())
		m_timer.stop();

	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("White", m_listOfFreqs);
	log_debug(QString("data for check = %1").arg(m_listOfFreqs.count()));
	m_itCheckMode = m_listOfFreqs.begin();
	slotCheckModeSetFreq();
	m_timerCheck.start(60000);
}

void ControlPanelController::slotViewMode()
{
	if(m_timerCheck.isActive())
		m_timerCheck.stop();
	if(m_timer.isActive())
		m_timer.stop();

	bool err = m_dbStation->getFrequencyAndBandwidthByCategory("White", m_listOfFreqs);
	log_debug(QString("data for leading = %1").arg(m_listOfFreqs.count()));
	m_itCheckMode = m_listOfFreqs.begin();
	slotCheckModeSetFreq();
	m_timerCheck.start(60000);

	/// TODo in next release
//	ListWhiteDialog* listView = new ListWhiteDialog(m_view);
//	ListWhiteDialogController* listController = new ListWhiteDialogController(m_dbStation->getDataBase(), this);
//	bool isOpen = m_dbStation->getDataBase().isOpen();
//	if(!isOpen)
//	{
//		QMessageBox msgBox;
//		msgBox.setText(tr("DataBase is not opened!"));
//		msgBox.exec();
//		return;
//	}

//	listController->appendView(listView);
//	listView->show();
}

void ControlPanelController::slotChangeFreq()
{
    if(m_currentFreq >= m_finishFreq)
    {
        m_currentFreq = m_startFreq;
    }
    m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);
	m_currentFreq += INTERVAL;
}

void ControlPanelController::slotCheckModeSetFreq()
{
	m_mainStation = NULL;

	if(m_listOfFreqs.size() == 0) {
		return;
	}

	if(m_itCheckMode == m_listOfFreqs.end())
	{
		m_itCheckMode = m_listOfFreqs.begin();
	}

	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, (*m_itCheckMode).frequency);
	m_dbManager->updatePropertyForAllObjects(DB_SELECTED_PROPERTY, (*m_itCheckMode).bandwidth);

	foreach(Station* st, m_stationsMap)
	{
		m_rpcFlakonClient->sendBandwidth(st->getId(), (*m_itCheckMode).bandwidth);
		if(st->getName() == (*m_itCheckMode).stationName)
		{
			m_mainStation = st;
		}
	}

	/// set main station fo correlations
	QString leadStation;
	if(m_mainStation)
	{
		leadStation = (*m_itCheckMode).stationName;
	}
	else
	{
		m_mainStation = m_stationsMap.value(0);
		leadStation = tr("Auto");
	}

	if(m_mainStation == 0) {
		return;
	}

	m_rpcFlakonClient->sendMainStationCorrelation(m_mainStation->getId(), leadStation);

	m_dbManager->updatePropertyForAllObjects(DB_LEADING_OP_PROPERTY, leadStation);

	m_rpcFlakonClient->sendCorrelation(m_mainStation->getId(), true);

	m_itCheckMode++;
}

void ControlPanelController::slotDown1MHz()
{
	if (NULL == m_dbManager) {
		return;
	}

	if(m_currentFreq > MINIMUM_FREQ)
	{
		m_currentFreq = m_currentFreq - 1;
	}
	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotDown10MHz()
{
	if (NULL == m_dbManager) {
		return;
	}

	if(m_currentFreq > MINIMUM_FREQ + 10)
	{
		m_currentFreq = m_currentFreq - 10;
	}
	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotDown100MHz()
{
	if (NULL == m_dbManager) {
		return;
	}

	if(m_currentFreq > MINIMUM_FREQ + 100)
	{
		m_currentFreq = m_currentFreq - 100;
	}
	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotUp1MHz()
{
	if (NULL == m_dbManager) {
		return;
	}

	if(m_currentFreq < MAXIMUM_FREQ)
	{
		m_currentFreq = m_currentFreq + 1;
	}
	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotUp10MHz()
{
	if (NULL == m_dbManager) {
		return;
	}

	if(m_currentFreq < MAXIMUM_FREQ + 10)
	{
		m_currentFreq = m_currentFreq + 10;
	}
	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::slotUp100MHz()
{
	if (NULL == m_dbManager) {
		return;
	}

	if(m_currentFreq < MAXIMUM_FREQ + 100)
	{
		m_currentFreq = m_currentFreq + 100;
	}
	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, m_currentFreq);

	emit signalSetComonFreq(m_currentFreq);
}

void ControlPanelController::changeCorrelationStatus(QString correlationStatus)
{
	m_view->changeCorrelationStatus(correlationStatus);
}

void ControlPanelController::changeCorrelationStatusActive(bool isActive)
{
	m_view->changeCorrelationStatusActive(isActive);
}
