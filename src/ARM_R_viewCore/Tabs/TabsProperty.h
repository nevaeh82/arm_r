#ifndef TABSPROPERTY_H
#define TABSPROPERTY_H

#include <QString>
#include <QMap>

//#include "MainTab.h"

#include "../GraphicWidget.h"

class TabsProperty
{
public:
    TabsProperty();

private:
    /// punkt Id
    int                 _id;
    /// name
    QString             _name;
    /// latitude
    double              _latitude;
    /// longitude
    double              _longitude;
    /// IP PRM300
    QString             _ip_prm300;
    /// IP ADC
    QString             _ip_ADC;
    /// port ADC
    quint16             _port_ADC;


    GraphicWidget*              _grafic_widget;

    QMap<int, GraphicWidget *>  _map_correlations;

public:
    void                destroy();
    int                 get_id();
    QString             get_name();
    GraphicWidget       *get_graphic_widget();
    double              get_latitude();
    double              get_longitude();
    QString             get_ip_prm300();
    QString             get_ip_adc();
    quint16             get_port_adc();
    QMap<int, GraphicWidget *> &get_correlations();


    void set_id(int id);
    void set_name(QString name);
    void set_latitude(double lat);
    void set_longitude(double lon);
    void set_ip_prm300(QString ip);
    void set_ip_adc(QString ip);
    void set_port_adc(quint16 port);

    void set_graphic_widget(GraphicWidget *gr);
    void set_graphic_widget_correlation(int id, GraphicWidget *gr_corr);
};

#endif // TABSPROPERTY_H
