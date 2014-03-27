#ifndef IRPCCONTROLLERBASE_H
#define IRPCCONTROLLERBASE_H

#include "IRpcListener.h"
#include "BaseSubject.h"

class QHostAddress;

class IRpcControllerBase: public BaseSubject<IRpcListener>
{
public:
    virtual ~IRpcControllerBase(){}

	virtual bool start(quint16 port, QHostAddress ipAddress) = 0;
    virtual void stop() = 0;

};

#endif // IRPCCONTROLLERBASE_H
