#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QList>

#include <PwLogger/PwLogger.h>

#include "Interfaces/ITcpManager.h"
#include "../RPC/IRPC.h"
#include "TcpFlakonController.h"

class TcpManager : public QObject, public ITcpManager
{
	Q_OBJECT

private:
	Pw::Logger::ILogger* m_logger;
	IRPC* m_rpcServer;
	QList<BaseTcpDeviceController*> m_tcpDeviceControllers;

public:
	explicit TcpManager(QObject* parent = NULL);
	virtual ~TcpManager();

	// ITcpManager interface
public:
	virtual void setRpcServer(IRPC* rpcServer);
	virtual QObject* asQObject();
};

#endif // TCPMANAGER_H
