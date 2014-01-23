#ifndef IPARSER_H
#define IPARSER_H

#include "IMessage.h"
#include "ICoderReceiver.h"

class QByteArray;

class ICoder
{
public:
	virtual ~ICoder(){}
	virtual QByteArray encode(const IMessage<QByteArray>* message) const = 0;
	virtual void decode(const QByteArray& data) = 0;
	virtual void registerReceiver(ICoderReceiver* receiver) = 0;
	virtual void deregisterReceiver(ICoderReceiver* receiver) = 0;
};

#endif // IPARSER_H
