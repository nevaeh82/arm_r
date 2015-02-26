#ifndef IGRAPHICWIDGET_H
#define IGRAPHICWIDGET_H

#include <QVector>

#include "../Common/IMessage.h"

class IGraphicWidget
{
public:
    virtual ~IGraphicWidget(){};

    virtual void set(QVector<double> data)      = 0;
    virtual void set_command(IMessage *msg)     = 0;
};

#endif // IGRAPHICWIDGET_H
