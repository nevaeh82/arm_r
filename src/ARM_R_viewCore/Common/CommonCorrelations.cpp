#include "CommonCorrelations.h"
#include <QDebug>

CommonCorrelations::CommonCorrelations()
{
	_map_widgets = new QMap<int, ICorrelationWidget* >;
}

CommonCorrelations::~CommonCorrelations()
{
	QMap<int, ICorrelationWidget* >::iterator it;
    for(it = _map_widgets->begin(); it != _map_widgets->end(); ++it)
    {
        delete it.value();
    }

    delete _map_widgets;
}

int CommonCorrelations::init(QString path_to_ini_file)
{
    return 0;
}

int CommonCorrelations::init(int count)
{
    for(int i = 0; i < count; i++)
    {
		CorrelationWidget* cor = new CorrelationWidget();
		CorrelationWidgetController* controller = new CorrelationWidgetController(cor);

		controller->appendView(cor);
		_map_widgets->insert(i, controller);
    }
    return 0;
}

int CommonCorrelations::set(int id, ICorrelationWidget *widget)
{
	_map_widgets->insert(id, widget);
    return 0;
}

ICorrelationWidget* CommonCorrelations::get(int id)
{
    _mux.lock();
	ICorrelationWidget *gr = NULL;
    if(_map_widgets->contains(id))
    {
        gr = _map_widgets->value(id);
    }
    _mux.unlock();

    return gr;
}

int CommonCorrelations::count(int id)
{
    return _map_widgets->count();
}

QMap<int, ICorrelationWidget *>* CommonCorrelations::get_components()
{
    return _map_widgets;
}

