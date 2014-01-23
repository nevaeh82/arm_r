#ifndef IRPCCONTROLLERBASE_H
#define IRPCCONTROLLERBASE_H

#include "IRpcListener.h"

class IRpcControllerBase
{
public:
    virtual ~IRpcControllerBase(){}

    virtual bool start(quint16 port) = 0;
    virtual void stop() = 0;

    virtual void registerListener(IRpcListener* listener) = 0;
	virtual void deregisterListener(IRpcListener* listener) = 0;

};

#endif // IRPCCONTROLLERBASE_H
