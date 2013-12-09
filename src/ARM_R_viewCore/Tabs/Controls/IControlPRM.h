#ifndef ICONTROLPRM_H
#define ICONTROLPRM_H

#include <QMap>
#include <QVariant>

class IControlPRM
{
public:
    virtual ~IControlPRM(){};

    virtual void set_status(QMap<int, QVariant> data)   = 0;
};

#endif // ICONTROLPRM_H
