#include <Logger.h>
#include <TcpDevicesDefines.h>
#include <Rpc/RpcDefines.h>
#include <Rpc/RpcMessageStruct.h>

#include "Tabs/RPC/RpcFlakonClient.h"

RpcFlakonClient::RpcFlakonClient(QObject *parent) :
	RpcRoutedClient( RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent )
{
	//	connect( m_clientPeer, SIGNAL(connectedToServer()), SLOT(registerRoute()) );

	//	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(pointsReceived(QByteArray)));
	//	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, this, SLOT(bandwidthReceived(QByteArray)));
	//	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(correlationReceived(QByteArray)));

	//	m_clientPeer->attachSignal(this, SIGNAL(signalEnableCorrelation(int,bool)), RPC_METHOD_SS_CORRELATION);
	//	connect(this, SIGNAL(signalEnableCorrelation(int,bool)), this, SLOT(slotEnableCorrelation(int,bool)));
}

bool RpcFlakonClient::start(quint16 port, QHostAddress ipAddress)
{
	connect( m_clientPeer, SIGNAL(connectedToServer()), SLOT(registerRoute()) );

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(pointsReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, this, SLOT(bandwidthReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(correlationReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_FLAKON_STATUS, this, SLOT(flakonStatusReceived(QByteArray)));

	m_clientPeer->attachSignal(this, SIGNAL(signalEnableCorrelation(int,bool)), RPC_METHOD_SS_CORRELATION);

	log_debug("Start RpcPrmClient");
	return RpcClientBase::start(port, ipAddress);
}

void RpcFlakonClient::registerRoute()
{
	bool err = RpcRoutedClient::registerRoute( FLAKON_ROUTE_ID );
	err = m_clientPeer->isClient();
}

void RpcFlakonClient::sendMainStationCorrelation(IStation *station, const QString& value)
{
	m_clientPeer->call( RPC_METHOD_SET_MAIN_STATION_CORRELATION, station->getId(), value );
}

void RpcFlakonClient::sendBandwidth(IStation *station, const float bandwidth)
{
	m_clientPeer->call( RPC_METHOD_SET_BANDWIDTH, station->getId(), bandwidth );
}

void RpcFlakonClient::sendShift(IStation *station, const float shift)
{
	m_clientPeer->call( RPC_METHOD_SET_SHIFT, station->getId(), shift );
}

void RpcFlakonClient::recognize(IStation *station, const int type)
{
	m_clientPeer->call( RPC_METHOD_RECOGNIZE, station->getId(), type );
}

void RpcFlakonClient::sendCorrelation(IStation *station, bool enable)
{
	emit signalEnableCorrelation(station->getId(), enable);
	//	m_clientPeer->call( RPC_METHOD_SS_CORRELATION, station->getId(), enable );
}

void RpcFlakonClient::sendAvarageSpectrum(IStation* station, const int avarage)
{
	m_clientPeer->call( RPC_METHOD_AVARAGE_SPECTRUM, station->getId(), avarage );
}

void RpcFlakonClient::requestFlakonStatus()
{
	m_clientPeer->call( RPC_METHOD_FLAKON_REQUEST_STATUS);
}

void RpcFlakonClient::pointsReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_POINTS, data );
	}
}

void RpcFlakonClient::bandwidthReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, data );
	}
}

void RpcFlakonClient::correlationReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_CORRELATION, data );
	}
}

void RpcFlakonClient::flakonStatusReceived(QByteArray data)
{
	QDataStream dataStream(&data, QIODevice::ReadOnly);
	int state;
	dataStream >> state;

	foreach( IRpcListener *listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_FLAKON_STATUS, QVariant(state) );
	}
}

void RpcFlakonClient::slotEnableCorrelation(int id, bool state)
{
	m_clientPeer->call( RPC_METHOD_SS_CORRELATION, id, state );
}

