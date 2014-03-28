#include "RPCAtlant.h"

RPCAtlant::RPCAtlant(int id, ITabAtlant* parent_tab, QObject *parent) :
	RpcClientBase(parent)
{
	m_id = id;
	m_parentTab = parent_tab;
}

RPCAtlant::~RPCAtlant()
{
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

bool RPCAtlant::start(quint16 port, QHostAddress address)
{
	connect(m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(slotRCPConnetion()));
	connect(m_clientPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(slotErrorRPCConnection(QAbstractSocket::SocketError)));

	connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(slotSetCommand(IMessage*)));

	m_clientPeer->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_clientPeer->attachSignal(this, SIGNAL(signalSetFreq(QByteArray)), RPC_SLOT_SET_ATLANT_FREQUENCY);

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_ATLANT_DIRECTION, this, SLOT(rpcSlotServerAtlantDirection(QByteArray)));

	debug("Start RPCAtlant");
	return RpcClientBase::start(port, address);
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
