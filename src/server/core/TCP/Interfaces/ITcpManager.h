#ifndef ITCPMANAGER_H
#define ITCPMANAGER_H

#include <qglobal.h>

class IRpcControllerBase;
class ITcpServerController;
class QObject;

class ITcpManager
{

public:
	virtual ~ITcpManager() {}

	virtual void addTcpDevice(const QString& deviceName, const int& type)	= 0;
	virtual void removeTcpDevice(const QString& deviceName)	= 0;
	virtual void setRpcServer(IRpcControllerBase* rpcServer)	= 0;
	virtual void setTcpServer(ITcpServerController* tcpServer)	= 0;
	virtual QObject* asQObject()	= 0;

	/// TODO: add sendDataToDevice from some internal message
};

#endif // ITCPMANAGER_H
