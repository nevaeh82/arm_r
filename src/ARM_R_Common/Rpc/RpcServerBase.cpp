#include "RpcServerBase.h"

RpcServerBase::RpcServerBase(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
	m_serverPeer = new QxtRPCPeer(this);
}

bool RpcServerBase::start(quint16 port)
{
	bool result = m_serverPeer->listen(QHostAddress::Any, port);

	if (!result){
		m_logger->debug(QString("RPC-server was not started on port: %1").arg(QString::number(port)));
		return false;
	}

	m_logger->debug(QString("RPC-server started successfully and listens to port: %1").arg(QString::number(port)));

	return true;
}

void RpcServerBase::stop()
{
	m_serverPeer->stopListening();
	m_serverPeer->disconnectAll();
}

void RpcServerBase::registerListener(IRpcListener *listener)
{
	m_listeners.append(listener);
}


void RpcServerBase::deregisterListener(IRpcListener *listener)
{
	int index = m_listeners.indexOf(listener);

	if (index < 0){
		return;
	}

	m_listeners.removeAt(index);
}
