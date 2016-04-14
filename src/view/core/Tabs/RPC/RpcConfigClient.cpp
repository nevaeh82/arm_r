#include "RpcConfigClient.h"

RpcConfigClient::RpcConfigClient(QObject* parent) :
	RpcRoutedClient(RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SIGNAL(connectionEstablishedSignal()));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, this, SLOT(receivedStationListSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_LOCSYSTEM, this, SLOT(receivedLocSystem(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION, this, SLOT(receivedAtlantConfigSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, this, SLOT(receivedDbConfigurationSlot(QByteArray)));
}

RpcConfigClient::~RpcConfigClient()
{
}

void RpcConfigClient::receivedStationListSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, data);
	}
}

void RpcConfigClient::receivedLocSystem(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_LOCSYSTEM, data);
	}
}

void RpcConfigClient::receivedAtlantConfigSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION, data);
	}
}

void RpcConfigClient::receivedDbConfigurationSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, data);
	}
}

void RpcConfigClient::requestGetStationList(const QString& filename)
{
	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, filename);
}

void RpcConfigClient::requestGetAtlantConfiguration(const QString& filename)
{
	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_ATLANT_CONFIGURATION, filename);
}

void RpcConfigClient::requestGetDbConfiguration(const QString& filename)
{
	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, filename);
}
