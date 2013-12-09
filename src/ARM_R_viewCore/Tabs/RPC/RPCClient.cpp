#include "RPCClient.h"

#include <QDebug>

RPCClient::RPCClient(TabsProperty *prop, IDBManager *db_manager, ITabSpectrum* parent_tab, GraphicData* gr_data, IControlPRM* control_prm)
{

    _control_prm = control_prm;
    _parent_tab = parent_tab;
    _tab_property = prop;
    _db_manager = db_manager;
    //    _rpc_client->attachSlot()

    connect(this, SIGNAL(signalStart()), this, SLOT(start()));
    connect(this, SIGNAL(signalStop()), this, SLOT(stop()));
    connect(this, SIGNAL(signalFinishRPC()), this, SLOT(_close()));

    _spectrum = new float[1];
    _spectrum_peak_hold = new float[1];
    _bandwidth = 0;
    _needSetup = true;

    _gr_data = gr_data;

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
    if(_read_settings(tabs_setting_file) != 0)
    {
        return;
    }

    _rpc_client = new QxtRPCPeer();
    connect(_rpc_client, SIGNAL(connectedToServer()), this, SLOT(_slotRCPConnetion()));
    connect(_rpc_client, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));

    connect(this, SIGNAL(signalSetCommand(IMessage*)), this, SLOT(_slotSetCommand(IMessage*)));

    _rpc_client->attachSignal(this, SIGNAL(signalSetClientId(int)), RPC_SLOT_SET_CLIENT_ID);
    _rpc_client->attachSignal(this, SIGNAL(signalSetMainStationCor(int,int)), RPC_SLOT_SET_MAIN_STATION_COR);
    _rpc_client->attachSignal(this, SIGNAL(signalSetBandwidth(int, float)), RPC_SLOT_SET_BANDWIDTH);
    _rpc_client->attachSignal(this, SIGNAL(signalSetShift(int, float)), RPC_SLOT_SET_SHIFT);
    _rpc_client->attachSignal(this, SIGNAL(signalRecognize(int, int)), RPC_SLOT_RECOGNIZE);
    _rpc_client->attachSignal(this, SIGNAL(signalSSCorrelation(int, bool)), RPC_SLOT_SS_CORRELATION);
    _rpc_client->attachSignal(this, SIGNAL(signalSetAvarageSpectrum(int,int)), RPC_SLOT_AVARAGE_SPECTRUM);

    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetFreq(int, short)), RPC_SLOT_PRM_SET_FREQ);
    _rpc_client->attachSignal(this, SIGNAL(signalPRMRequestFreq(int)), RPC_SLOT_PRM_REQUEST_FREQ);
    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetAtt1(int, int)), RPC_SLOT_PRM_SET_ATT1);
    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetAtt2(int, int)), RPC_SLOT_PRM_SET_ATT2);
    _rpc_client->attachSignal(this, SIGNAL(signalPRMSetFilter(int,int)), RPC_SLOT_PRM_SET_FILTER);


    _rpc_client->attachSignal(this, SIGNAL(signalRequestStatus(int)), RPC_SLOT_REQUEST_STATUS);

    connect(this, SIGNAL(signalReconnection()), this, SLOT(_slotReconnection()));
    connect(_rpc_client, SIGNAL(disconnectedFromServer()), this, SLOT(_slotRPCDisconnection()));

//    return 0;
    slotStart();
}

int RPCClient::start()
{
    _rpc_client->connect(_ip_RPC, _port_RPC);
    return 0;
}

int RPCClient::stop()
{
    _rpc_client->disconnectServer();
    return 0;
}

void RPCClient::_close()
{
    if(_rpc_client->isClient())
    {
        _rpc_client->disconnectServer();
        delete _rpc_client;
        _rpc_client = NULL;
    }
    emit signalFinished();
}

void RPCClient::set_command(IMessage *msg)
{
    emit signalSetCommand(msg);
}

void RPCClient::_slotSetCommand(IMessage *msg)
{
    _command_msg = msg;
    _form_command(_command_msg);
}

void RPCClient::_form_command(IMessage *msg)
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
        _recognize();
        break;
    case COMMAND_KM:
        _ss_correlation(data.toBool());
        break;
    case COMMAND_FLAKON_SET_MAIN_STATION_COR:
        _flakon_set_main_station_cor(data.toInt());
        break;
    case COMMAND_PRM_SET_FREQ:
        _prm_set_freq(data.toUInt());
        break;
    case COMMAND_PRM_REQUEST_FREQ:
        _prm_request_freq();
        break;
    case COMMAND_PRM_SET_ATT1:
        _prm_set_att1(data.toInt());
        break;
    case COMMAND_PRM_SET_ATT2:
        _prm_set_att2(data.toInt());
        break;
    case COMMAND_PRM_SET_FILTER:
        _prm_set_filter(data.toInt());
        break;
    case COMMAND_FLAKON_SET_AVARAGE:
        _flakon_set_avarage(data.toInt());
        break;
    case COMMAND_REQUEST_STATUS:
        _request_satatus();
    break;

    default:
        break;
    }
    msg->clenup();
}

