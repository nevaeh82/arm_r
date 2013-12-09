#include "CommonCorrelations.h"
#include <QDebug>

CommonCorrelations::CommonCorrelations()
{
    _map_widgets = new QMap<int, IGraphicWidget* >;
}

CommonCorrelations::~CommonCorrelations()
{
    QMap<int, IGraphicWidget* >::iterator it;
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
        CorrelationWidget *cor = new CorrelationWidget(0,0, "", i);//( _tab_property->get_ip_prm300(), _tab_property->get_ip_adc(), _tab_property->get_port_adc(), this, view);
//        vboxlayout->addWidget(cor, Qt::AlignJustify);
        _map_widgets->insert(i, cor);
    }
    return 0;
}

int CommonCorrelations::set(int id, IGraphicWidget *widget)
{
    IGraphicWidget* spectrum = widget;
    _map_widgets->insert(id, spectrum);
    return 0;
}

IGraphicWidget* CommonCorrelations::get(int id)
{
    _mux.lock();
    IGraphicWidget *gr = NULL;
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

QMap<int, IGraphicWidget *>* CommonCorrelations::get_components()
{
    return _map_widgets;
}

