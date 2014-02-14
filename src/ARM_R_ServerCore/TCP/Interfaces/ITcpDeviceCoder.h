#ifndef ITCPDEVICECODER_H
#define ITCPDEVICECODER_H

#include "Message.h"

class ITcpDeviceCoder
{

public:
	virtual ~ITcpDeviceCoder() {}

	virtual IMessage<QByteArray>* encode(const QByteArray& data)	= 0;
	virtual QByteArray decode(const IMessage<QByteArray>* message)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPDEVICECODER_H
