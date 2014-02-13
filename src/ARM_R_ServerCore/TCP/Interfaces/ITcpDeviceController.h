#ifndef ITCPDEVICECONTROLLER_H
#define ITCPDEVICECONTROLLER_H

#include <qglobal.h>
class QObject;

class ITcpDeviceController
{

public:
	virtual ~ITcpDeviceController() {}

	virtual void connectToHost(const QString& host, const quint32& port)	= 0;
	virtual void disconnectFromHost()	= 0;
	virtual void isConnected()	= 0;
	virtual void sendData(const QByteArray& data)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPDEVICECONTROLLER_H
