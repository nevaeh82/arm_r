#include <Logger.h>
#include <TcpDevicesDefines.h>
#include <Rpc/RpcDefines.h>
#include <Rpc/RpcMessageStruct.h>

#include "Tabs/RPC/RpcFlakonClient.h"

RpcFlakonClient::RpcFlakonClient(QObject *parent) :
	RpcRoutedClient( RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent )
{
	connect( m_clientPeer, SIGNAL(connectedToServer()), SLOT(registerRoute()) );

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(pointsReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, this, SLOT(bandwidthReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(correlationReceived(uint, uint, QByteArray)));
}

void RpcFlakonClient::registerRoute()
{
	RpcRoutedClient::registerRoute( FLAKON_ROUTE_ID );
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
	m_clientPeer->call( RPC_METHOD_SS_CORRELATION, station->getId(), enable );
}

void RpcFlakonClient::sendAvarageSpectrum(IStation* station, const int avarage)
{
	m_clientPeer->call( RPC_METHOD_AVARAGE_SPECTRUM, station->getId(), avarage );
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

void RpcFlakonClient::correlationReceived(uint point1, uint point2, QByteArray points)
{
	QList<QVariant> list;
	QVariant pointsVariant( points );
	QVariant point1Variant( point1 );
	QVariant point2Variant( point2 );

	list.append( pointsVariant );
	list.append( point1Variant );
	list.append( point2Variant );

	QVariant data( list );

	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_CORRELATION, data );
	}
}

