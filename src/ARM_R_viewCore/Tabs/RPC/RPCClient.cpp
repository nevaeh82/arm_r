#include "RPCClient.h"

#include <QDebug>

RPCClient::RPCClient(IStation *prop, IDbManager *db_manager,
					 ITabSpectrum* parent_tab,
					 IControlPRM* control_prm, QObject *parent) :
	RpcClientBase(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(RPCClient)), parent)
{
//	m_controlPrm = control_prm;
	m_parentTab = parent_tab;
	m_station = prop;
	m_dbManager = db_manager;

	m_spectrum = new float[1];
	m_spectrumPeakHold = new float[1];
	m_bandwidth = 0;
	m_needSetup = true;
}

RPCClient::~RPCClient()
{
}

bool RPCClient::start(quint16 port, QHostAddress ipAddress)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(slotRpcConnetion()));

	connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(slotSetCommand(IMessage*)));

	m_clientPeer->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetMainStationCor(int,QString)), RPC_SLOT_SET_MAIN_STATION_COR);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetBandwidth(int, float)), RPC_SLOT_SET_BANDWIDTH);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetShift(int, float)), RPC_SLOT_SET_SHIFT);
	m_clientPeer->attachSignal(this, SIGNAL(signalRecognize(int, int)), RPC_SLOT_RECOGNIZE);
	m_clientPeer->attachSignal(this, SIGNAL(signalSSCorrelation(int, bool)), RPC_SLOT_SS_CORRELATION);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetAvarageSpectrum(int,int)), RPC_SLOT_AVARAGE_SPECTRUM);

	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetFreq(QString, short)), RPC_SLOT_PRM_SET_FREQ);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMRequestFreq(QString)), RPC_SLOT_PRM_REQUEST_FREQ);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetAtt1(QString, int)), RPC_SLOT_PRM_SET_ATT1);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetAtt2(QString, int)), RPC_SLOT_PRM_SET_ATT2);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetFilter(QString,int)), RPC_SLOT_PRM_SET_FILTER);
	m_clientPeer->attachSignal(this, SIGNAL(signalRequestStatus(QString)), RPC_SLOT_REQUEST_STATUS);

	///server
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(rpcSlotGettingPoints(QByteArray)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, this, SLOT(rpcSlotGettingDetectedBandwidth(QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, this, SLOT(rpcSlotGettingModulation(QString)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(rpcSlotServerSendCorrelation(uint, uint, QByteArray)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_PRM_STATUS, this, SLOT(rpcSlotServerPrmStatus(int, int, int, int)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_STATUS, this, SLOT(rpcSlotServerStatus(QByteArray)));

	m_logger->debug("Start RPCClient");
	return RpcClientBase::start(port, ipAddress);
}

void RPCClient::setCommand(IMessage *msg)
{
	emit signalSetCommand(msg);
}

void RPCClient::slotSetCommand(IMessage *msg)
{
	m_commandMsg = msg;
	formCommand(m_commandMsg);
}

void RPCClient::formCommand(IMessage *msg)
{
	QVariant data;
	int type = msg->get(data);
	switch(type)
	{
		case COMMAND_TOBLACKLIST:
			break;
		case COMMAND_TOWHITELIST:
			break;
		case COMMAND_RECOGNIZESIGNAL:
			recognize();
			break;
		case COMMAND_KM:
			ssCorrelation(data.toBool());
			break;
		case COMMAND_FLAKON_SET_MAIN_STATION_COR:
			flakonSetMainStationCor(data.toString());
			break;
		case COMMAND_PRM_SET_FREQ:
			prmSetFreq(data.toUInt());
			break;
		case COMMAND_PRM_REQUEST_FREQ:
			prmRequestFreq();
			break;
		case COMMAND_PRM_SET_ATT1:
			prmSetAtt1(data.toInt());
			break;
		case COMMAND_PRM_SET_ATT2:
			prmSetAtt2(data.toInt());
			break;
		case COMMAND_PRM_SET_FILTER:
			prmSetFilter(data.toInt());
			break;
		case COMMAND_FLAKON_SET_AVARAGE:
			flakonSetAvarage(data.toInt());
			break;
		case COMMAND_REQUEST_STATUS:
			requestStatus();
			break;

		default:
			break;
	}
	msg->clenup();
}

void RPCClient::prmSetFreq(short freq)
{
	emit signalPRMSetFreq(m_station->getName(), freq);
}

void RPCClient::prmRequestFreq()
{
	m_logger->debug(QString("Station Name = %1").arg(m_station->getName()));
	emit signalPRMRequestFreq(m_station->getName());
}

void RPCClient::prmSetAtt1(int att1)
{
	emit signalPRMSetAtt1(m_station->getName(), att1);
}

void RPCClient::prmSetAtt2(int att2)
{
	emit signalPRMSetAtt2(m_station->getName(), att2);
}

void RPCClient::prmSetFilter(int index)
{
	emit signalPRMSetFilter(m_station->getName(), index);
}

void RPCClient::flakonSetMainStationCor(QString value)
{
	emit signalSetMainStationCor(m_station->getId(), value);
}

void RPCClient::flakonSetAvarage(int value)
{
	emit signalSetAvarageSpectrum(m_station->getId(), value);
}

void RPCClient::requestStatus()
{
	emit signalRequestStatus(m_station->getName());
}

void RPCClient::recognize()
{
	float bandwidth = 0;
	float shift = 0;

	QVariant value = m_dbManager->getPropertyValue(m_station->getName(), DB_SELECTED_PROPERTY);

	if (value.isValid()) {
		bandwidth = value.toFloat();
	}

	value = m_dbManager->getPropertyValue(m_station->getName(), DB_CENTER_PROPERTY);

	if (value.isValid()) {
		shift = value.toFloat();
	}

	emit signalSetBandwidth(m_station->getId(), bandwidth);
	emit signalSetShift(m_station->getId(), shift);
	int s_type = 104;
	emit signalRecognize(m_station->getId(), s_type);
}

void RPCClient::ssCorrelation(bool enable)
{
	//    int type = 103;
	emit signalSSCorrelation(m_station->getId(), enable);
}

/// slot when connection complete
void RPCClient::slotRpcConnetion()
{
	emit signalSetClientId(m_station->getId());
	CommandMessage *msg = new CommandMessage(COMMAND_PRM_REQUEST_FREQ, QVariant());
	setCommand(msg);
}

///// slot if have some error while connetiting
//void RPCClient::slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
//{
//	QString thiserror;
//	switch(socketError)
//	{
//		case QAbstractSocket::RemoteHostClosedError:
//			thiserror.append(("Ошибка! Соеденение с пунктом потеряно!"));
//			break;
//		case QAbstractSocket::HostNotFoundError:
//			thiserror.append(("Ошибка! Не удалось подключиться к пункту!"));
//			break;
//		case QAbstractSocket::ConnectionRefusedError:
//			thiserror.append(("Ошибка! Отказано в соединении"));
//			break;
//		default:
//			//        thiserror.append(("Ошибка! Произошла ошибка: " + _rpc_client->->errorString()));
//			break;
//	}
//}

/// getting points from server
void RPCClient::rpcSlotGettingPoints(QByteArray points)
{
	RpcMessageStruct messageStruct;

	QDataStream dataStream(&points, QIODevice::ReadOnly);
	dataStream >> messageStruct.name;
	dataStream >> messageStruct.data;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SERVER_SEND_POINTS, messageStruct.data);
	}

	//m_grData->set_data(points, true);
}

void RPCClient::rpcSlotGettingDetectedBandwidth(QByteArray points)
{
	RpcMessageStruct messageStruct;

	QDataStream dataStream(&points, QIODevice::ReadOnly);
	dataStream >> messageStruct.name;
	dataStream >> messageStruct.data;

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, messageStruct.data);
	}

	//m_grData->setDetectedAreas(points);
}

