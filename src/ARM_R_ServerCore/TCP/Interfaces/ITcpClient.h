#ifndef ITCPCLIENT_H
#define ITCPCLIENT_H

#include <qglobal.h>
class QObject;

class ITcpClient
{

public:
	virtual ~ITcpClient() {}

	virtual void connectToHost(const QString& host, const quint32& port)	= 0;
	virtual void disconnectFromHost()	= 0;
	virtual bool isConnected()	= 0;
	virtual void writeData(const QByteArray& data)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPCLIENT_H