void RPCClient::_prm_set_freq(short freq)
{
    emit signalPRMSetFreq(_tab_property->get_id(), freq);
}

void RPCClient::_prm_request_freq()
{
    emit signalPRMRequestFreq(_tab_property->get_id());
}

void RPCClient::_prm_set_att1(int att1)
{
    emit signalPRMSetAtt1(_tab_property->get_id(), att1);
}

void RPCClient::_prm_set_att2(int att2)
{
    emit signalPRMSetAtt2(_tab_property->get_id(), att2);
}

void RPCClient::_prm_set_filter(int index)
{
    emit signalPRMSetFilter(_tab_property->get_id(), index);
}

void RPCClient::_flakon_set_main_station_cor(int value)
{
    emit signalSetMainStationCor(_tab_property->get_id(), value);
}

void RPCClient::_flakon_set_avarage(int value)
{
    emit signalSetAvarageSpectrum(_tab_property->get_id(), value);
}

void RPCClient::_request_satatus()
{
    emit signalRequestStatus(_tab_property->get_id());
}


void RPCClient::_recognize()
{
    QMap<QString, QVariant>* m_data = _db_manager->get(1, _tab_property->get_id());
    float bandwidth = m_data->value("value").toFloat();
//    bandwidth /=1000;
    emit signalSetBandwidth(_tab_property->get_id(), bandwidth);
    m_data = _db_manager->get(2, _tab_property->get_id());
    float shift = m_data->value("value").toFloat();
//    shift /= 1000;
    emit signalSetShift(_tab_property->get_id(), shift);
    int s_type = 104;
    emit signalRecognize(_tab_property->get_id(), s_type);
}

void RPCClient::_ss_correlation(bool enable)
{
//    int type = 103;
    emit signalSSCorrelation(_tab_property->get_id(), enable);
}

/// slot when connection complete
void RPCClient::_slotRCPConnetion()
{
    emit signalSetClientId(_tab_property->get_id());
    ///server
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_POINTS, this, SLOT(rpc_slot_getting_points(rpc_send_points_vector)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, this, SLOT(rpc_slot_getting_modulation(QString)));
    _rpc_client->attachSlot(RPC_SLOT_SERVER_SEND_CORRELATION, this, SLOT(rpc_slot_server_send_correlation(int, int, rpc_send_points_vector)));

    _rpc_client->attachSlot(RPC_SLOT_SERVER_PRM_STATUS, this, SLOT(rpc_slot_server_prm_status(int, int, int, int)));

    _rpc_client->attachSlot(RPC_SLOT_SERVER_STATUS, this, SLOT(rpc_slot_server_status(bool)));

    CommandMessage *msg = new CommandMessage(COMMAND_PRM_REQUEST_FREQ, QVariant());
    this->set_command(msg);
}

void RPCClient::_slotRPCDisconnection()
{
    emit signalReconnection();
}

/// slot if have some error while connetiting
void RPCClient::_slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
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

void RPCClient::_slotReconnection()
{
    QIODevice *dev = _rpc_client->takeDevice();
    _rpc_client->connect(_ip_RPC, _port_RPC);
}

/// read rpc configuration from ini file
int RPCClient::_read_settings(QString path_to_ini_file_RPC)
{
    int error = -1;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QSettings m_settings(path_to_ini_file_RPC, QSettings::IniFormat);

    m_settings.setIniCodec(codec);

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        if(childKey.toLatin1() != "RPC_UI")
        {
            continue;
        }
        m_settings.beginGroup(childKey);

        _ip_RPC = m_settings.value("IP", "127.0.0.1").toString();
        _port_RPC = m_settings.value("Port", 24500).toInt();

        error = 0;
        m_settings.endGroup();
    }

    return error;
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
void RPCClient::rpc_slot_getting_points(rpc_send_points_vector points)
{
    _gr_data->set_data(points, true);
    //    _parent_tab->set_points_rpc(points);
}

void RPCClient::rpc_slot_getting_modulation(QString modulation)
{
    _gr_data->set_def_modulation(modulation);
}

void RPCClient::rpc_slot_server_send_correlation(int point1, int point2, rpc_send_points_vector points)
{
    if(point2 != _tab_property->get_id())
        _gr_data->set_data(point2, points, true);
}

void RPCClient::rpc_slot_server_prm_status(int prm_freq, int prm_filter, int prm_att1, int prm_att2)
{
    QMap<QString, QVariant>* data = _db_manager->get(0, _tab_property->get_id());
    data->insert("value", prm_freq);
    _db_manager->set(data);

    QMap<int, QVariant> status;
    status.insert(0, QVariant::fromValue(prm_freq));
    status.insert(1, QVariant::fromValue(prm_filter));
    status.insert(2, QVariant::fromValue(prm_att1));
    status.insert(3, QVariant::fromValue(prm_att2));

    _control_prm->set_status(status);

}

void RPCClient::rpc_slot_server_status(bool state)
{
    _parent_tab->set_indicator(state);
}
