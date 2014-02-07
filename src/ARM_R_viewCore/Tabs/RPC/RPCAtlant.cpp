#include "RPCAtlant.h"

RPCAtlant::RPCAtlant(int id, ITabAtlant* parent_tab)
{
	_rpc_client = NULL;

	_id = id;
	_parent_tab = parent_tab;
	connect(this, SIGNAL(signalStart()), this, SLOT(start()));
	connect(this, SIGNAL(signalStop()), this, SLOT(stop()));
	connect(this, SIGNAL(signalFinishRPC()), this, SLOT(_close()));
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

	_rpc_client = new QxtRPCPeer();
	connect(_rpc_client, SIGNAL(connectedToServer()), this, SLOT(_slotRCPConnetion()));
	connect(_rpc_client, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(signalReconnection()), this, SLOT(_slotReconnection()));
	connect(_rpc_client, SIGNAL(disconnectedFromServer()), this, SLOT(_slotRPCDisconnection()));

	connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(_slotSetCommand(IMessage*)));

	_rpc_client->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	_rpc_client->attachSignal(this, SIGNAL(signalSetFreq(QByteArray)), RPC_SLOT_SET_ATLANT_FREQUENCY);
	//    _rpc_client->attachSignal(this, SIGNAL(signalSetBandwidth(int, float)), RPC_SLOT_SET_BANDWIDTH);
	//    _rpc_client->attachSignal(this, SIGNAL(signalSetShift(int, float)), RPC_SLOT_SET_SHIFT);
	//    _rpc_client->attachSignal(this, SIGNAL(signalRecognize(int, int)), RPC_SLOT_RECOGNIZE);
	//    _rpc_client->attachSignal(this, SIGNAL(signalSSCorrelation(int, bool)), RPC_SLOT_SS_CORRELATION);

	//    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetFreq(int, short)), RPC_SLOT_PRM_SET_FREQ);
	//    _rpc_client->attachSignal(this, SIGNAL(signalPRMRequestFreq(int)), RPC_SLOT_PRM_REQUEST_FREQ);
	//    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetAtt1(int, int)), RPC_SLOT_PRM_SET_ATT1);
	//    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetAtt2(int, int)), RPC_SLOT_PRM_SET_ATT2);
	//    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetFilter(int,int)), RPC_SLOT_PRM_SET_FILTER);



	//    return 0;
}

int RPCAtlant::start()
{
	_rpc_client->connect(_ip_RPC, _port_RPC);
	return 0;
}

int RPCAtlant::stop()
{
	_rpc_client->disconnectServer();
	return 0;
}

void RPCAtlant::_close()
{
	if(_rpc_client->isClient())
	{
		_rpc_client->disconnectServer();
		delete _rpc_client;
		_rpc_client = NULL;
	}
	emit signalFinished();
}

void RPCAtlant::set_command(IMessage *msg)
{
	emit signalSetCommand(msg);
}

void RPCAtlant::_slotSetCommand(IMessage *msg)
{
	_command_msg = msg;
	_form_command(_command_msg);
}

void RPCAtlant::_form_command(IMessage *msg)
{
	QVariant data;
	int type = msg->get(data);
	switch(type)
	{
		case COMMAND_ATLANT_SET_FREQ:
			_send_freq(data);
			break;
			//    case COMMAND_TOBLACKLIST:
			//        break;
			//    case COMMAND_TOWHITELIST:
			//        break;
			//    case COMMAND_RECOGNIZESIGNAL:
			//        _recognize();
			//        break;
			//    case COMMAND_KM:
			//        _ss_correlation(data.toBool());
			//        break;
			//    case COMMAND_PRM_SET_FREQ:
			//        _prm_set_freq(data.toUInt());
			//        break;
			//    case COMMAND_PRM_REQUEST_FREQ:
			//        _prm_request_freq();
			//        break;
			//    case COMMAND_PRM_SET_ATT1:
			//        _prm_set_att1(data.toInt());
			//        break;
			//    case COMMAND_PRM_SET_ATT2:
			//        _prm_set_att2(data.toInt());
			//        break;
			//    case COMMAND_PRM_SET_FILTER:
			//        _prm_set_filter(data.toInt());
			//        break;
		default:
			break;
	}
	msg->clenup();
}

void RPCAtlant::_send_freq(QVariant data)
{
	QByteArray ba  = data.toByteArray();
	emit signalSetFreq(ba);
}

/// slot when connection complete
void RPCAtlant::_slotRCPConnetion()
{
	emit signalSetClientId(_id);
	///server
	_rpc_client->attachSlot(RPC_SLOT_SERVER_ATLANT_DIRECTION, this, SLOT(rpc_slot_server_atlant_direction(QByteArray)));
	//    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, this, SLOT(rpc_slot_getting_modulation(QString)));
	//    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(rpc_slot_server_send_correlation(int, int, rpc_send_points_vector)));

	//    _rpc_client->attachSlot(RPC_SLOT_SERVER_PRM_STATUS, this, SLOT(rpc_slot_server_prm_status(int, int, int, int)));


	//    CommandMessage *msg = new CommandMessage(COMMAND_PRM_REQUEST_FREQ, QVariant());
	//    this->set_command(msg);
}

void RPCAtlant::_slotRPCDisconnection()
{
	emit signalReconnection();
}

/// slot if have some error while connetiting
void RPCAtlant::_slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
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

void RPCAtlant::_slotReconnection()
{
	QIODevice *dev = _rpc_client->takeDevice();
	_rpc_client->connect(_ip_RPC, _port_RPC);
}

/// read rpc configuration from ini file
bool RPCAtlant::readSettings(const QString & settingsFile)
{
	//QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
	QSettings m_settings(settingsFile, QSettings::IniFormat);

	//m_settings.setIniCodec(codec);

	_ip_RPC = m_settings.value("RPC_UI/IP", "127.0.0.1").toString();
	_port_RPC = m_settings.value("RPC_UI/Port", 24500).toInt();

	return true;
}

void RPCAtlant::slotStart()
{
	emit signalStart();
}

void RPCAtlant::slotStop()
{
	emit signalStop();
}

void RPCAtlant::slotFinish()
{
	emit signalFinishRPC();
}

void RPCAtlant::rpc_slot_server_atlant_direction(QByteArray data)
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

	_parent_tab->setLog(data);
}
