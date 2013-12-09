#include "RPCServer.h"

#include <QDebug>

RPCServer::RPCServer(IRouter* router)
{
    _rpc_server = NULL;
    _router = router;
    _subscriber = router->get_subscriber();


    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");


//    double test = 99;
//    QByteArray *ba = new QByteArray();
//    QDataStream ds(ba, QIODevice::ReadWrite);
//    ds << test;
    //ba->fromRawData(reinterpret_cast<const char* >(&test), sizeof(double));
//    for(int i = 0; i < 2222299; ++i)
//    {
//        QSharedPointer<IMessage> msg(new Message(RCP_BANDWIDTH_TYPE, ba));
//        _subscriber->data_ready(RCP_BANDWIDTH_TYPE, msg);
//    }
}

RPCServer::~RPCServer()
{
    if(_rpc_server != NULL)
    {
        delete _rpc_server;
    }
}

int RPCServer::start()
{
    _rpc_server = new QxtRPCPeer();
    connect(_rpc_server, SIGNAL(clientConnected(quint64)), this, SLOT(_slotRPCConnetion(quint64)));
    connect(_rpc_server, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(_slotErrorRPCConnection(QAbstractSocket::SocketError)));
    connect(_rpc_server, SIGNAL(clientDisconnected(quint64)), this, SLOT(_slotRPCDisconnected(quint64)));


    _rpc_server->attachSlot(RPC_SLOT_SET_CLIENT_ID, this, SLOT(rpc_slot_set_client_id(quint64,int)));
    _rpc_server->attachSlot(RPC_SLOT_SET_MAIN_STATION_COR, this, SLOT(rpc_slot_set_main_station_cor(quint64,int,int)));
    _rpc_server->attachSlot(RPC_SLOT_SET_BANDWIDTH, this, SLOT(rpc_slot_set_bandwidth(quint64, int, float)));
    _rpc_server->attachSlot(RPC_SLOT_SET_SHIFT, this, SLOT(rpc_slot_set_shift(quint64, int, float)));
    _rpc_server->attachSlot(RPC_SLOT_RECOGNIZE, this, SLOT(rpc_slot_recognize(quint64, int, int)));
    _rpc_server->attachSlot(RPC_SLOT_SS_CORRELATION, this, SLOT(rpc_slot_ss_correlation(quint64,int,bool)));
    _rpc_server->attachSlot(RPC_SLOT_AVARAGE_SPECTRUM, this, SLOT(rpc_slot_set_avarage_spectrum(quint64,int,int)));
    _rpc_server->attachSlot(RPC_SLOT_PRM_SET_FREQ, this, SLOT(rpc_slot_prm_set_freq(quint64, int, short)));
    _rpc_server->attachSlot(RPC_SLOT_PRM_REQUEST_FREQ, this, SLOT(rpc_slot_prm_request_freq(quint64,int)));
    _rpc_server->attachSlot(RPC_SLOT_PRM_SET_ATT1, this, SLOT(rpc_slot_prm_set_att1(quint64, int, int)));
    _rpc_server->attachSlot(RPC_SLOT_PRM_SET_ATT2, this, SLOT(rpc_slot_prm_set_att2(quint64, int, int)));
    _rpc_server->attachSlot(RPC_SLOT_PRM_SET_FILTER, this, SLOT(rpc_slot_prm_set_filter(quint64, int, int)));

    _rpc_server->attachSlot(RPC_SLOT_SET_ATLANT_FREQUENCY, this, SLOT(rpc_slot_set_atlant_frequency(quint64,QByteArray)));

    _rpc_server->attachSlot(RPC_SLOT_SET_DATA_TO_SOLVER, this, SLOT(rpc_slot_set_data_to_solver(quint64,QByteArray)));
    _rpc_server->attachSlot(RPC_SLOT_SET_CLEAR_TO_SOLVER, this, SLOT(rpc_slot_set_clear_to_solver(quint64,QByteArray)));

    _rpc_server->attachSlot(RPC_SLOT_REQUEST_STATUS, this, SLOT(rpc_slot_request_status(quint64, int)));

    if(!_rpc_server->listen(QHostAddress::Any, 24500))
    {
        QTextStream(stdout) << "error" << endl;
    }
    return 0;
}

