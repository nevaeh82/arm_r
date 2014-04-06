#include "RpcConfigClient.h"

RpcConfigClient::RpcConfigClient(QObject* parent) :
	RpcRoutedClient(RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent)
{
	m_clientPeer->attachSignal(this, SIGNAL(getStationListSignal(QString)), RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST);
	m_clientPeer->attachSignal(this, SIGNAL(getAtlantConfigSignal(QString)), RPC_METHOD_CONFIG_REQUEST_GET_ATLANT_CONFIGURATION);
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, this, SLOT(receivedStationListSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION, this, SLOT(receivedAtlantConfigSlot(QByteArray)));
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

void RpcConfigClient::receivedAtlantConfigSlot(QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION, data);
	}
}

void RpcConfigClient::requestGetStationList(const QString& filename)
{
	emit getStationListSignal(filename);
}

void RpcConfigClient::requestGetAtlantConfiguration(const QString& filename)
{
	emit getAtlantConfigSignal(filename);
}
