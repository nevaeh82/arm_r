/****************************************************************************
** Meta object code from reading C++ file 'RPCClient.h'
**
** Created: Thu 6. Mar 17:46:31 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/Tabs/RPC/RPCClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RPCClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RPCClient[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x05,
      43,   10,   10,   10, 0x05,
      60,   10,   10,   10, 0x05,
      74,   10,   10,   10, 0x05,
      87,   10,   10,   10, 0x05,
     108,  105,   10,   10, 0x05,
     136,  131,   10,   10, 0x05,
     167,  131,   10,   10, 0x05,
     199,  131,   10,   10, 0x05,
     236,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     261,   10,  257,   10, 0x08,
     269,   10,  257,   10, 0x08,
     276,   11,   10,   10, 0x08,
     303,   10,   10,   10, 0x08,
     312,   10,   10,   10, 0x08,
     332,   10,   10,   10, 0x08,
     368,  356,   10,   10, 0x08,
     422,   10,   10,   10, 0x08,
     442,   10,   10,   10, 0x0a,
     453,   10,   10,   10, 0x0a,
     465,   10,   10,   10, 0x0a,
     476,   10,   10,   10, 0x0a,
     521,  489,   10,   10, 0x0a,
     579,  131,   10,   10, 0x0a,
     621,  131,   10,   10, 0x0a,
     654,  131,   10,   10, 0x0a,
     692,  131,   10,   10, 0x0a,
     724,  131,   10,   10, 0x0a,
     774,  131,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RPCClient[] = {
    "RPCClient\0\0msg\0signalSetCommand(IMessage*)\0"
    "signalFinished()\0signalStart()\0"
    "signalStop()\0signalFinishRPC()\0id\0"
    "signalSetClientId(int)\0data\0"
    "signalSetNIIPPBPLA(QByteArray)\0"
    "signalSetSolverData(QByteArray)\0"
    "signalSetSolverDataClear(QByteArray)\0"
    "signalReconnection()\0int\0start()\0"
    "stop()\0_slotSetCommand(IMessage*)\0"
    "_close()\0_slotRCPConnetion()\0"
    "_slotRPCDisconnection()\0socketError\0"
    "_slotErrorRPCConnection(QAbstractSocket::SocketError)\0"
    "_slotReconnection()\0slotInit()\0"
    "slotStart()\0slotStop()\0slotFinish()\0"
    "id,point,alt,speed,course,state\0"
    "rpc_send_bla_points(int,QPointF,double,double,double,int)\0"
    "rpc_slot_server_send_ais_data(QByteArray)\0"
    "rpc_send_bpla_points(QByteArray)\0"
    "rpc_send_bpla_points_auto(QByteArray)\0"
    "rpc_send_niipp_data(QByteArray)\0"
    "rpc_slot_server_send_atlant_direction(QByteArray)\0"
    "rpc_slot_server_send_atlant_position(QByteArray)\0"
};

const QMetaObject RPCClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RPCClient,
      qt_meta_data_RPCClient, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RPCClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RPCClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RPCClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RPCClient))
        return static_cast<void*>(const_cast< RPCClient*>(this));
    if (!strcmp(_clname, "IRPC"))
        return static_cast< IRPC*>(const_cast< RPCClient*>(this));
    return QObject::qt_metacast(_clname);
}

int RPCClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 1: signalFinished(); break;
        case 2: signalStart(); break;
        case 3: signalStop(); break;
        case 4: signalFinishRPC(); break;
        case 5: signalSetClientId((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: signalSetNIIPPBPLA((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: signalSetSolverData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: signalSetSolverDataClear((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 9: signalReconnection(); break;
        case 10: { int _r = start();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: { int _r = stop();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: _slotSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 13: _close(); break;
        case 14: _slotRCPConnetion(); break;
        case 15: _slotRPCDisconnection(); break;
        case 16: _slotErrorRPCConnection((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 17: _slotReconnection(); break;
        case 18: slotInit(); break;
        case 19: slotStart(); break;
        case 20: slotStop(); break;
        case 21: slotFinish(); break;
        case 22: rpc_send_bla_points((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 23: rpc_slot_server_send_ais_data((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 24: rpc_send_bpla_points((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 25: rpc_send_bpla_points_auto((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 26: rpc_send_niipp_data((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 27: rpc_slot_server_send_atlant_direction((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 28: rpc_slot_server_send_atlant_position((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void RPCClient::signalSetCommand(IMessage * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RPCClient::signalFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void RPCClient::signalStart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void RPCClient::signalStop()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void RPCClient::signalFinishRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void RPCClient::signalSetClientId(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RPCClient::signalSetNIIPPBPLA(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RPCClient::signalSetSolverData(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void RPCClient::signalSetSolverDataClear(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void RPCClient::signalReconnection()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
