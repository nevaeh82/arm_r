#include "AtlantController.h"

AtlantController::AtlantController(IRouter* router, IRPC* server)
{
    _router = router;
    _server = server;
    _host = "192.168.30.20";
    _port = 1170;
    _name = "Detector";
//    _controller = new ControllerPelengator(_host, _port, _name, _router);
//    connect();
}

AtlantController::~AtlantController()
{
//    delete _controller;
}

//void AtlantController::connect()
//{
//    _controller->setCon(_host, _port, _name);
//}

void AtlantController::change_mode(int mode)
{
//    switch(_current_mode)
//    {
//    case 0:
//        _controller->slotStopScan();
////        emit signalScanStopRequest();
//        break;
//    case 1:
//        _controller->slotStopScan();

////        emit signalScanStopRequest();
//        break;
//    case 2:
//        _controller->slotStopDF();
////        emit signalDFStopRequest();
//        break;
//    case 3:
//        _controller->slotStopDF();

////        emit signalDFStopRequest();
//        break;
//    default:
//        break;
//    }

//    QVector<double> freqs;
//    switch(mode)
//    {
//    case 0:
//        _controller->slotAdaptation(800,200);
////        emit signalAdaptation(_sb_freq->value(), _sb_bw->value());
//        break;
//    case 1:
//        _controller->slotDuty(800, 200);
////        emit signalDuty(_sb_freq->value(), _sb_bw->value());
//        break;
//    case 2:
//        freqs.push_back(800);
//        _controller->slotCheck(freqs, 200);
////        emit signalCheck();
//        break;
//    case 3:
//        freqs.push_back(800);
//        _controller->slotWork(freqs, 200);
////        emit signalWork(freqs, _sb_bw->value());
//        break;
//    default:
//        break;
//    }
//    _current_mode = mode;
}

void AtlantController::set_id(int id)
{
    _id = id;
}

void AtlantController::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
    _slotGetData(msg_ptr);
}

int AtlantController::get_id()
{
    return _id;
}

void AtlantController::set_type(int type)
{
    _type = type;
}

int AtlantController::get_type()
{
    return _type;
}

void AtlantController::_slotGetData(rpc_flakon_msg msg_ptr)
{
    int type1 = 1;
    int id = 0;
    IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);
    quint64 cid = _server->getClientId(this);

    switch(type1)
    {
    case ATLANT_DIRECTION:
//        ds >> points;
        emit signalSendToRPCAtlantDirection(cid, dd);
        break;
    case ATLANT_POSITION:
        emit signalSendToRPCAtlantPosition(cid, dd);
        break;

    default:
        break;
    }
}
