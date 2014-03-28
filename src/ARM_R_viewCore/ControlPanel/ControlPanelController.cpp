#include "ControlPanelController.h"

#include "UiDefines.h"

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

}

void ControlPanelController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
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
    m_timer.stop();
}

void ControlPanelController::slotScanMode(int start, int finish)
{
    m_startFreq = start;
    m_currentFreq = start;
    m_finishFreq = finish;
    m_timer.start(5000);
}

void ControlPanelController::slotCheckMode()
{
    m_timer.stop();
}

void ControlPanelController::slotViewMode()
{
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
