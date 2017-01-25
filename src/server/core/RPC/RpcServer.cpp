#include "RPCServer.h"

#include "Tcp/TcpFlakonController.h"

RpcServer::RpcServer(QObject* parent) :
	RpcRoutedServer( parent )
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
	connect(m_serverPeer, SIGNAL(clientConnected(quint64)), this, SLOT(logConnectionSuccess(quint64)));
	connect(m_serverPeer, SIGNAL(clientDisconnected(quint64)), this, SLOT(logClientDisconected(quint64)));

	connect(m_serverPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(logConnectionError(QAbstractSocket::SocketError)));

	m_serverPeer->attachSlot(RPC_METHOD_REGISTER_CLIENT, this, SLOT(registerClient(quint64,uint)) );
	m_serverPeer->attachSlot(RPC_METHOD_DEREGISTER_CLIENT, this, SLOT(deregisterClient(quint64)) );

	m_serverPeer->attachSlot(RPC_METHOD_SET_MAIN_STATION_CORRELATION, this, SLOT(setMainStationCorrelation(quint64,int,QString)));
	m_serverPeer->attachSlot(RPC_METHOD_SET_BANDWIDTH, this, SLOT(setBandwidth(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_METHOD_SET_SHIFT, this, SLOT(setShift(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_METHOD_SET_CENTER, this, SLOT(setCenter(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_METHOD_RECOGNIZE, this, SLOT(recognize(quint64, int, int)));
	m_serverPeer->attachSlot(RPC_METHOD_SS_CORRELATION, this, SLOT(ssCorrelation(quint64,int,float,bool)));

	m_serverPeer->attachSlot(RPC_METHOD_AVARAGE_SPECTRUM, this, SLOT(setAvarageSpectrum(quint64,int,int)));
	m_serverPeer->attachSlot(RPC_METHOD_ENABLE_RECEIVER, this, SLOT(setEnableReceiver(quint64,int,bool)));

	m_serverPeer->attachSlot(RPC_METHOD_SOLVER_SETTINGS_SETUP, this, SLOT(setSolverSettingsSetup(quint64, QByteArray)));
	m_serverPeer->attachSlot(RPC_METHOD_SEND_RDS_PROTO, this, SLOT(setRdsProtoSetup(quint64, QByteArray)));

	m_serverPeer->attachSlot(RPC_METHOD_PRM_SET_FREQUENCY, this, SLOT(setPrmFrequency(quint64, QString, short)));
	m_serverPeer->attachSlot(RPC_METHOD_PRM_REQUEST_FREQUENCY, this, SLOT(requestPrmFrequency(quint64,QString)));
	m_serverPeer->attachSlot(RPC_METHOD_PRM_SET_ATT1, this, SLOT(setPrmAtt1(quint64, QString, QVariant)));
	m_serverPeer->attachSlot(RPC_METHOD_PRM_SET_ATT2, this, SLOT(setPrmAtt2(quint64, QString, int)));
	m_serverPeer->attachSlot(RPC_METHOD_PRM_SET_FILTER, this, SLOT(setPrmFilter(quint64, QString, int)));

	m_serverPeer->attachSlot(RPC_METHOD_SET_DATA_TO_SOLVER, this, SLOT(setDataToSolver(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_METHOD_SET_CLEAR_TO_SOLVER, this, SLOT(setClearToSolver(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_KTR_TO_ARMT, this, SLOT(setKtrData(quint64,QByteArray)));

	m_serverPeer->attachSlot(RPC_METHOD_REQUEST_STATUS, this, SLOT(requestStatus(quint64, QString)));
	m_serverPeer->attachSlot(RPC_METHOD_FLAKON_REQUEST_STATUS, this, SLOT(requestFlakonStatus(quint64)));

	m_serverPeer->attachSlot(RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, this, SLOT(requestGetStationListSlot(quint64,QString)));
	m_serverPeer->attachSlot(RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, this, SLOT(requestGetDbConfigurationSlot(quint64,QString)));
}

RpcServer::~RpcServer()
{
}

/// slot if have some error while connetiting
void RpcServer::logConnectionError(QAbstractSocket::SocketError socketError)
{
	log_debug("Have error in connection");

	QString message;
	switch(socketError) {
		case QAbstractSocket::RemoteHostClosedError:
			message = "Ошибка! Соеденение с пунктом потеряно!";
			return;
			break;
		case QAbstractSocket::HostNotFoundError:
			message = "Ошибка! Не удалось подключиться к пункту!";
			break;
		case QAbstractSocket::ConnectionRefusedError:
			message = "Ошибка! Отказано в соединении";
			break;
		default:
//			message.append(("Ошибка! Произошла ошибка: " + _rpc_client->->errorString()));
			break;
	}

	log_debug(message);
}

void RpcServer::logConnectionSuccess(quint64 client)
{
	log_debug(QString("Client %1 connected").arg(client));
}

void RpcServer::logClientDisconected(quint64 client)
{
	Q_UNUSED( client );
}

void RpcServer::requestGetStationListSlot(quint64, QString configFilename)
{
	dispatch( RPC_METHOD_CONFIG_REQUEST_GET_STATION_LIST, QVariant(configFilename) );
}

void RpcServer::requestGetDbConfigurationSlot(quint64, QString configFilename)
{
	dispatch( RPC_METHOD_CONFIG_REQUEST_GET_DB_CONFIGURATION, QVariant(configFilename) );
}

void RpcServer::setMainStationCorrelation(quint64 client, int id, QString station)
{
	Q_UNUSED( id );

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << station;
	dispatch( RPC_METHOD_SET_MAIN_STATION_CORRELATION, byteArray, client );
}

void RpcServer::setBandwidth(quint64 client, int id, float bandwidth)
{
	Q_UNUSED( id );

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << bandwidth;

	dispatch( RPC_METHOD_SET_BANDWIDTH, QVariant(byteArray), client );
}

void RpcServer::setShift(quint64 client, int id, float shift)
{
	Q_UNUSED( id );

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << shift;

	dispatch( RPC_METHOD_SET_SHIFT, QVariant(byteArray), client );
}

void RpcServer::setCenter(quint64 client, int id, float center)
{
	Q_UNUSED( id );

	dispatch( RPC_METHOD_SET_CENTER, QVariant(center), client );
}

void RpcServer::recognize(quint64 client, int id, int)
{
	Q_UNUSED( id );

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << id;

	dispatch( RPC_METHOD_RECOGNIZE, QVariant(byteArray), client );
}

void RpcServer::ssCorrelation(quint64 client, int id, float frequency, bool enable)
{

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << id;
	dataStream << frequency;
	dataStream << enable;

	dispatch( RPC_METHOD_SS_CORRELATION, QVariant(byteArray), client );
}

void RpcServer::setAvarageSpectrum(quint64 client, int id, int avarage)
{
	Q_UNUSED( id );

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << avarage;

	dispatch( RPC_METHOD_AVARAGE_SPECTRUM, QVariant(byteArray), client );
}

void RpcServer::setEnableReceiver(quint64 client, int id, bool val)
{
	Q_UNUSED( id );

	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << id;
	dataStream << val;

	dispatch( RPC_METHOD_ENABLE_RECEIVER, QVariant(byteArray), client );
}

void RpcServer::setSolverSettingsSetup(quint64 client, QByteArray data) {

	dispatch( RPC_METHOD_SOLVER_SETTINGS_SETUP, QVariant(data), 0 );
}

void RpcServer::setRdsProtoSetup(quint64 client, QByteArray data) {

	dispatch( RPC_METHOD_SEND_RDS_PROTO, QVariant(data), 0 );
}

void RpcServer::setWorkMode(quint64 client, int mode, bool isOn)
{
	QByteArray byteArrayMod;
	QByteArray byteArrayOn;

	QDataStream dataStreamMod(&byteArrayMod, QIODevice::WriteOnly);
	dataStreamMod << mode;

	QDataStream dataStreamOn(&byteArrayOn, QIODevice::WriteOnly);
	dataStreamOn << isOn;

	dispatch( RPC_METHOD_WORK_MODE_M, QVariant(byteArrayMod), client );
	dispatch( RPC_METHOD_WORK_MODE_ON, QVariant(byteArrayOn), client );
}


/// send command to prm300 for set central freq
void RpcServer::setPrmFrequency(quint64 client, QString name, short freq)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << freq;

	dispatch( RPC_METHOD_PRM_SET_FREQUENCY, QVariant(byteArray)/*, client*/ );
}

void RpcServer::requestPrmFrequency(quint64 client, QString name)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;

	dispatch( RPC_METHOD_PRM_REQUEST_FREQUENCY, QVariant(byteArray), client );
}

void RpcServer::setPrmAtt1(quint64 client, QString name, QVariant value)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << value;

	dispatch( RPC_METHOD_PRM_SET_ATT1, value/*, client*/ );
}

void RpcServer::setPrmAtt2(quint64 client, QString name, int value)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << value;

	dispatch( RPC_METHOD_PRM_SET_ATT2, QVariant(byteArray)/*, client*/ );
}

void RpcServer::setPrmFilter(quint64 client, QString name, int index)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << index;

	dispatch( RPC_METHOD_PRM_SET_FILTER, QVariant(byteArray)/*, client*/ );
}

void RpcServer::setDataToSolver(quint64 client, QByteArray data)
{
	QByteArray byteArray;
	byteArray.append(data);

	dispatch( RPC_METHOD_SET_DATA_TO_SOLVER, QVariant(byteArray), client );
}

void RpcServer::setClearToSolver(quint64 client, QByteArray data)
{
	QByteArray byteArray;
	byteArray.append(data);

	dispatch( RPC_METHOD_SET_CLEAR_TO_SOLVER, QVariant(byteArray), client );
}

void RpcServer::setKtrData(quint64 client, QByteArray data)
{
	QByteArray byteArray;
	byteArray.append(data);

	dispatch( RPC_SLOT_SET_KTR_TO_ARMT, QVariant(byteArray), client );
}

void RpcServer::sendResponseModulation(quint64 client, QString modulation)
{
	Q_UNUSED( client );
	Q_UNUSED( modulation );
	/// TODO
	//	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, modulation);
}

void RpcServer::requestStatus(quint64 client, QString name)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;

	dispatch( RPC_METHOD_REQUEST_STATUS, QVariant(byteArray), client );
}

void RpcServer::requestFlakonStatus(quint64 client)
{
	dispatch( RPC_METHOD_FLAKON_REQUEST_STATUS, QVariant(), client );
}