/// slot if have some error while connetiting
void RPCServer::_slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
{
    QTextStream(stdout) << "Have error in connection " << endl;
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
//        thiserror.append(("Ошибка! Произошла ошибка: " + _rpc_client->->errorString()));
        break;
    }
    QTextStream(stdout) << thiserror << endl;
}

/// slot when connection complete
void RPCServer::_slotRPCConnetion(quint64 client)
{
    QTextStream(stdout) << "Client connection" << client << endl;
}

void RPCServer::_slotRPCDisconnected(quint64 client)
{
    IClient *cl = _map_clients.value(client);
    _subscriber->remove_subscription(cl);
    delete cl;

    _map_clients.remove(client);
}

/// add client identification to QMap for assosiating with client sender
void RPCServer::rpc_slot_set_client_id(quint64 client, int id)
{
    if(_map_clients.contains(client))
    {
        delete _map_clients.value(client);
    }

    if(id == 6)
    {
        AtlantController* atlant_controller = new AtlantController(_router, this);
        _map_clients.insert(client, atlant_controller);
        connect(atlant_controller, SIGNAL(signalSendToRPCAtlantDirection(quint64,QByteArray*)), this, SLOT(rpc_slot_send_atalnt_data(quint64, QByteArray*)));
        _subscriber->add_subscription(ATLANT_DIRECTION, atlant_controller);
        _subscriber->add_subscription(ATLANT_POSITION, atlant_controller);
        atlant_controller->set_id(id);
        atlant_controller->set_type(3);
        QTextStream(stdout) << "set id = " << id << "for client = " << client << endl;
        return;
    }
    if(id == 701)
    {
        RPCClient_OD* cl = new RPCClient_OD(_router, this);
        _map_clients.insert(client, cl);
        _subscriber->add_subscription(FLAKON_BPLA, cl);
        _subscriber->add_subscription(FLAKON_BPLA_AUTO, cl);
        _subscriber->add_subscription(ATLANT_DIRECTION, cl);
        _subscriber->add_subscription(ATLANT_POSITION, cl);
        cl->set_id(id);
        cl->set_type(1);
        connect(cl, SIGNAL(signalSendToRPCODBpla(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla(quint64,QByteArray*)));
        connect(cl, SIGNAL(signalSendToRPCODBplaAuto(quint64,QByteArray*)), this, SLOT(rpc_slot_send_bpla_auto(quint64,QByteArray*)));
        connect(cl, SIGNAL(signalSendRPCODAtlantDirection(quint64, QByteArray*)), this, SLOT(rpc_slot_send_atlant_direction(quint64, QByteArray*)));
        connect(cl, SIGNAL(signalSendRPCODAtlantPosition(quint64,QByteArray*)), this, SLOT(rpc_slot_send_atalnt_data_pos(quint64,QByteArray*)));
        return;
    }

    RPCClientFlakon* cl = new RPCClientFlakon(_router, this);
    _map_clients.insert(client, cl);
    connect(cl, SIGNAL(signalSendToRPCPoints(quint64,rpc_send_points_vector)), this, SLOT(rpc_slot_send_FFT(quint64,rpc_send_points_vector)));
    connect(cl, SIGNAL(signalResponseModulationType(quint64,QString)), this, SLOT(rpc_slot_send_resp_modulation(quint64,QString)));
    connect(cl, SIGNAL(signalSendToRPCCorPoints(quint64,quint32,quint32,rpc_send_points_vector)), this, SLOT(rpc_slot_send_corr(quint64,quint32,quint32,rpc_send_points_vector)));
    connect(cl, SIGNAL(signalPRMStatus(quint64,QByteArray*)), this, SLOT(rpc_slot_prm_status(quint64,QByteArray*)));

    connect(cl, SIGNAL(signalStatus(quint64, QByteArray*)), this, SLOT(rpc_slot_status(quint64,QByteArray*)));

//    if(!_map_clients.contains(client))
//    {
//        return;
//        //delete _map_clients.value(client);
//    }
//    IClient *cl = _map_clients.value(client);
    cl->set_id(id);
    cl->set_type(2);
    QTextStream(stdout) << "set id = " << id << "for client = " << client << endl;

    /// added subscription with type == 1001
    _subscriber->add_subscription(FLAKON_FFT, cl);
    _subscriber->add_subscription(FLAKON_CORRELATION, cl);
    _subscriber->add_subscription(FLAKON_SIGNAL_TYPE, cl);
    _subscriber->add_subscription(PRM_STATUS, cl);
    _subscriber->add_subscription(CONNECTION_STATUS, cl);

}

void RPCServer::rpc_slot_set_main_station_cor(quint64 client, int id, int station)
{
    QTextStream(stdout) << "received " << station << "from client" << client << "id = " << id << endl;
    if(!_map_clients.contains(client))
    {
        QTextStream(stdout) << "this client doesn't registered!" << endl;
    }

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << station;

    QSharedPointer<IMessage> msg(new Message(id, RPC_MAIN_STATION_COR, ba));
    _subscriber->data_ready(RPC_MAIN_STATION_COR, msg);
}

int RPCServer::stop()
{
    if(_rpc_server != NULL)
    {
        delete _rpc_server;
    }
    return 0;
}

quint64 RPCServer::get_client_id(IClient *client)
{
    return _map_clients.key(client);
}

void RPCServer::rpc_slot_set_bandwidth(quint64 client, int id, float bandwidth)
{
    QTextStream(stdout) << "received " << bandwidth << "from client" << client << "id = " << id << endl;
    if(!_map_clients.contains(client))
    {
        QTextStream(stdout) << "this client doesn't registered!" << endl;
    }

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << bandwidth;

    QSharedPointer<IMessage> msg(new Message(id, RCP_BANDWIDTH_TYPE, ba));
    _subscriber->data_ready(RCP_BANDWIDTH_TYPE, msg);
}

void RPCServer::rpc_slot_set_shift(quint64 client, int id, float shift)
{

    QTextStream(stdout) << "received " << shift << "from client" << client << endl;
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << shift;

    QSharedPointer<IMessage> msg(new Message(id, RCP_SHIFT_TYPE, ba));
    _subscriber->data_ready(RCP_SHIFT_TYPE, msg);
}

void RPCServer::rpc_slot_recognize(quint64 client, int id, int type)
{

    QTextStream(stdout) << "received " << type << "from client" << client << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << id;

    QSharedPointer<IMessage> msg(new Message(id, RCP_COMMAND_RECOGNIZE, ba));
    _subscriber->data_ready(RCP_COMMAND_RECOGNIZE, msg);
}

void RPCServer::rpc_slot_ss_correlation(quint64 client, int id, bool enable)
{

    QTextStream(stdout) << "received " << enable << "from client" << client << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << enable;

    QSharedPointer<IMessage> msg(new Message(id, RPC_SS_CORRELATION, ba));
    _subscriber->data_ready(RPC_SS_CORRELATION, msg);
}

void RPCServer::rpc_slot_set_avarage_spectrum(quint64 client, int id, int avarage)
{
    QTextStream(stdout) << "received " << avarage << "from client" << client << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << avarage;

    QSharedPointer<IMessage> msg(new Message(id, RPC_AVARAGE_SPECTRUM, ba));
    _subscriber->data_ready(RPC_AVARAGE_SPECTRUM, msg);
}


/// send command to prm300 for set central freq
void RPCServer::rpc_slot_prm_set_freq(quint64 client, int id, short freq)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << freq;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_FREQ, ba));
    _subscriber->data_ready(PRM_SET_FREQ, msg);
}

