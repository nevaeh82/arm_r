#include "RPCServer.h"

#include <QDebug>

RPCServer::RPCServer(IRouter* router, QObject *parent) :
	RpcServerBase(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(RPCServer)), parent)
{
	m_router = router;
	m_subscriber = router->get_subscriber();

    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");
}

RPCServer::~RPCServer()
{
}

bool RPCServer::start(quint16 port, QHostAddress address)
{
	connect(m_serverPeer, SIGNAL(clientConnected(quint64)), this, SLOT(slotRPCConnetion(quint64)));
	connect(m_serverPeer, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(slotErrorRPCConnection(QAbstractSocket::SocketError)));
	connect(m_serverPeer, SIGNAL(clientDisconnected(quint64)), this, SLOT(slotRPCDisconnected(quint64)));


	m_serverPeer->attachSlot(RPC_SLOT_SET_CLIENT_ID, this, SLOT(rpcSlotSetClientId(quint64,int)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_MAIN_STATION_COR, this, SLOT(rpcSlotSetMainStationCor(quint64,int,int)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_BANDWIDTH, this, SLOT(rpcSlotSetBandwidth(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_SHIFT, this, SLOT(rpcSlotSetShift(quint64, int, float)));
	m_serverPeer->attachSlot(RPC_SLOT_RECOGNIZE, this, SLOT(rpcSlotRecognize(quint64, int, int)));
	m_serverPeer->attachSlot(RPC_SLOT_SS_CORRELATION, this, SLOT(rpcSlotSsCorrelation(quint64,int,bool)));
	m_serverPeer->attachSlot(RPC_SLOT_AVARAGE_SPECTRUM, this, SLOT(rpcSlotSetAvarageSpectrum(quint64,int,int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_FREQ, this, SLOT(rpcSlotPrmSetFreq(quint64, int, short)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_REQUEST_FREQ, this, SLOT(rpcSlotPrmRequestFreq(quint64,int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_ATT1, this, SLOT(rpcSlotPrmSetAtt1(quint64, int, int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_ATT2, this, SLOT(rpcSlotPrmSetAtt2(quint64, int, int)));
	m_serverPeer->attachSlot(RPC_SLOT_PRM_SET_FILTER, this, SLOT(rpcSlotPrmSetFilter(quint64, int, int)));

	m_serverPeer->attachSlot(RPC_SLOT_SET_ATLANT_FREQUENCY, this, SLOT(rpcSlotSetAtlantFrequency(quint64,QByteArray)));

	m_serverPeer->attachSlot(RPC_SLOT_SET_DATA_TO_SOLVER, this, SLOT(rpcSlotSetDataToSolver(quint64,QByteArray)));
	m_serverPeer->attachSlot(RPC_SLOT_SET_CLEAR_TO_SOLVER, this, SLOT(rpcSlotSetClearToSolver(quint64,QByteArray)));

	m_serverPeer->attachSlot(RPC_SLOT_REQUEST_STATUS, this, SLOT(rpcSlotRequestStatus(quint64, int)));

	return RpcServerBase::start(port, address);
}

/// slot if have some error while connetiting
void RPCServer::slotErrorRPCConnection(QAbstractSocket::SocketError socketError)
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
void RPCServer::slotRPCConnetion(quint64 client)
{
    QTextStream(stdout) << "Client connection" << client << endl;
}

void RPCServer::slotRPCDisconnected(quint64 client)
{
	IClient *cl = m_mapClients.value(client);
	m_subscriber->remove_subscription(cl);
    delete cl;

	m_mapClients.remove(client);
}

/// add client identification to QMap for assosiating with client sender
void RPCServer::rpcSlotSetClientId(quint64 client, int id)
{
	if(m_mapClients.contains(client))
    {
		delete m_mapClients.value(client);
    }

    if(id == 6)
    {
		AtlantController* atlant_controller = new AtlantController(m_router, this);
		m_mapClients.insert(client, atlant_controller);
		connect(atlant_controller, SIGNAL(signalSendToRPCAtlantDirection(quint64,QByteArray*)), this, SLOT(rpcSlotSendAtalntData(quint64, QByteArray*)));
		m_subscriber->add_subscription(ATLANT_DIRECTION, atlant_controller);
		m_subscriber->add_subscription(ATLANT_POSITION, atlant_controller);
        atlant_controller->set_id(id);
        atlant_controller->set_type(3);
        QTextStream(stdout) << "set id = " << id << "for client = " << client << endl;
        return;
    }
    if(id == 701)
    {
		RPCClient_OD* cl = new RPCClient_OD(m_router, this);
		m_mapClients.insert(client, cl);
		m_subscriber->add_subscription(FLAKON_BPLA, cl);
		m_subscriber->add_subscription(FLAKON_BPLA_AUTO, cl);
		m_subscriber->add_subscription(ATLANT_DIRECTION, cl);
		m_subscriber->add_subscription(ATLANT_POSITION, cl);
        cl->set_id(id);
        cl->set_type(1);
		connect(cl, SIGNAL(signalSendToRPCODBpla(quint64,QByteArray*)), this, SLOT(rpcSlotSendBpla(quint64,QByteArray*)));
		connect(cl, SIGNAL(signalSendToRPCODBplaAuto(quint64,QByteArray*)), this, SLOT(rpcSlotSendBplaAuto(quint64,QByteArray*)));
		connect(cl, SIGNAL(signalSendRPCODAtlantDirection(quint64, QByteArray*)), this, SLOT(rpcSlotSendAtlantDirection(quint64, QByteArray*)));
		connect(cl, SIGNAL(signalSendRPCODAtlantPosition(quint64,QByteArray*)), this, SLOT(rpcSlotSendAtalntDataPos(quint64,QByteArray*)));
        return;
    }

	RPCClientFlakon* cl = new RPCClientFlakon(m_router, this);
	m_mapClients.insert(client, cl);
	connect(cl, SIGNAL(signalSendToRPCPoints(quint64,rpc_send_points_vector)), this, SLOT(rpcSlotSendFft(quint64,rpc_send_points_vector)));
	connect(cl, SIGNAL(signalSendToRPCDetectedBandwidth(quint64,rpc_send_points_vector)), this,SLOT(rpcSlotsenddetectedbandwidth(quint64,rpc_send_points_vector)));
	connect(cl, SIGNAL(signalResponseModulationType(quint64,QString)), this, SLOT(rpcSlotSendRespModulation(quint64,QString)));
	connect(cl, SIGNAL(signalSendToRPCCorPoints(quint64,quint32,quint32,rpc_send_points_vector)), this, SLOT(rpcSlotSendCorr(quint64,quint32,quint32,rpc_send_points_vector)));
	connect(cl, SIGNAL(signalPRMStatus(quint64,QByteArray*)), this, SLOT(rpcSlotPrmStatus(quint64,QByteArray*)));

	connect(cl, SIGNAL(signalStatus(quint64, QByteArray*)), this, SLOT(rpcSlotStatus(quint64,QByteArray*)));

    cl->set_id(id);
    cl->set_type(2);
    QTextStream(stdout) << "set id = " << id << "for client = " << client << endl;

    /// added subscription with type == 1001
	m_subscriber->add_subscription(FLAKON_FFT, cl);
	m_subscriber->add_subscription(FLAKON_DETECTED_BANDWIDTH, cl);
	m_subscriber->add_subscription(FLAKON_CORRELATION, cl);
	m_subscriber->add_subscription(FLAKON_SIGNAL_TYPE, cl);
	m_subscriber->add_subscription(PRM_STATUS, cl);
	m_subscriber->add_subscription(CONNECTION_STATUS, cl);
}

void RPCServer::rpcSlotSetMainStationCor(quint64 client, int id, int station)
{
    QTextStream(stdout) << "received " << station << "from client" << client << "id = " << id << endl;
	if(!m_mapClients.contains(client))
    {
        QTextStream(stdout) << "this client doesn't registered!" << endl;
    }

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << station;

    QSharedPointer<IMessage> msg(new Message(id, RPC_MAIN_STATION_COR, ba));
	m_subscriber->data_ready(RPC_MAIN_STATION_COR, msg);
}

quint64 RPCServer::getClientId(IClient *client)
{
	return m_mapClients.key(client);
}

void RPCServer::rpcSlotSetBandwidth(quint64 client, int id, float bandwidth)
{
    QTextStream(stdout) << "received " << bandwidth << "from client" << client << "id = " << id << endl;
	if(!m_mapClients.contains(client))
    {
        QTextStream(stdout) << "this client doesn't registered!" << endl;
    }

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << bandwidth;

    QSharedPointer<IMessage> msg(new Message(id, RCP_BANDWIDTH_TYPE, ba));
	m_subscriber->data_ready(RCP_BANDWIDTH_TYPE, msg);
}

void RPCServer::rpcSlotSetShift(quint64 client, int id, float shift)
{
    QTextStream(stdout) << "received " << shift << "from client" << client << endl;
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << shift;

    QSharedPointer<IMessage> msg(new Message(id, RCP_SHIFT_TYPE, ba));
	m_subscriber->data_ready(RCP_SHIFT_TYPE, msg);
}

void RPCServer::rpcSlotRecognize(quint64 client, int id, int type)
{
    QTextStream(stdout) << "received " << type << "from client" << client << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << id;

    QSharedPointer<IMessage> msg(new Message(id, RCP_COMMAND_RECOGNIZE, ba));
	m_subscriber->data_ready(RCP_COMMAND_RECOGNIZE, msg);
}

void RPCServer::rpcSlotSsCorrelation(quint64 client, int id, bool enable)
{
    QTextStream(stdout) << "received " << enable << "from client" << client << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << enable;

    QSharedPointer<IMessage> msg(new Message(id, RPC_SS_CORRELATION, ba));
	m_subscriber->data_ready(RPC_SS_CORRELATION, msg);
}

void RPCServer::rpcSlotSetAvarageSpectrum(quint64 client, int id, int avarage)
{
    QTextStream(stdout) << "received " << avarage << "from client" << client << endl;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << avarage;

    QSharedPointer<IMessage> msg(new Message(id, RPC_AVARAGE_SPECTRUM, ba));
	m_subscriber->data_ready(RPC_AVARAGE_SPECTRUM, msg);
}


/// send command to prm300 for set central freq
void RPCServer::rpcSlotPrmSetFreq(quint64 client, int id, short freq)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << freq;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_FREQ, ba));
	m_subscriber->data_ready(PRM_SET_FREQ, msg);
}

void RPCServer::rpcSlotPrmRequestFreq(quint64 client, int id)
{
    QSharedPointer<IMessage> msg(new Message(id, PRM_REQUEST_FREQ, NULL));
	m_subscriber->data_ready(PRM_REQUEST_FREQ, msg);
}

void RPCServer::rpcSlotPrmSetAtt1(quint64 client, int id, int value)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << value;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_ATT1, ba));
	m_subscriber->data_ready(PRM_SET_ATT1, msg);
}

void RPCServer::rpcSlotPrmSetAtt2(quint64 client, int id, int value)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << value;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_ATT2, ba));
	m_subscriber->data_ready(PRM_SET_ATT2, msg);
}

void RPCServer::rpcSlotPrmSetFilter(quint64 client, int id, int index)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << index;

    QSharedPointer<IMessage> msg(new Message(id, PRM_SET_FILTER, ba));
	m_subscriber->data_ready(PRM_SET_FILTER, msg);
}

void RPCServer::rpcSlotSetDataToSolver(quint64 client, QByteArray data)
{
    QByteArray* ba = new QByteArray();
    ba->append(data);
    QSharedPointer<IMessage> msg(new Message(701, SET_SOLVER, ba));
	m_subscriber->data_ready(SET_SOLVER, msg);
}

void RPCServer::rpcSlotSetClearToSolver(quint64 client, QByteArray data)
{
    QByteArray* ba = new QByteArray();
    ba->append(data);
    QSharedPointer<IMessage> msg(new Message(701, SET_SOLVER_CLEAR, ba));
	m_subscriber->data_ready(SET_SOLVER_CLEAR, msg);
}

void RPCServer::rpcSlotSendFft(quint64 client, rpc_send_points_vector points)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_POINTS, QVariant::fromValue(points));
}

