#include "RpcClientBase.h"

RpcClientBase::RpcClientBase(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;
	m_clientPeer = new QxtRPCPeer(this);
	reconnectTimer = new QTimer(this);
	connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(connectToServer()));
}


bool RpcClientBase::start(QString& ipAddress, quint16 port)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(connectedToServerSlot()) );
	connect(m_clientPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(connectionToServerFailedSlot()));
	connect(m_clientPeer, SIGNAL(disconnectedFromServer()), this, SLOT(connectionToServerFailedSlot()));

	m_port = port;
	m_ipAddress = ipAddress;

	connectToServer();

	return true;
}

void RpcClientBase::stop()
{
	m_clientPeer->disconnectAll();
}


void RpcClientBase::registerListener(IRpcListener *listener)
{
	m_listeners.append(listener);
}

void RpcClientBase::deregisterListener(IRpcListener *listener)
{
	int index = m_listeners.indexOf(listener);

	if (index < 0){
		return;
	}

	m_listeners.removeAt(index);
}


void RpcClientBase::connectToServer()
{
	m_logger->debug(QString("RPC-client status: connecting to server %1:%2").arg(m_ipAddress).arg(QString::number(m_port)));
	m_clientPeer->connect(m_ipAddress, m_port);
}

void RpcClientBase::connectedToServerSlot()
{
	m_logger->debug("RPC-client status: connected to server");
	reconnectTimer->stop();
}

void RpcClientBase::connectionToServerFailedSlot()
{
	m_logger->debug("ERROR RPC-client unable to connect to server");
	reconnectTimer->start(1000);
}
