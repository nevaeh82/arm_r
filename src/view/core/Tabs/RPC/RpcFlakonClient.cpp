#include <Logger/Logger.h>
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

	bool n = m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(pointsReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, this, SLOT(bandwidthReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(correlationReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_FLAKON_STATUS, this, SLOT(flakonStatusReceived(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_RESULT, this, SLOT(sloverResultReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_QUALITY_STATUS, this, SLOT(solverQualityStatusReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_SOLVER_ERRORS, this, SLOT(solverErrorsReceived(QByteArray)));

	m_clientPeer->attachSignal(this, SIGNAL(signalEnableCorrelation(int,float,bool)), RPC_METHOD_SS_CORRELATION);


	log_debug("Start RpcPrmClient");
	return RpcClientBase::start(port, ipAddress);
}

void RpcFlakonClient::registerRoute()
{
	//bool err = RpcRoutedClient::registerRoute( FLAKON_ROUTE_ID );
	bool err = RpcRoutedClient::registerRoute( RDS_ROUTE_ID );
	err = m_clientPeer->isClient();
}

void RpcFlakonClient::sendMainStationCorrelation(const int id, const QString& value)
{
	m_clientPeer->call( RPC_METHOD_SET_MAIN_STATION_CORRELATION, id, value );
}

void RpcFlakonClient::sendBandwidth(const int id, const float bandwidth)
{
	m_clientPeer->call( RPC_METHOD_SET_BANDWIDTH, id, bandwidth );
}

void RpcFlakonClient::sendShift(const int id, const float shift)
{
	m_clientPeer->call( RPC_METHOD_SET_SHIFT, id, shift );
}

void RpcFlakonClient::sendCenter(const int id, const float center)
{
	m_clientPeer->call( RPC_METHOD_SET_CENTER, id, center );
}

void RpcFlakonClient::recognize(const int id, const int type)
{
	m_clientPeer->call( RPC_METHOD_RECOGNIZE, id, type );
}

void RpcFlakonClient::sendCorrelation(const int id, const float frequency, bool enable)
{
	emit signalEnableCorrelation(id, frequency, enable);
	//	m_clientPeer->call( RPC_METHOD_SS_CORRELATION, station->getId(), enable );
}

void RpcFlakonClient::sendAvarageSpectrum(const int id, const int avarage)
{
	m_clientPeer->call( RPC_METHOD_AVARAGE_SPECTRUM, id, avarage );
}

void RpcFlakonClient::requestFlakonStatus()
{
	m_clientPeer->call( RPC_METHOD_FLAKON_REQUEST_STATUS);
}

void RpcFlakonClient::pointsReceived(QByteArray data)
{
	log_debug(QString("Receive Points >>>>>  %1").arg(data.size()));
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_POINTS, data );
	}
}

void RpcFlakonClient::bandwidthReceived(QByteArray data)
{
	//log_debug("Receive Bandwidth >>>>>");
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, data );
	}
}

void RpcFlakonClient::correlationReceived(QByteArray data)
{
	//log_debug(QString("Receive Correlations >>>>>  %1").arg(data.size()));
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_CORRELATION, data );
	}
}

void RpcFlakonClient::clearReceiversList()
{
	foreach( IRpcListener *listener, m_receiversList ) {
		deregisterReceiver(listener);
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

void RpcFlakonClient::sloverResultReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_BPLA_RESULT, data );
	}
}

void RpcFlakonClient::solverQualityStatusReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_QUALITY_STATUS, data );
	}
}

void RpcFlakonClient::solverErrorsReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_SOLVER_ERRORS, data );
	}
}

void RpcFlakonClient::slotEnableCorrelation(int id, float frequency, bool state)
{
	m_clientPeer->call( RPC_METHOD_SS_CORRELATION, id, frequency,state );
}

