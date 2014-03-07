/****************************************************************************
** Meta object code from reading C++ file 'RPCClientFlakon.h'
**
** Created: Thu 6. Mar 15:53:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/RPC/RPCClientFlakon.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RPCClientFlakon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RPCClientFlakon[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x05,
      64,   57,   16,   16, 0x05,
     120,  105,   16,   16, 0x05,
     177,  166,   16,   16, 0x05,
     256,  231,   16,   16, 0x05,
     333,  329,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     370,   17,   16,   16, 0x08,
     401,  399,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RPCClientFlakon[] = {
    "RPCClientFlakon\0\0msg_ptr\0"
    "signalReadyData(rpc_flakon_msg)\0points\0"
    "signalSendPoints(rpc_send_points_vector)\0"
    "cid,modulation\0"
    "signalResponseModulationType(quint64,QString)\0"
    "cid,points\0"
    "signalSendToRPCPoints(quint64,rpc_send_points_vector)\0"
    "cid,point1,point2,points\0"
    "signalSendToRPCCorPoints(quint64,quint32,quint32,rpc_send_points_vecto"
    "r)\0"
    ",ba\0signalPRMStatus(quint64,QByteArray*)\0"
    "_slotGetData(rpc_flakon_msg)\0v\0"
    "_slotTest(rpc_send_points_vector)\0"
};

const QMetaObject RPCClientFlakon::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RPCClientFlakon,
      qt_meta_data_RPCClientFlakon, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RPCClientFlakon::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RPCClientFlakon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RPCClientFlakon::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RPCClientFlakon))
        return static_cast<void*>(const_cast< RPCClientFlakon*>(this));
    if (!strcmp(_clname, "IClient"))
        return static_cast< IClient*>(const_cast< RPCClientFlakon*>(this));
    return QObject::qt_metacast(_clname);
}

int RPCClientFlakon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalReadyData((*reinterpret_cast< rpc_flakon_msg(*)>(_a[1]))); break;
        case 1: signalSendPoints((*reinterpret_cast< rpc_send_points_vector(*)>(_a[1]))); break;
        case 2: signalResponseModulationType((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: signalSendToRPCPoints((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< rpc_send_points_vector(*)>(_a[2]))); break;
        case 4: signalSendToRPCCorPoints((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint32(*)>(_a[3])),(*reinterpret_cast< rpc_send_points_vector(*)>(_a[4]))); break;
        case 5: signalPRMStatus((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 6: _slotGetData((*reinterpret_cast< rpc_flakon_msg(*)>(_a[1]))); break;
        case 7: _slotTest((*reinterpret_cast< rpc_send_points_vector(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void RPCClientFlakon::signalReadyData(rpc_flakon_msg _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RPCClientFlakon::signalSendPoints(rpc_send_points_vector _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RPCClientFlakon::signalResponseModulationType(quint64 _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RPCClientFlakon::signalSendToRPCPoints(quint64 _t1, rpc_send_points_vector _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RPCClientFlakon::signalSendToRPCCorPoints(quint64 _t1, quint32 _t2, quint32 _t3, rpc_send_points_vector _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RPCClientFlakon::signalPRMStatus(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
