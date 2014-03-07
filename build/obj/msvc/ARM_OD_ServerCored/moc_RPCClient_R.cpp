/****************************************************************************
** Meta object code from reading C++ file 'RPCClient_R.h'
**
** Created: Thu 6. Mar 17:47:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/RPC/Client/RPCClient_R.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RPCClient_R.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RPCClient_R[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x05,
      50,   45,   12,   12, 0x05,
      77,   12,   12,   12, 0x05,
     102,   94,   12,   12, 0x05,
     143,   12,   12,   12, 0x05,
     157,   12,   12,   12, 0x05,
     170,   12,   12,   12, 0x05,
     191,  188,   12,   12, 0x05,
     214,   45,   12,   12, 0x05,
     242,   45,   12,   12, 0x05,
     275,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     300,   12,  296,   12, 0x08,
     308,   12,  296,   12, 0x08,
     315,   13,   12,   12, 0x08,
     342,   13,   12,   12, 0x08,
     367,   94,   12,   12, 0x08,
     406,   12,   12,   12, 0x08,
     415,   12,   12,   12, 0x08,
     435,   12,   12,   12, 0x08,
     471,  459,   12,   12, 0x08,
     525,   12,   12,   12, 0x08,
     545,   12,   12,   12, 0x0a,
     556,   12,   12,   12, 0x0a,
     568,   12,   12,   12, 0x0a,
     579,   12,   12,   12, 0x0a,
     595,  592,   12,   12, 0x0a,
     637,  592,   12,   12, 0x0a,
     684,  592,   12,   12, 0x0a,
     729,  592,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RPCClient_R[] = {
    "RPCClient_R\0\0msg\0signalSetCommand(IMessage*)\0"
    "data\0signalPushMsh(QByteArray*)\0"
    "signalFinished()\0msg_ptr\0"
    "signalSendData(QSharedPointer<IMessage>)\0"
    "signalStart()\0signalStop()\0signalFinishRPC()\0"
    "id\0signalSetClientId(int)\0"
    "signalSetSolver(QByteArray)\0"
    "signalSetSolverClear(QByteArray)\0"
    "signalReconnection()\0int\0start()\0"
    "stop()\0_slotSetCommand(IMessage*)\0"
    "_slotPushMsg(QByteArray)\0"
    "_slotGetData(QSharedPointer<IMessage>)\0"
    "_close()\0_slotRCPConnetion()\0"
    "_slotRPCDisconnection()\0socketError\0"
    "_slotErrorRPCConnection(QAbstractSocket::SocketError)\0"
    "_slotReconnection()\0slotInit()\0"
    "slotStart()\0slotStop()\0slotFinish()\0"
    "ba\0rpc_slot_server_send_bpla_def(QByteArray)\0"
    "rpc_slot_server_send_bpla_def_auto(QByteArray)\0"
    "rpc_slot_server_atlant_direction(QByteArray)\0"
    "rpc_slot_server_atlant_position(QByteArray)\0"
};

const QMetaObject RPCClient_R::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RPCClient_R,
      qt_meta_data_RPCClient_R, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RPCClient_R::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RPCClient_R::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RPCClient_R::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RPCClient_R))
        return static_cast<void*>(const_cast< RPCClient_R*>(this));
    if (!strcmp(_clname, "IClient"))
        return static_cast< IClient*>(const_cast< RPCClient_R*>(this));
    return QObject::qt_metacast(_clname);
}

int RPCClient_R::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 1: signalPushMsh((*reinterpret_cast< QByteArray*(*)>(_a[1]))); break;
        case 2: signalFinished(); break;
        case 3: signalSendData((*reinterpret_cast< QSharedPointer<IMessage>(*)>(_a[1]))); break;
        case 4: signalStart(); break;
        case 5: signalStop(); break;
        case 6: signalFinishRPC(); break;
        case 7: signalSetClientId((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: signalSetSolver((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 9: signalSetSolverClear((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 10: signalReconnection(); break;
        case 11: { int _r = start();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { int _r = stop();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: _slotSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 14: _slotPushMsg((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 15: _slotGetData((*reinterpret_cast< QSharedPointer<IMessage>(*)>(_a[1]))); break;
        case 16: _close(); break;
        case 17: _slotRCPConnetion(); break;
        case 18: _slotRPCDisconnection(); break;
        case 19: _slotErrorRPCConnection((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 20: _slotReconnection(); break;
        case 21: slotInit(); break;
        case 22: slotStart(); break;
        case 23: slotStop(); break;
        case 24: slotFinish(); break;
        case 25: rpc_slot_server_send_bpla_def((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 26: rpc_slot_server_send_bpla_def_auto((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 27: rpc_slot_server_atlant_direction((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 28: rpc_slot_server_atlant_position((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void RPCClient_R::signalSetCommand(IMessage * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RPCClient_R::signalPushMsh(QByteArray * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RPCClient_R::signalFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void RPCClient_R::signalSendData(QSharedPointer<IMessage> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RPCClient_R::signalStart()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void RPCClient_R::signalStop()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void RPCClient_R::signalFinishRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void RPCClient_R::signalSetClientId(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void RPCClient_R::signalSetSolver(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void RPCClient_R::signalSetSolverClear(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void RPCClient_R::signalReconnection()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
QT_END_MOC_NAMESPACE
