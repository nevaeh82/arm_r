#include "RpcServerBase.h"

RpcServerBase::RpcServerBase(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
	m_serverPeer = new QxtRPCPeer(this);
}

bool RpcServerBase::start(quint16 port, QHostAddress address)
{
	bool result = m_serverPeer->listen(address, port);

	if (!result){
		m_logger->debug(QString("RPC-server was not started on %1:%2").arg(address.toString()).arg(QString::number(port)));
		return false;
	}

	m_logger->debug(QString("RPC-server started successfully and listens to %1:%2").arg(address.toString()).arg(QString::number(port)));

	return true;
}

void RpcServerBase::stop()
{
	m_serverPeer->stopListening();
	m_serverPeer->disconnectAll();
}
