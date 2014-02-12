#include "RPCClient.h"

#include <QDebug>

RPCClient::RPCClient(TabsProperty *prop, IDBManager *db_manager, ITabSpectrum* parent_tab, GraphicData* gr_data, IControlPRM* control_prm)
{

	m_controlPrm = control_prm;
	m_parentTab = parent_tab;
	m_tabProperty = prop;
	m_dbManager = db_manager;
	//    _rpc_client->attachSlot()

	connect(this, SIGNAL(signalStart()), this, SLOT(start()));
	connect(this, SIGNAL(signalStop()), this, SLOT(stop()));
	connect(this, SIGNAL(signalFinishRPC()), this, SLOT(close()));

	m_spectrum = new float[1];
	m_spectrumPeakHold = new float[1];
	m_bandwidth = 0;
	m_needSetup = true;

	m_grData = gr_data;

	//    _init();
}

RPCClient::~RPCClient()
{
	//    _close();
	//    delete _rpc_client;
}

void RPCClient::slotInit()
{
	QString tabs_setting_file = QCoreApplication::applicationDirPath();
	tabs_setting_file.append("/Tabs/RPC.ini");
	if(!readSettings(tabs_setting_file)){
		return;
	}

	m_rpcClient = new QxtRPCPeer();
	connect(m_rpcClient, SIGNAL(connectedToServer()), this, SLOT(slotRCPConnetion()));
	connect(m_rpcClient, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(slotErrorRPCConnection(QAbstractSocket::SocketError)));

	connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(slotSetCommand(IMessage*)));

	m_rpcClient->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
	m_rpcClient->attachSignal(this, SIGNAL(signalSetMainStationCor(int,int)), RPC_SLOT_SET_MAIN_STATION_COR);
	m_rpcClient->attachSignal(this, SIGNAL(signalSetBandwidth(int, float)), RPC_SLOT_SET_BANDWIDTH);
	m_rpcClient->attachSignal(this, SIGNAL(signalSetShift(int, float)), RPC_SLOT_SET_SHIFT);
	m_rpcClient->attachSignal(this, SIGNAL(signalRecognize(int, int)), RPC_SLOT_RECOGNIZE);
	m_rpcClient->attachSignal(this, SIGNAL(signalSSCorrelation(int, bool)), RPC_SLOT_SS_CORRELATION);
	m_rpcClient->attachSignal(this, SIGNAL(signalSetAvarageSpectrum(int,int)), RPC_SLOT_AVARAGE_SPECTRUM);

	m_rpcClient->attachSignal(this, SIGNAL(signalPRMSetFreq(int, short)), RPC_SLOT_PRM_SET_FREQ);
	m_rpcClient->attachSignal(this, SIGNAL(signalPRMRequestFreq(int)), RPC_SLOT_PRM_REQUEST_FREQ);
	m_rpcClient->attachSignal(this, SIGNAL(signalPRMSetAtt1(int, int)), RPC_SLOT_PRM_SET_ATT1);
	m_rpcClient->attachSignal(this, SIGNAL(signalPRMSetAtt2(int, int)), RPC_SLOT_PRM_SET_ATT2);
	m_rpcClient->attachSignal(this, SIGNAL(signalPRMSetFilter(int,int)), RPC_SLOT_PRM_SET_FILTER);


	m_rpcClient->attachSignal(this, SIGNAL(signalRequestStatus(int)), RPC_SLOT_REQUEST_STATUS);

	connect(this, SIGNAL(signalReconnection()), this, SLOT(slotReconnection()));
	connect(m_rpcClient, SIGNAL(disconnectedFromServer()), this, SLOT(slotRPCDisconnection()));

	//    return 0;
	slotStart();
}

int RPCClient::start()
{
	m_rpcClient->connect(m_ipRpc, m_portRpc);
	return 0;
}

int RPCClient::stop()
{
	m_rpcClient->disconnectServer();
	return 0;
}

void RPCClient::close()
{
	if(m_rpcClient->isClient())
	{
		m_rpcClient->disconnectServer();
		delete m_rpcClient;
		m_rpcClient = NULL;
	}
	emit signalFinished();
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
			flakonSetMainStationCor(data.toInt());
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
			requestSatatus();
			break;

		default:
			break;
	}
	msg->clenup();
}

void RPCClient::prmSetFreq(short freq)
{
	emit signalPRMSetFreq(m_tabProperty->get_id(), freq);
}

void RPCClient::prmRequestFreq()
{
	emit signalPRMRequestFreq(m_tabProperty->get_id());
}

void RPCClient::prmSetAtt1(int att1)
{
	emit signalPRMSetAtt1(m_tabProperty->get_id(), att1);
}

void RPCClient::prmSetAtt2(int att2)
{
	emit signalPRMSetAtt2(m_tabProperty->get_id(), att2);
}