void RPCServer::rpcSlotsenddetectedbandwidth(quint64 client, rpc_send_points_vector points)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, QVariant::fromValue(points));
}

void RPCServer::rpcSlotSendCorr(quint64 client, quint32 point1, quint32 point2, rpc_send_points_vector points)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_CORRELATION, QVariant::fromValue((int)point1), QVariant::fromValue((int)point2), QVariant::fromValue(points));
}

void RPCServer::rpcSlotSendRespModulation(quint64 client, QString modulation)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION, QVariant::fromValue(modulation));
}

void RPCServer::rpcSlotPrmStatus(quint64 client, QByteArray *data)
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
	m_serverPeer->call(client, RPC_SLOT_SERVER_PRM_STATUS, QVariant::fromValue((int)freq),  QVariant::fromValue((int)filter), \
                      QVariant::fromValue((int)att1), QVariant::fromValue((int)att2));
}

void RPCServer::rpcSlotStatus(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    bool state;

    ds >> state;

	m_serverPeer->call(client, RPC_SLOT_SERVER_STATUS, QVariant::fromValue((bool)state));
}

void RPCServer::rpcSlotSendAtalntData(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_ATLANT_DIRECTION, QVariant::fromValue(*data));
}

void RPCServer::rpcSlotSendAtalntDataPos(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_ATLANT_POSITION, QVariant::fromValue(*data));
}

