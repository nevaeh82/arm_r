#include "ControlPanelController.h"

ControlPanelController::ControlPanelController(QObject *parent) :
	QObject(parent)
{
	m_view = NULL;
}

ControlPanelController::~ControlPanelController()
{

}

void ControlPanelController::appendView(ControlPanelWidget *view)
{
	m_view = view;

	/// TODO: Add UI connects HERE
}