void RPCClient::prmSetFilter(int index)
{
	emit signalPRMSetFilter(m_tabProperty->get_id(), index);
}

void RPCClient::flakonSetMainStationCor(int value)
{
	emit signalSetMainStationCor(m_tabProperty->get_id(), value);
}

void RPCClient::flakonSetAvarage(int value)
{
	emit signalSetAvarageSpectrum(m_tabProperty->get_id(), value);
}

void RPCClient::requestSatatus()
{
	emit signalRequestStatus(m_tabProperty->get_id());
}


void RPCClient::recognize()
{
	QMap<QString, QVariant>* m_data = m_dbManager->get(1, m_tabProperty->get_id());
	float bandwidth = m_data->value("value").toFloat();
	//    bandwidth /=1000;
	emit signalSetBandwidth(m_tabProperty->get_id(), bandwidth);
	m_data = m_dbManager->get(2, m_tabProperty->get_id());
	float shift = m_data->value("value").toFloat();
	//    shift /= 1000;
	emit signalSetShift(m_tabProperty->get_id(), shift);
	int s_type = 104;
	emit signalRecognize(m_tabProperty->get_id(), s_type);
}

void RPCClient::ssCorrelation(bool enable)
{
	//    int type = 103;
	emit signalSSCorrelation(m_tabProperty->get_id(), enable);
}

/// slot when connection complete
void RPCClient::slotRCPConnetion()
{

	emit signalSetClientId(m_tabProperty->get_id());
    ///server
	m_rpcClient->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(rpcSlotGettingPoints(rpc_send_points_vector)));
	m_rpcClient->attachSlot(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, this, SLOT(rpcSlotgettingdetectedbandwidth(rpc_send_points_vector)));

	m_rpcClient->attachSlot(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, this, SLOT(rpcSlotGettingModulation(QString)));
	m_rpcClient->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(rpcSlotServerSendCorrelation(int, int, rpc_send_points_vector)));

	m_rpcClient->attachSlot(RPC_SLOT_SERVER_PRM_STATUS, this, SLOT(rpcSlotServerPrmStatus(int, int, int, int)));

	m_rpcClient->attachSlot(RPC_SLOT_SERVER_STATUS, this, SLOT(rpcSlotServerStatus(bool)));

	CommandMessage *msg = new CommandMessage(COMMAND_PRM_REQUEST_FREQ, QVariant());
	this->setCommand(msg);
}

void RPCClient::slotRPCDisconnection()
{
	emit signalReconnection();
}

/// slot if have some error while connetiting
void RPCClient::slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
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
	//    _rpc_client->takeDevice()->;


	emit signalReconnection();
}

void RPCClient::slotReconnection()
{
	QIODevice *dev = m_rpcClient->takeDevice();
	m_rpcClient->connect(m_ipRpc, m_portRpc);
}

/// read rpc configuration from ini file
bool RPCClient::readSettings(const QString& settingsFile)
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings m_settings(settingsFile, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

	m_ipRpc = m_settings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_portRpc = m_settings.value("RPC_UI/Port", 24500).toInt();


	return true;
}

void RPCClient::slotStart()
{
	emit signalStart();
}

void RPCClient::slotStop()
{
	emit signalStop();
}

void RPCClient::slotFinish()
{
	emit signalFinishRPC();
}


/// getting points from server
void RPCClient::rpcSlotGettingPoints(rpc_send_points_vector points)
{
	m_grData->set_data(points, true);
}

void RPCClient::rpcSlotGettingDetectedBandwidth(rpc_send_points_vector points)
{
	m_grData->setDetectedAreas(points);
}

void RPCClient::rpcSlotGettingModulation(QString modulation)
{
	m_grData->set_def_modulation(modulation);
}

void RPCClient::rpcSlotServerSendCorrelation(int point1, int point2, rpc_send_points_vector points)
{
	if(point2 != m_tabProperty->get_id())
		m_grData->set_data(point2, points, true);
}

void RPCClient::rpcSlotServerPrmStatus(int prm_freq, int prm_filter, int prm_att1, int prm_att2)
{
	QMap<QString, QVariant>* data = m_dbManager->get(0, m_tabProperty->get_id());
	data->insert("value", prm_freq);
	m_dbManager->set(data);

	QMap<int, QVariant> status;
	status.insert(0, QVariant::fromValue(prm_freq));
	status.insert(1, QVariant::fromValue(prm_filter));
	status.insert(2, QVariant::fromValue(prm_att1));
	status.insert(3, QVariant::fromValue(prm_att2));

	m_controlPrm->set_status(status);

}

void RPCClient::rpcSlotServerStatus(bool state)
{
	m_parentTab->set_indicator(state);
}
