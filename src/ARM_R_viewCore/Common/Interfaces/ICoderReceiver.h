#ifndef ICODERRECEIVER_H
#define ICODERRECEIVER_H

#include "IMessage.h"

class ICoderReceiver
{
public:
	virtual ~ICoderReceiver(){}
	virtual void onDecodedMessageReceived(const IMessage<QByteArray>*) = 0;
};

#endif // ICODERRECEIVER_H
