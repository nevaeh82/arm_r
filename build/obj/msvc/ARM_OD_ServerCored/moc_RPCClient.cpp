/****************************************************************************
** Meta object code from reading C++ file 'RPCClient.h'
**
** Created: Thu 6. Mar 17:47:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/RPC/RPCClient.h"
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
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   11,   10,   10, 0x05,
      51,   44,   10,   10, 0x05,
     128,  126,   10,   10, 0x05,
     172,  126,   10,   10, 0x05,
     219,  126,   10,   10, 0x05,
     270,  126,   10,   10, 0x05,
     315,  126,   10,   10, 0x05,
     367,  126,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     418,   11,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RPCClient[] = {
    "RPCClient\0\0msg_ptr\0signalReadyData(rpc_msg)\0"
    ",,,,,,\0"
    "signalSendToRPCBLAPoints(quint64,int,rpc_QPointF,double,double,double,"
    "int)\0"
    ",\0signalSendToRPCAISData(quint64,QByteArray*)\0"
    "signalSendToRPCBPLAPoints(quint64,QByteArray*)\0"
    "signalSendToRPCBPLAPointsAuto(quint64,QByteArray*)\0"
    "signalSendToNIIPPPoints(quint64,QByteArray*)\0"
    "signalSendToRPCAtlantDirection(quint64,QByteArray*)\0"
    "signalSendToRPCAtlantPosition(quint64,QByteArray*)\0"
    "_slotGetData(rpc_msg)\0"
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
    if (!strcmp(_clname, "IClient"))
        return static_cast< IClient*>(const_cast< RPCClient*>(this));
    return QObject::qt_metacast(_clname);
}

int RPCClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalReadyData((*reinterpret_cast< rpc_msg(*)>(_a[1]))); break;
        case 1: signalSendToRPCBLAPoints((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< rpc_QPointF(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7]))); break;
        case 2: signalSendToRPCAISData((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 3: signalSendToRPCBPLAPoints((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 4: signalSendToRPCBPLAPointsAuto((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 5: signalSendToNIIPPPoints((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 6: signalSendToRPCAtlantDirection((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 7: signalSendToRPCAtlantPosition((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray*(*)>(_a[2]))); break;
        case 8: _slotGetData((*reinterpret_cast< rpc_msg(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void RPCClient::signalReadyData(rpc_msg _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RPCClient::signalSendToRPCBLAPoints(quint64 _t1, int _t2, rpc_QPointF _t3, double _t4, double _t5, double _t6, int _t7)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RPCClient::signalSendToRPCAISData(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RPCClient::signalSendToRPCBPLAPoints(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RPCClient::signalSendToRPCBPLAPointsAuto(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RPCClient::signalSendToNIIPPPoints(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RPCClient::signalSendToRPCAtlantDirection(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RPCClient::signalSendToRPCAtlantPosition(quint64 _t1, QByteArray * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
