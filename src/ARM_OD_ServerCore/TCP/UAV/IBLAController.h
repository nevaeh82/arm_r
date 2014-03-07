#ifndef IBLACONTROLLER_H
#define IBLACONTROLLER_H

#include <QVector>

class IBLAController
{
public:
    virtual ~IBLAController(){};
    virtual void connect_bla(QVector<quint16>)      = 0;
};

#endif // IBLACONTROLLER_H
