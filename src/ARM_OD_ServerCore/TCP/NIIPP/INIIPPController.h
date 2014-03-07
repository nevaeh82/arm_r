#ifndef INIIPPCONTROLLER_H
#define INIIPPCONTROLLER_H

#include <QPointF>

class INIIPPController
{
public:
    virtual ~INIIPPController(){};

    virtual void set_power(double value) = 0;
    virtual void set_switch_on(bool state) = 0;
    virtual void set_antenna_type(int value) = 0;
    virtual void send_evil(QPointF point, double alt) = 0;
};

#endif // INIIPPCONTROLLER_H
