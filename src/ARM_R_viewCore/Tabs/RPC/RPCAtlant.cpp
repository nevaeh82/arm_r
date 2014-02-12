#include "RPCAtlant.h"

RPCAtlant::RPCAtlant(int id, ITabAtlant* parent_tab)
{
	m_rpcClient = NULL;

	m_id = id;
	m_parentTab = parent_tab;
	connect(this, SIGNAL(signalStart()), this, SLOT(slotStart()));
	connect(this, SIGNAL(signalStop()), this, SLOT(slotStop()));
	connect(this, SIGNAL(signalFinishRPC()), this, SLOT(slotClose()));
}

RPCAtlant::~RPCAtlant()
{
}

void RPCAtlant::slotInit()
{
	QString tabs_setting_file = QCoreApplication::applicationDirPath();
	tabs_setting_file.append("./Tabs/RPC.ini");

	if(!readSettings(tabs_setting_file)){
		return;
	}

	m_rpcClient = new QxtRPCPeer();
	connect(m_rpcClient, SIGNAL(connectedToServer()), this, SLOT(slotRCPConnetion()));
	connect(m_rpcClient, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(slotErrorRPCConnection(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(signalReconnection()), this, SLOT(slotReconnection()));
	connect(m_rpcClient, SIGNAL(disconnectedFromServer()), this, SLOT(slotRPCDisconnection()));

	connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(slotSetCommand(IMessage*)));

	m_rpcClient->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_rpcClient->attachSignal(this, SIGNAL(signalSetFreq(QByteArray)), RPC_SLOT_SET_ATLANT_FREQUENCY);
}

int RPCAtlant::slotStart()
{
	m_rpcClient->connect(m_ipRpc, m_portRpc);
	return 0;
}

int RPCAtlant::slotStop()
{
	m_rpcClient->disconnectServer();
	return 0;
}

void RPCAtlant::slotClose()
{
	if(m_rpcClient->isClient())
	{
		m_rpcClient->disconnectServer();
		delete m_rpcClient;
		m_rpcClient = NULL;
	}
	emit signalFinished();
}

void RPCAtlant::set_command(IMessage *msg)
{
	emit signalSetCommand(msg);
}

void RPCAtlant::slotSetCommand(IMessage *msg)
{
	m_commandMsg = msg;
	formCommand(m_commandMsg);
}

void RPCAtlant::formCommand(IMessage *msg)
{
	QVariant data;
	int type = msg->get(data);
	switch(type)
	{
		case COMMAND_ATLANT_SET_FREQ:
			sendFreq(data);
			break;
		default:
			break;
	}
	msg->clenup();
}

void RPCAtlant::sendFreq(QVariant data)
{
	QByteArray ba  = data.toByteArray();
	emit signalSetFreq(ba);
}

/// slot when connection complete
void RPCAtlant::slotRCPConnetion()
{
	emit signalSetClientId(m_id);
	m_rpcClient->attachSlot(RPC_SLOT_SERVER_ATLANT_DIRECTION, this, SLOT(rpcSlotServerAtlantDirection(QByteArray)));
}

void RPCAtlant::slotRPCDisconnection()
{
	emit signalReconnection();
}

/// slot if have some error while connetiting
void RPCAtlant::slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
{
	QString thiserror;
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
			thiserror.append(("Ошибка! Соеденение с пунктом потеряно!"));
			//return;
			break;
		case QAbstractSocket::HostNotFoundError:
			thiserror.append(("Ошибка! Не удалось подключиться к пункту!"));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			thiserror.append(("Ошибка! Отказано в соединении"));
			break;
		default:
			//        thiserror.append(("Ошибка! Произошла ошибка: " + _rpc_client->->errorString()));
			break;
	}
	
	emit signalReconnection();
}

void RPCAtlant::slotReconnection()
{
	QIODevice *dev = m_rpcClient->takeDevice();
	m_rpcClient->connect(m_ipRpc, m_portRpc);
}

/// read rpc configuration from ini file
bool RPCAtlant::readSettings(const QString & settingsFile)
{
	//QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	QSettings m_settings(settingsFile, QSettings::IniFormat);
	//m_settings.setIniCodec(codec);

	m_ipRpc = m_settings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_portRpc = m_settings.value("RPC_UI/Port", 24500).toInt();

	return true;
}

void RPCAtlant::start()
{
	emit signalStart();
}

void RPCAtlant::stop()
{
	emit signalStop();
}

void RPCAtlant::slotFinish()
{
	emit signalFinishRPC();
}

void RPCAtlant::rpcSlotServerAtlantDirection(QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadWrite);
	A_Dir_Ans_msg msg;
	ds >> msg.requestId;
	ds >> msg.sourceId;
	ds >> msg.dateTime;
	ds >> msg.post;
	ds >> msg.postLatitude;
	ds >> msg.postLongitude;
	ds >> msg.postHeight;
	ds >> msg.frequency;
	ds >> msg.widht;
	ds >> msg.direction;
	ds >> msg.angle;
	ds >> msg.level;
	ds >> msg.quality;
	ds >> msg.motionType;
	ds >> msg.motionConfidence;

	m_parentTab->setLog(data);
}