void RPCServer::rpcSlotSetAtlantFrequency(quint64 client, QByteArray data)
{
    QByteArray* ba = new QByteArray(data);

    QSharedPointer<IMessage> msg(new Message(6, ATLANT_SET_FREQ, ba));
	m_subscriber->data_ready(ATLANT_SET_FREQ, msg);
}

void RPCServer::rpcSlotSendBpla(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    rpc_send_atlant_data msg;
    ds >> msg;

	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_BPLA_DEF, QVariant::fromValue(*data));
}

void RPCServer::rpcSlotSendBplaAuto(quint64 client, QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);
    rpc_send_atlant_data msg;
    ds >> msg;

	m_serverPeer->call(client, RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, QVariant::fromValue(*data));
}

/// to OD
void RPCServer::rpcSlotSendAtlantDirection(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_ATLANT_DIRECTION, QVariant::fromValue(*data));
}

void RPCServer::rpcSlotSendAtlantPosition(quint64 client, QByteArray *data)
{
	m_serverPeer->call(client, RPC_SLOT_SERVER_ATLANT_POSITION, QVariant::fromValue(*data));
}

void RPCServer::rpcSlotRequestStatus(quint64 client, int id)
{
    QByteArray* ba = new QByteArray();

    QSharedPointer<IMessage> msg(new Message(id, REQUEST_STATUS, ba));
	m_subscriber->data_ready(REQUEST_STATUS, msg);
}

void RPCServer::aboutToQuitApp()
{
}
