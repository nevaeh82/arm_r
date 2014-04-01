#include "RpcPrmClient.h"

#include <QDebug>

RpcPrmClient::RpcPrmClient(IStation *prop, IDbManager *dbManager, IControlPRM*, QObject *parent)
	: RpcRoutedClient( RPC_METHOD_REGISTER_CLIENT, RPC_METHOD_DEREGISTER_CLIENT, parent )
//	, m_controlPrm( controlPrm )
	, m_station( prop )
	, m_dbManager( dbManager )
	, m_spectrum( new float[1] )
	, m_spectrumPeakHold( new float[1] )
	, m_bandwidth( 0 )
	, m_needSetup( true )
{
}

RpcPrmClient::~RpcPrmClient()
{
}

bool RpcPrmClient::start(quint16 port, QHostAddress ipAddress)
{
	connect( m_clientPeer, SIGNAL(connectedToServer()), this, SLOT(slotRpcConnetion()) );
	connect( this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(slotSetCommand(IMessage*)) );

	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetFreq(QString, short)), RPC_METHOD_PRM_SET_FREQUENCY);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMRequestFreq(QString)), RPC_METHOD_PRM_REQUEST_FREQUENCY);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetAtt1(QString, int)), RPC_METHOD_PRM_SET_ATT1);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetAtt2(QString, int)), RPC_METHOD_PRM_SET_ATT2);
	m_clientPeer->attachSignal(this, SIGNAL(signalPRMSetFilter(QString,int)), RPC_METHOD_PRM_SET_FILTER);
	m_clientPeer->attachSignal(this, SIGNAL(signalRequestStatus(QString)), RPC_METHOD_REQUEST_STATUS);

	///server
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, this, SLOT(rpcSlotGettingModulation(QString)));

	m_clientPeer->attachSlot(RPC_SLOT_SERVER_PRM_STATUS, this, SLOT(rpcSlotServerPrmStatus(int, int, int, int)));
	m_clientPeer->attachSlot(RPC_SLOT_SERVER_STATUS, this, SLOT(rpcSlotServerStatus(QByteArray)));

	log_debug("Start RpcPrmClient");
	return RpcClientBase::start(port, ipAddress);
}

void RpcPrmClient::setCommand(IMessage *msg)
{
	emit signalSetCommand(msg);
}

void RpcPrmClient::slotSetCommand(IMessage *msg)
{
	m_commandMsg = msg;
	formCommand(m_commandMsg);
}

void RpcPrmClient::formCommand(IMessage *msg)
{
	QVariant data;
	int type = msg->get(data);
	switch(type)
	{
		case COMMAND_TOBLACKLIST:
			break;
		case COMMAND_TOWHITELIST:
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
		case COMMAND_REQUEST_STATUS:
			requestStatus();
			break;

		default:
			break;
	}
	msg->clenup();
}

void RpcPrmClient::prmSetFreq(short freq)
{
	emit signalPRMSetFreq(m_station->getName(), freq);
}

void RpcPrmClient::prmRequestFreq()
{
	log_debug(QString("Station Name = %1").arg(m_station->getName()));
	emit signalPRMRequestFreq(m_station->getName());
}

void RpcPrmClient::prmSetAtt1(int att1)
{
	emit signalPRMSetAtt1(m_station->getName(), att1);
}

void RpcPrmClient::prmSetAtt2(int att2)
{
	emit signalPRMSetAtt2(m_station->getName(), att2);
}

void RpcPrmClient::prmSetFilter(int index)
{
	emit signalPRMSetFilter(m_station->getName(), index);
}

void RpcPrmClient::requestStatus()
{
	emit signalRequestStatus(m_station->getName());
}

/// slot when connection complete
void RpcPrmClient::slotRpcConnetion()
{
	registerRoute( m_station->getId() + 1 );
	CommandMessage *msg = new CommandMessage(COMMAND_PRM_REQUEST_FREQ, QVariant());
	setCommand(msg);
}

///// slot if have some error while connetiting
//void RpcPrmClient::slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
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

void RpcPrmClient::rpcSlotGettingModulation(QString modulation)
{
	foreach (IRpcListener* listener, m_receiversList) {
		listener->onMethodCalled(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, modulation);
	}

	//m_grData->set_def_modulation(modulation);
}

void RpcPrmClient::rpcSlotServerPrmStatus(int prm_freq, int prm_filter, int prm_att1, int prm_att2)
{

	log_info(QString("PRM Status = %1 %1 %3 %4").arg(prm_freq).arg(prm_filter).arg(prm_att1).arg(prm_att2));
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

void RpcPrmClient::rpcSlotServerStatus(QByteArray message)
{
	RpcMessageStruct messageStruct;

	QDataStream dataStream(&message, QIODevice::ReadOnly);
	dataStream >> messageStruct.name;
	dataStream >> messageStruct.data;

	QDataStream dataStream1(&messageStruct.data, QIODevice::ReadOnly);
	int state;
	dataStream1 >> state;

	log_debug(QString("NAMWS = %1 %2").arg(messageStruct.name).arg(m_station->getName()));

	if( messageStruct.name == m_station->getName() ) {
		foreach( IRpcListener *listener, m_receiversList ) {
			listener->onMethodCalled( RPC_PRM_STATE_CHANGED, QVariant(state) );
		}
	}
}
