#ifndef ITCPMANAGER_H
#define ITCPMANAGER_H

class IRPC;
class QObject;

class ITcpManager
{
public:
	virtual ~ITcpManager() {}

	virtual void setRpcServer(IRPC* rpcServer)	= 0;
	virtual QObject* asQObject()	= 0;
};

#endif // ITCPMANAGER_H
