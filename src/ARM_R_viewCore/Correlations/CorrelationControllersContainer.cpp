#include "CorrelationControllersContainer.h"
#include <QDebug>

CorrelationControllersContainer::CorrelationControllersContainer(QObject *parent):
	QObject(parent)
{
	m_mapWidgets = new QMap<int, ICorrelationWidget* >;
}

CorrelationControllersContainer::~CorrelationControllersContainer()
{
	QMap<int, ICorrelationWidget* >::iterator it;
	for(it = m_mapWidgets->begin(); it != m_mapWidgets->end(); ++it)
    {
        delete it.value();
    }

	delete m_mapWidgets;
}

int CorrelationControllersContainer::init(int count)
{
    for(int i = 0; i < count; i++)
    {
		CorrelationWidget* cor = new CorrelationWidget();
		CorrelationWidgetController* controller = new CorrelationWidgetController(cor);
		controller->appendView(cor);

		m_mapWidgets->insert(i, controller);
    }
    return 0;
}

ICorrelationWidget* CorrelationControllersContainer::get(int id)
{
	m_mux.lock();
	ICorrelationWidget *gr = NULL;
	if(m_mapWidgets->contains(id))
    {
		gr = m_mapWidgets->value(id);
    }
	m_mux.unlock();

    return gr;
}

int CorrelationControllersContainer::count()
{
	return m_mapWidgets->count();
}
