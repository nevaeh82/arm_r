#include <Logger/Logger.h>
#include <TcpDevicesDefines.h>
#include <Rpc/RpcDefines.h>
#include <Rpc/RpcMessageStruct.h>
#include <QTime>
#include <QThread>

#include "Tabs/RPC/RpcFlakonClient.h"
#include "Info/StationConfiguration.h"
#include "RDSExchange.h"

RpcFlakonClient::RpcFlakonClient(QObject *parent) :
	RpcRoutedClient( RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent )
{

	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SIGNAL(connectionEstablishedSignal()));

	//m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, this, SLOT(receivedStationListSlot(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_CONFIG_RDS_ANSWER, this, SLOT(receivedLocSystem(QByteArray)));
	m_clientPeer->attachSlot(RPC_METHOD_WORK_MODE, this, SLOT(receivedWorkMode(QByteArray)));
	//m_clientPeer->attachSlot(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, this, SLOT(receivedDbConfigurationSlot(QByteArray)));

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
	m_clientPeer->attachSlot(RPC_SLOT_FLAKON_DEV_STATE, this, SLOT(flakonStatusReceived(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_RESULT, this, SLOT(sloverResultReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_BPLA_RESULT_1, this, SLOT(sloverResultReceived1(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1, this, SLOT(sloverAnswerReceived1(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_QUALITY_STATUS, this, SLOT(solverQualityStatusReceived(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_SOLVER_ERRORS, this, SLOT(solverErrorsReceived(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_SOLVER_CONNECT_STATE, this, SLOT(sloverConnectState(bool)));

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
}

void RpcFlakonClient::sendAvarageSpectrum(const int id, const int avarage)
{
	m_clientPeer->call( RPC_METHOD_AVARAGE_SPECTRUM, id, avarage );
}

void RpcFlakonClient::sendEnableReceiver(const int id, const bool val)
{
	m_clientPeer->call( RPC_METHOD_ENABLE_RECEIVER, id, val );
}

void RpcFlakonClient::sendSolverSettings(const QByteArray &data)
{
	m_clientPeer->call( RPC_METHOD_SOLVER_SETTINGS_SETUP, data );
}

void RpcFlakonClient::sendRdsProto(const QByteArray &data)
{
	m_clientPeer->call( RPC_METHOD_SEND_RDS_PROTO, data );
}

void RpcFlakonClient::sendCPPacketProto(const QByteArray &data)
{
    m_clientPeer->call( RPC_METHOD_SEND_CPPACKET_PROTO, data );
}

//void RpcFlakonClient::sendWorkMode(const int mode, const bool isOn )
//{
//	m_clientPeer->call( RPC_METHOD_WORK_MODE, mode, isOn );
//}

void RpcFlakonClient::requestFlakonStatus()
{
	m_clientPeer->call( RPC_METHOD_FLAKON_REQUEST_STATUS);
}

void RpcFlakonClient::pointsReceived(QByteArray data)
{
	//    log_debug(QString("Receive Points >>>>>  %1 >>>> %2").arg(data.size()).arg(m_receiversList.size()));
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
	//	QDataStream dataStream(&data, QIODevice::ReadOnly);

	//	QList<DevState> stateList;

	//	dataStream >> stateList;
	log_debug(QString("flakonStatusReceived >>>>>  %1 >>>> %2").arg(data.size()).arg(m_receiversList.size()));


	foreach( IRpcListener *listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_FLAKON_STATUS, QVariant(data) );
	}
}

void RpcFlakonClient::sloverResultReceived(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_BPLA_RESULT, data );
	}
}

void RpcFlakonClient::sloverResultReceived1(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_BPLA_RESULT_1, data );
	}
}

void RpcFlakonClient::sloverAnswerReceived1(QByteArray data)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1, data );
	}
}

void RpcFlakonClient::sloverConnectState(bool state)
{
	foreach( IRpcListener* listener, m_receiversList ) {
		listener->onMethodCalled( RPC_SLOT_SERVER_SEND_SOLVER_CONNECT_STATE, state );
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

///config
///

void RpcFlakonClient::receivedStationListSlot(QByteArray data)
{

	//log_debug(QString("receivedStationListSlot >>>>>  %1 >>>> %2").arg(data.size()).arg(m_receiversList.size()));

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_STATION_LIST, data);
	}
}

void RpcFlakonClient::receivedLocSystem(QByteArray data)
{
	m_receiverMutex.lock();

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_RDS_ANSWER, data);
	}
	m_receiverMutex.unlock();
}

void RpcFlakonClient::receivedWorkMode(QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	int val;
	ds >> val;
	return;
}

void RpcFlakonClient::receivedDbConfigurationSlot(QByteArray data)
{
	//log_debug(QString("receivedDbConfigurationSlot >>>>>  %1 >>>> %2").arg(data.size()).arg(m_receiversList.size()));

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION, data);
	}
}

void RpcFlakonClient::requestGetStationList(const QString& filename)
{
	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, filename);
}

void RpcFlakonClient::requestGetDbConfiguration(const QString& filename)
{
	m_clientPeer->call(RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, filename);
}

