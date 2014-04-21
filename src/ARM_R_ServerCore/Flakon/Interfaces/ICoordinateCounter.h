#ifndef ICOORDINATECOUNTER_H
#define ICOORDINATECOUNTER_H

#include "MessageSP.h"

class ICoordinateCounter
{

public:
	virtual ~ICoordinateCounter(){}

	virtual void sendData(const MessageSP message) = 0;
	virtual QObject* asQObject() = 0;
	virtual void sendSetCenterFrequency(const double& frequency) = 0;

public slots:
	virtual void setCenterFrequency(const double& frequency) = 0;
};

#endif // ICOORDINATECOUNTER_H
