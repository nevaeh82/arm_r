#include "CorrelationControllersContainer.h"
#include <QDebug>

CorrelationControllersContainer::CorrelationControllersContainer(QObject *parent):
	QObject(parent)
{
}

CorrelationControllersContainer::~CorrelationControllersContainer()
{
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
        }
		CorrelationWidgetController* controller = new CorrelationWidgetController(this);
		controller->appendView(cor);

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
