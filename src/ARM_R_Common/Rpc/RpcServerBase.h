#ifndef RPCSERVERBASE_H
#define RPCSERVERBASE_H

#include <QObject>
#include <QxtRPCPeer>

#include <PwLogger/PwLogger.h>
#include "Interfaces/IRpcControllerBase.h"

class RpcServerBase : public QObject, public IRpcControllerBase
{
	Q_OBJECT

private:
	quint16 m_port;

protected:
	Pw::Logger::ILogger* m_logger;
	QxtRPCPeer* m_serverPeer;
	QList<IRpcListener*> m_listeners;

public:
	explicit RpcServerBase(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~RpcServerBase(){}

	virtual bool start(quint16 port);
	virtual void stop();

	void registerListener(IRpcListener *listener);
	void deregisterListener(IRpcListener *listener);
};

#endif // RPCSERVERBASE_H
