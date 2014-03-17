#include "RPCServer.h"

RPCServer::RPCServer(QObject* parent) :
	RpcServerBase(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(RPCServer)), parent)
{
}

RPCServer::~RPCServer()
{
}

bool RPCServer::start(quint16 port, QHostAddress address)
{
	connect(m_serverPeer, SIGNAL(clientConnected(quint64)), this, SLOT(slotRPCConnetion(quint64)));
	connect(m_serverPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(slotErrorRPCConnection(QAbstractSocket::SocketError)));
	connect(m_serverPeer, SIGNAL(clientDisconnected(quint64)), this, SLOT(slotRPCDisconnected(quint64)));

	m_serverPeer->attachSlot(RPC_SLOT_SET_MAIN_STATION_COR, this, SLOT(rpcSlotSetMainStationCor(quint64,int,int)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_BANDWIDTH, this, SLOT(rpcSlotSetBandwidth(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_SHIFT, this, SLOT(rpcSlotSetShift(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_SLOT_RECOGNIZE, this, SLOT(rpcSlotRecognize(quint64, int, int)));
	m_serverPeer->attachSlot(RPC_SLOT_SS_CORRELATION, this, SLOT(rpcSlotSsCorrelation(quint64,int,bool)));
	m_serverPeer->attachSlot(RPC_SLOT_AVARAGE_SPECTRUM, this, SLOT(rpcSlotSetAvarageSpectrum(quint64,int,int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_FREQ, this, SLOT(rpcSlotPrmSetFreq(quint64, QString, short)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_REQUEST_FREQ, this, SLOT(rpcSlotPrmRequestFreq(quint64,QString)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_ATT1, this, SLOT(rpcSlotPrmSetAtt1(quint64, QString, int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_ATT2, this, SLOT(rpcSlotPrmSetAtt2(quint64, QString, int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_FILTER, this, SLOT(rpcSlotPrmSetFilter(quint64, QString, int)));

	m_serverPeer->attachSlot(RPC_SLOT_SET_ATLANT_FREQUENCY, this, SLOT(rpcSlotSetAtlantFrequency(quint64,QByteArray)));

	m_serverPeer->attachSlot(RPC_SLOT_SET_DATA_TO_SOLVER, this, SLOT(rpcSlotSetDataToSolver(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_CLEAR_TO_SOLVER, this, SLOT(rpcSlotSetClearToSolver(quint64,QByteArray)));

	m_serverPeer->attachSlot(RPC_SLOT_REQUEST_STATUS, this, SLOT(rpcSlotRequestStatus(quint64, int)));

	m_serverPeer->attachSignal(this, SIGNAL(serverSendPointsRpcSignal(QByteArray)), RPC_SLOT_SERVER_SEND_POINTS);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendDetectedBandwidthRpcSignal(QByteArray)), RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendCorrelationRpcSignal(uint, uint, QByteArray)), RPC_SLOT_SERVER_SEND_CORRELATION);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendAtlantDirectionRpcSignal(QByteArray)), RPC_SLOT_SERVER_ATLANT_DIRECTION);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendAtlantPositionRpcSignal(QByteArray)), RPC_SLOT_SERVER_ATLANT_POSITION);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendPrmStatusRpcSignal(int,int,int,int)), RPC_SLOT_SERVER_PRM_STATUS);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendPrmStatusRpcSignalBool(QByteArray)), RPC_SLOT_SERVER_STATUS);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendBplaDefRpcSignal(QByteArray)), RPC_SLOT_SERVER_SEND_BPLA_DEF);
	m_serverPeer->attachSignal(this, SIGNAL(serverSendBplaDefAutoRpcSignal(QByteArray)), RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO);

	return RpcServerBase::start(port, address);
}

/// slot if have some error while connetiting
void RPCServer::slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
{
	m_logger->debug("Have error in connection");

	QString thiserror;
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
			thiserror.append(("Ошибка! Соеденение с пунктом потеряно!"));
			return;
			break;
		case QAbstractSocket::HostNotFoundError:
			thiserror.append(("Ошибка! Не удалось подключиться к пункту!"));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			thiserror.append(("Ошибка! Отказано в соединении"));
			break;
		default:
			//			thiserror.append(("Ошибка! Произошла ошибка: " + _rpc_client->->errorString()));
			break;
	}
	m_logger->debug(thiserror);
}

void RPCServer::slotRPCConnetion(quint64 client)
{
	m_logger->debug(QString("Client %1 connected").arg(client));
}

void RPCServer::slotRPCDisconnected(quint64 client)
{
}

void RPCServer::rpcSlotSetMainStationCor(quint64 client, int id, int station)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << station;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_MAIN_STATION_COR, QVariant(byteArray));
	}
}

