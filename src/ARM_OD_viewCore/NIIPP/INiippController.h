#ifndef INIIPPCONTROLLER_H
#define INIIPPCONTROLLER_H

#include <QPointF>

class INiiPPController
{
public:
    virtual ~INiiPPController(){};

    virtual void set_power(double value) = 0;
    virtual void set_switch_on(bool state) = 0;
    virtual void set_antenna_type(int value) = 0;
    virtual void send_evil(QPointF point, QPointF point_uvoda, double alt, double bearing) = 0;
    virtual int get_id()    = 0;

    virtual void set_angle(double angle)    = 0;

    virtual void set_point(QPointF coord) = 0;
};

#endif // INIIPPCONTROLLER_H
