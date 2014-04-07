#include "ControlPanelController.h"

#include "UiDefines.h"

#include <Logger.h>

ControlPanelController::ControlPanelController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_dbManager = NULL;
    m_currentFreq = 0;
    m_startFreq = 0;
    m_finishFreq = 0;
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

}

void ControlPanelController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void ControlPanelController::setDbStationController(IDBStation *dbStationController)
{
	m_dbStation = dbStationController;
}

void ControlPanelController::setRpcFlakonClient(RpcFlakonClient *rpcFlakonClient)
{
	m_rpcFlakonClient = rpcFlakonClient;
}

void ControlPanelController::setMapStations(QMap<int, Station *> stationsMap)
{
	m_stationsMap = stationsMap;
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

	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, value);
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
	if(m_itCheckMode == m_listOfFreqs.end())
	{
		m_itCheckMode = m_listOfFreqs.begin();
	}

	m_dbManager->updatePropertyForAllObjects(DB_FREQUENCY_PROPERTY, (*m_itCheckMode).frequency);
	m_dbManager->updatePropertyForAllObjects(DB_SELECTED_PROPERTY, (*m_itCheckMode).bandwidth);

	foreach(Station* st, m_stationsMap)
	{
		m_rpcFlakonClient->sendBandwidth(st, (*m_itCheckMode).bandwidth);
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
		leadStation = QString("Авто");
	}

	m_rpcFlakonClient->sendMainStationCorrelation(m_mainStation, leadStation);

	m_dbManager->updatePropertyForAllObjects(DB_LEADING_OP_PROPERTY, leadStation);

	m_rpcFlakonClient->sendCorrelation(m_mainStation, true);

	m_itCheckMode++;
}
