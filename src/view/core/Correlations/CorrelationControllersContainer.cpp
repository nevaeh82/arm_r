#include "CorrelationControllersContainer.h"
#include <QDebug>
#include "Logger/Logger.h"

CorrelationControllersContainer::CorrelationControllersContainer(QObject *parent):
	QObject(parent)
{
}

CorrelationControllersContainer::~CorrelationControllersContainer()
{
	//log_debug(">>>>>> ");
	//log_debug("<<<<<<");
}

int CorrelationControllersContainer::init(int count, int type)
{
	for(int i = 0; i < count; i++)
	{
		BaseCorrelationWidget* cor;
		if(type == 0) {
			cor = new CorrelationWidget();
        } else {
            cor = new CorrelationIndicatorWidget();
			connect((CorrelationIndicatorWidget*)cor, SIGNAL(signalExpand()),
					this, SIGNAL(signalExpand()));

            connect(m_controlPanel, SIGNAL(skoChanged(float)), cor, SLOT(setNewSko(float)));
		}
		CorrelationWidgetController* controller = new CorrelationWidgetController(type, this);
		controller->appendView(cor);
		controller->setLocationController(m_locationController);

		m_mapWidgets.insert(i, controller);
	}
	return 0;
}

ICorrelationWidget* CorrelationControllersContainer::get(int id)
{
	m_mux.lock();
	ICorrelationWidget *gr = NULL;
	if(m_mapWidgets.contains(id))
	{
		gr = m_mapWidgets.value(id);
	}
	m_mux.unlock();

	return gr;
}

int CorrelationControllersContainer::count()
{
	return m_mapWidgets.count();
}

void CorrelationControllersContainer::setLocationController(LocationSetupWidgetController *controller)
{
    m_locationController = controller;
}

void CorrelationControllersContainer::setControlPanelController(ControlPanelController * obj)
{
    m_controlPanel = obj;
}
