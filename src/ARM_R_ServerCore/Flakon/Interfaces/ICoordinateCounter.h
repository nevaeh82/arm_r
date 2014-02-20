#ifndef ICOORDINATECOUNTER_H
#define ICOORDINATECOUNTER_H

#include "Message.h"

class ICoordinateCounter
{

public:
	virtual ~ICoordinateCounter(){}

	virtual void sendData(const IMessage<QByteArray>* message)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ICOORDINATECOUNTER_H