void RPCServer::rpc_slot_prm_request_freq(quint64 client, int id)
{
    QSharedPointer<IMessage> msg(new Message(id, PRM_REQUEST_FREQ, NULL));
    _subscriber->data_ready(PRM_REQUEST_FREQ, msg);
}

void RPCServer::rpc_slot_prm_set_att1(quint64 client, int id, int value)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << value;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_ATT1, ba));
    _subscriber->data_ready(PRM_SET_ATT1, msg);
}

void RPCServer::rpc_slot_prm_set_att2(quint64 client, int id, int value)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << value;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_ATT2, ba));
    _subscriber->data_ready(PRM_SET_ATT2, msg);
}

void RPCServer::rpc_slot_prm_set_filter(quint64 client, int id, int index)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << index;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_FILTER, ba));
    _subscriber->data_ready(PRM_SET_FILTER, msg);
}

void RPCServer::rpc_slot_set_data_to_solver(quint64 client, QByteArray data)
{
    QByteArray* ba = new QByteArray();
    ba->append(data);
    QSharedPointer<IMessage> msg(new Message(701, SET_SOLVER, ba));
    _subscriber->data_ready(SET_SOLVER, msg);
}

void RPCServer::rpc_slot_set_clear_to_solver(quint64 client, QByteArray data)
{
    QByteArray* ba = new QByteArray();
    ba->append(data);
    QSharedPointer<IMessage> msg(new Message(701, SET_SOLVER_CLEAR, ba));
    _subscriber->data_ready(SET_SOLVER_CLEAR, msg);
}