void RPCServer::sendDataByRpc(const QString& signalType, const QString& deviceName, const QByteArray& data)
{

	/// TODO correct it by deviceName

	RpcMessageStruct message;
	message.name = deviceName;
	message.data = data;

	QByteArray baMessage;
	QDataStream dataStream(&baMessage, QIODevice::WriteOnly);
	dataStream << message;

	if (signalType == RPC_SLOT_SERVER_SEND_POINTS) {
		emit serverSendPointsRpcSignal(baMessage);
	}
	else if (signalType == RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH) {
		emit serverSendDetectedBandwidthRpcSignal(baMessage);
	}
	else if (signalType == RPC_SLOT_SERVER_SEND_CORRELATION) {
		QByteArray inputData = data;
		QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);
		quint32 point1, point2;
		QVector<QPointF> points;
		inputDataStream >> point1 >> point2 >> points;

		QByteArray outputData;
		QDataStream outputDataStream(&outputData, QIODevice::WriteOnly);
		outputDataStream << points;

		emit serverSendCorrelationRpcSignal(point1, point2, outputData);
	}
	else if (signalType == RPC_SLOT_SERVER_ATLANT_DIRECTION) {
		emit serverSendAtlantDirectionRpcSignal(baMessage);
	}
	else if (signalType == RPC_SLOT_SERVER_ATLANT_POSITION) {
		/// TODO: no connection in view
		emit serverSendAtlantPositionRpcSignal(baMessage);
	}
	else if (signalType == RPC_SLOT_SERVER_PRM_STATUS) {
		/// WTF?! TODO: refactor
		QByteArray inputData = data;
		QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

		quint16 freq;
		quint8 filter;
		quint8 att1;
		quint8 att2;

		inputDataStream >> freq;
		inputDataStream >> filter;
		inputDataStream >> att1;
		inputDataStream >> att2;

		emit serverSendPrmStatusRpcSignal((int)freq, (int)filter, (int)att1, (int)att2);
	}
	else if (signalType == RPC_SLOT_SERVER_STATUS) {

		/// TODO change signal and parse qbytearray
		QByteArray inputData = data;
		QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);
		bool status = false;
		inputDataStream >> status;
		emit serverSendPrmStatusRpcSignalBool(baMessage);
//		emit serverSendPrmStatusRpcSignal((int)freq, (int)filter, (int)att1, (int)att2);
	}


	/// TODO: Not used in UI
	else if (signalType == RPC_SLOT_SERVER_SEND_BPLA_DEF) {
		emit serverSendBplaDefRpcSignal(baMessage);
	}
	/// TODO: Not used in UI
	else if (signalType == RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO) {
		emit serverSendBplaDefAutoRpcSignal(baMessage);
	}
}

void RPCServer::rpcSlotSetBandwidth(quint64 client, int id, float bandwidth)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << bandwidth;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_BANDWIDTH, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotSetShift(quint64 client, int id, float shift)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << shift;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_SHIFT, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotRecognize(quint64 client, int id, int type)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << id;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_RECOGNIZE, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotSsCorrelation(quint64 client, int id, bool enable)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << enable;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SS_CORRELATION, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotSetAvarageSpectrum(quint64 client, int id, int avarage)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << avarage;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_AVARAGE_SPECTRUM, QVariant(byteArray));
	}
}

/// send command to prm300 for set central freq
void RPCServer::rpcSlotPrmSetFreq(quint64, QString name, short freq)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << freq;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_PRM_SET_FREQ, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotPrmRequestFreq(quint64 client, QString name)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;

//	QVariant var(byteArray);


//	QDataStream ds(&var.toByteArray(), QIODevice::ReadOnly);
//	QString name1;
//	ds >> name1;

//	m_logger->debug(QString("StName = %1").arg(name1));

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_PRM_REQUEST_FREQ, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotPrmSetAtt1(quint64 client, QString name, int value)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << value;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_PRM_SET_ATT1, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotPrmSetAtt2(quint64 client, QString name, int value)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << value;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_PRM_SET_ATT2, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotPrmSetFilter(quint64 client, QString name, int index)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << name;
	dataStream << index;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_PRM_SET_FILTER, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotSetDataToSolver(quint64 client, QByteArray data)
{
	QByteArray byteArray;
	byteArray.append(data);

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_DATA_TO_SOLVER, QVariant(byteArray));
	}
}

void RPCServer::rpcSlotSetClearToSolver(quint64 client, QByteArray data)
{
	QByteArray byteArray;
	byteArray.append(data);

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_CLEAR_TO_SOLVER, QVariant(byteArray));
	}
}


void RPCServer::rpcSlotSendRespModulation(quint64 client, QString modulation)
{
	/// TODO
	//	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, modulation);
}

void RPCServer::rpcSlotSetAtlantFrequency(quint64 client, QByteArray data)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SET_ATLANT_FREQUENCY, QVariant(data));
	}
}

void RPCServer::rpcSlotRequestStatus(quint64 client, int id)
{
	/// TODO
}