void RPCClient::rpcSlotGettingModulation(QString modulation)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, modulation);
	}

	//m_grData->set_def_modulation(modulation);
}

void RPCClient::rpcSlotServerSendCorrelation(uint point1, uint point2, QByteArray points)
{
	QList<QVariant> list;
	QVariant pointsVariant(points);
	QVariant point1Variant(point1);
	QVariant point2Variant(point2);

	list.append(pointsVariant);
	list.append(point1Variant);
	list.append(point2Variant);

	QVariant data(list);

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SERVER_SEND_CORRELATION, data);
	}

	//	if(point2 != m_tabProperty->get_id())
	//		m_grData->set_data(point2, points, true);
}

void RPCClient::rpcSlotServerPrmStatus(int prm_freq, int prm_filter, int prm_att1, int prm_att2)
{

	m_logger->info(QString("PRM Status = %1 %1 %3 %4").arg(prm_freq).arg(prm_filter).arg(prm_att1).arg(prm_att2));
	QList<QVariant> list;

	list.append(QVariant(prm_freq));
	list.append(QVariant(prm_filter));
	list.append(QVariant(prm_att1));
	list.append(QVariant(prm_att2));

	QVariant data(list);

	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SERVER_PRM_STATUS, data);
	}

	//	m_parentTab->setIndicator(true);

	///TODO: update

	/*QMap<QString, QVariant>* data = _db_manager->get(0, _tab_property->get_id());
	data->insert("value", prm_freq);
	_db_manager->set(data);

	QMap<int, QVariant> status;
	status.insert(0, QVariant::fromValue(prm_freq));
	status.insert(1, QVariant::fromValue(prm_filter));
	status.insert(2, QVariant::fromValue(prm_att1));
	status.insert(3, QVariant::fromValue(prm_att2));

	_control_prm->set_status(status);*/

}

void RPCClient::rpcSlotServerStatus(QByteArray message)
{
	RpcMessageStruct messageStruct;

	QDataStream dataStream(&message, QIODevice::ReadOnly);
	dataStream >> messageStruct.name;
	dataStream >> messageStruct.data;

	QDataStream dataStream1(&messageStruct.data, QIODevice::ReadOnly);
	int state;
	dataStream1 >> state;

	m_logger->debug(QString("NAMWS = %1 %2").arg(messageStruct.name).arg(m_station->getName()));

	if(messageStruct.name == m_station->getName())
	{
		m_parentTab->setIndicator(state);
	}
}