void RPCServer::rpc_slot_send_FFT(quint64 client, rpc_send_points_vector points)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_POINTS, QVariant::fromValue(points));
}

void RPCServer::rpc_slot_send_corr(quint64 client, quint32 point1, quint32 point2, rpc_send_points_vector points)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_CORRELATION, QVariant::fromValue((int)point1), QVariant::fromValue((int)point2), QVariant::fromValue(points));
}

void RPCServer::rpc_slot_send_resp_modulation(quint64 client, QString modulation)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, QVariant::fromValue(modulation));
}

void RPCServer::rpc_slot_prm_status(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    quint16 freq;
    quint8  filter;
    quint8 att1;
    quint8 att2;

    ds >> freq;
    ds >> filter;
    ds >> att1;
    ds >> att2;
    _rpc_server->call(client, RPC_SLOT_SERVER_PRM_STATUS, QVariant::fromValue((int)freq),  QVariant::fromValue((int)filter), \
                      QVariant::fromValue((int)att1), QVariant::fromValue((int)att2));
}

void RPCServer::rpc_slot_status(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    bool state;

    ds >> state;

    _rpc_server->call(client, RPC_SLOT_SERVER_STATUS, QVariant::fromValue((bool)state));
}

void RPCServer::rpc_slot_send_atalnt_data(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_ATLANT_DIRECTION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_atalnt_data_pos(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_ATLANT_POSITION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_set_atlant_frequency(quint64 client, QByteArray data)
{
    QByteArray* ba = new QByteArray(data);

    QSharedPointer<IMessage> msg(new Message(6, ATLANT_SET_FREQ, ba));
    _subscriber->data_ready(ATLANT_SET_FREQ, msg);
}

void RPCServer::rpc_slot_send_bpla(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    rpc_send_atlant_data msg;
    ds >> msg;

    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_BPLA_DEF, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_bpla_auto(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    rpc_send_atlant_data msg;
    ds >> msg;

    _rpc_server->call(client, RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, QVariant::fromValue(*data));

}

/// to OD
void RPCServer::rpc_slot_send_atlant_direction(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_ATLANT_DIRECTION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_send_atlant_position(quint64 client, QByteArray *data)
{
    _rpc_server->call(client, RPC_SLOT_SERVER_ATLANT_POSITION, QVariant::fromValue(*data));
}

void RPCServer::rpc_slot_request_status(quint64 client, int id)
{
    QByteArray* ba = new QByteArray();

    QSharedPointer<IMessage> msg(new Message(id, REQUEST_STATUS, ba));
    _subscriber->data_ready(REQUEST_STATUS, msg);

}

void RPCServer::aboutToQuitApp()
{
}
