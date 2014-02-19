#include "ControlPanelController.h"

#include "UiDefines.h"

ControlPanelController::ControlPanelController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
	m_dbManager = NULL;
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
}

void ControlPanelController::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void ControlPanelController::onPanoramaStateChangedSlot(bool isEnabled)
{

}


void ControlPanelController::onAutoSearchStateChangedSlot(bool isEnabled)
{

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
	m_dbManager->updatePropertyForAllObjects(DB_START_PROPERTY, start);
	m_dbManager->updatePropertyForAllObjects(DB_STOP_PROPERTY, end);
}
