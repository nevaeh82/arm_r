#include "RPCClient_OD.h"

RPCClient_OD::RPCClient_OD(IRouter* router, IRPC* server):
    _id(0),
    _type(0)
{
    qRegisterMetaType<rpc_flakon_msg> ("rpc_flakon_msg");
    _router = router;
    _server = server;

    connect(this, SIGNAL(signalReadyData(rpc_flakon_msg)), this, SLOT(_slotGetData(rpc_flakon_msg)));
//    connect(this, SIGNAL(signalSendPoints(rpc_send_points_vector)), this, SLOT(_slotTest(rpc_send_points_vector)));
}

RPCClient_OD::~RPCClient_OD()
{
}

void RPCClient_OD::set_id(int id)
{
    _id = id;
}

void RPCClient_OD::send_data(QSharedPointer<IMessage> msg_ptr)
{
    _slotGetData(msg_ptr);
//    emit signalReadyData(msg_ptr);
}

int RPCClient_OD::get_id()
{
    return _id;
}

void RPCClient_OD::set_type(int type)
{
    _type = type;
}

int RPCClient_OD::get_type()
{
    return _type;
}

void RPCClient_OD::_slotGetData(rpc_flakon_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
    IMessage *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    quint64 cid = _server->get_client_id(this);
    switch(type1)
    {
    case FLAKON_BPLA:
//        ds >> points;
        emit signalSendToRPCODBpla(cid, dd);
        break;
    case FLAKON_BPLA_AUTO:
        emit signalSendToRPCODBplaAuto(cid, dd);
    case ATLANT_DIRECTION:
        emit signalSendRPCODAtlantDirection(cid, dd);
        break;
    case ATLANT_POSITION:
        emit signalSendRPCODAtlantPosition(cid, dd);
        break;

    default:
        break;
    }
}
