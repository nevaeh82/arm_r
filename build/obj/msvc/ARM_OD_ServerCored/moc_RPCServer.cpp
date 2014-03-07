/****************************************************************************
** Meta object code from reading C++ file 'RPCServer.h'
**
** Created: Thu 6. Mar 17:47:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/RPC/RPCServer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RPCServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RPCServer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   22,   10,   10, 0x08,
      95,   88,   10,   10, 0x08,
     122,   88,   10,   10, 0x08,
     162,  152,   10,   10, 0x0a,
     210,  198,   10,   10, 0x0a,
     254,  198,   10,   10, 0x0a,
     299,  198,   10,   10, 0x0a,
     385,  345,   10,   10, 0x0a,
     460,  198,   10,   10, 0x0a,
     504,  198,   10,   10, 0x0a,
     551,  198,   10,   10, 0x0a,
     603,  198,   10,   10, 0x0a,
     655,  198,   10,   10, 0x0a,
     706,  198,   10,   10, 0x0a,
     752,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RPCServer[] = {
    "RPCServer\0\0finished()\0socketError\0"
    "_slotErrorRPCConnection(QAbstractSocket::SocketError)\0"
    "client\0_slotRPCConnetion(quint64)\0"
    "_slotRPCDisconnected(quint64)\0client,id\0"
    "rpc_slot_set_client_id(quint64,int)\0"
    "client,data\0rpc_slot_set_niipp_data(quint64,QByteArray)\0"
    "rpc_slot_set_solver_data(quint64,QByteArray)\0"
    "rpc_slot_set_solver_clear(quint64,QByteArray)\0"
    "client,id,points,alt,speed,course,state\0"
    "rpc_slot_send_bla_points(quint64,int,rpc_QPointF,double,double,double,"
    "int)\0"
    "rpc_slot_send_ais_data(quint64,QByteArray*)\0"
    "rpc_slot_send_bpla_points(quint64,QByteArray*)\0"
    "rpc_slot_send_bpla_points_auto(quint64,QByteArray*)\0"
    "rpc_slot_send_atlant_direction(quint64,QByteArray*)\0"
    "rpc_slot_send_atlant_position(quint64,QByteArray*)\0"
    "rpc_slot_send_NIIPP_data(quint64,QByteArray*)\0"
    "aboutToQuitApp()\0"
};

const QMetaObject RPCServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RPCServer,
      qt_meta_data_RPCServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RPCServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RPCServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RPCServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RPCServer))
        return static_cast<void*>(const_cast< RPCServer*>(this));
    if (!strcmp(_clname, "IRPC"))
        return static_cast< IRPC*>(const_cast< RPCServer*>(this));
    return QObject::qt_metacast(_clname);
}

int RPCServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        case 1: _slotErrorRPCConnection((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 2: _slotRPCConnetion((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 3: _slotRPCDisconnected((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 4: rpc_slot_set_client_id((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: rpc_slot_set_niipp_data((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 6: rpc_slot_set_solver_data((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 7: rpc_slot_set_solver_clear((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 8: rpc_slot_send_bla_points((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< rpc_QPointF(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7]))); break;
        case 9: rpc_slot_send_ais_data((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 10: rpc_slot_send_bpla_points((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 11: rpc_slot_send_bpla_points_auto((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 12: rpc_slot_send_atlant_direction((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 13: rpc_slot_send_atlant_position((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 14: rpc_slot_send_NIIPP_data((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 15: aboutToQuitApp(); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void RPCServer::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
