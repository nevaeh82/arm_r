#ifndef ITELEMETRYRPCCONTROLLER_H
#define ITELEMETRYRPCCONTROLLER_H

#include <QString>

class ITelemetryRpcController
{
public:
    virtual ~ITelemetryRpcController(){}
    virtual void getKey() = 0;

    virtual void receiveKey(QString) = 0;
    virtual void getKeySignal() = 0;
};


#endif // ITELEMETRYRPCCONTROLLER_H
