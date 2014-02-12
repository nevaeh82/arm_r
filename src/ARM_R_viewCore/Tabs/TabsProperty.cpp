#include "TabsProperty.h"

TabsProperty::TabsProperty(QObject *parent):
	QObject(parent)
{
	_grafic_widget = NULL;
}

TabsProperty::~TabsProperty()
{

}


void TabsProperty::set_id(int id)
{
    _id = id;
}

void TabsProperty::set_name(QString name)
{
    _name = name;
}

void TabsProperty::set_graphic_widget(SpectrumWidget *gr)
{
    _grafic_widget = gr;
}

void TabsProperty::set_latitude(double lat)
{
    _latitude = lat;
}

void TabsProperty::set_longitude(double lon)
{
    _longitude = lon;
}

void TabsProperty::set_ip_prm300(QString ip)
{
    _ip_prm300 = ip;
}

void TabsProperty::set_ip_adc(QString ip)
{
    _ip_ADC = ip;
}

void TabsProperty::set_port_adc(quint16 port)
{
    _port_ADC = port;
}

int TabsProperty::get_id()
{
    return _id;
}

QString TabsProperty::get_name()
{
    return _name;
}

double TabsProperty::get_latitude()
{
    return _latitude;
}

double TabsProperty::get_longitude()
{
    return _longitude;
}

QString TabsProperty::get_ip_prm300()
{
    return _ip_prm300;
}

QString TabsProperty::get_ip_adc()
{
    return _ip_ADC;
}

quint16 TabsProperty::get_port_adc()
{
    return _port_ADC;
}

SpectrumWidget *TabsProperty::get_graphic_widget()
{
    return _grafic_widget;
}

void TabsProperty::set_graphic_widget_correlation(int id, SpectrumWidget *gr_corr)
{
    _map_correlations.insert(id, gr_corr);
}

QMap<int, SpectrumWidget *> &TabsProperty::get_correlations()
{
    return _map_correlations;
}
