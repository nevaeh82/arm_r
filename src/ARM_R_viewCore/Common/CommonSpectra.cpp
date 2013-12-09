#include "CommonSpectra.h"
#include <QDebug>

CommonSpectra::CommonSpectra()
{
    _map_spectrum_widgets = new QMap<int, IGraphicWidget* >;
}

CommonSpectra::~CommonSpectra()
{
    QMap<int, IGraphicWidget* >::iterator it;
    for(it = _map_spectrum_widgets->begin(); it != _map_spectrum_widgets->end(); ++it)
    {
//        GraphicWidget *p = it.value();
//        delete p;//it.value();
    }
    delete _map_spectrum_widgets;
}

int CommonSpectra::init(QString path_to_ini_file)
{
    return 0;
}

int CommonSpectra::init(int count)
{
    return 0;
}

int CommonSpectra::set(int id, IGraphicWidget *widget)
{
    IGraphicWidget* spectrum = widget;
    _map_spectrum_widgets->insert(id, spectrum);
    return 0;
}

IGraphicWidget* CommonSpectra::get(int id)
{
    if(_map_spectrum_widgets->contains(id))
    {
        return _map_spectrum_widgets->value(id);
    }
    return NULL;
}

int CommonSpectra::count(int id)
{
    return _map_spectrum_widgets->count();
}

QMap<int, IGraphicWidget *>* CommonSpectra::get_components()
{
    return _map_spectrum_widgets;
}
