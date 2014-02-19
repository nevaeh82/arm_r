#ifndef ITCPLISTENER_H
#define ITCPLISTENER_H

#include "Interfaces/IMessage.h"

class ITcpListener
{

public:
	virtual ~ITcpListener() {}
	virtual void onMessageReceived(const QString& device, const IMessage<QByteArray>* argument)	= 0;
};

#endif // ITCPLISTENER_H
