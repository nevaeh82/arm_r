/****************************************************************************
** Meta object code from reading C++ file 'RpcAtlantClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/view/core/Tabs/RPC/RpcAtlantClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcAtlantClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcAtlantClient_t {
    QByteArrayData data[18];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcAtlantClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcAtlantClient_t qt_meta_stringdata_RpcAtlantClient = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RpcAtlantClient"
QT_MOC_LITERAL(1, 16, 16), // "signalSetCommand"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 9), // "IMessage*"
QT_MOC_LITERAL(4, 44, 3), // "msg"
QT_MOC_LITERAL(5, 48, 14), // "signalFinished"
QT_MOC_LITERAL(6, 63, 11), // "signalStart"
QT_MOC_LITERAL(7, 75, 10), // "signalStop"
QT_MOC_LITERAL(8, 86, 15), // "signalFinishRPC"
QT_MOC_LITERAL(9, 102, 13), // "signalSetFreq"
QT_MOC_LITERAL(10, 116, 4), // "data"
QT_MOC_LITERAL(11, 121, 18), // "signalReconnection"
QT_MOC_LITERAL(12, 140, 13), // "registerRoute"
QT_MOC_LITERAL(13, 154, 22), // "slotErrorRPCConnection"
QT_MOC_LITERAL(14, 177, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(15, 206, 11), // "socketError"
QT_MOC_LITERAL(16, 218, 14), // "slotSetCommand"
QT_MOC_LITERAL(17, 233, 28) // "rpcSlotServerAtlantDirection"

    },
    "RpcAtlantClient\0signalSetCommand\0\0"
    "IMessage*\0msg\0signalFinished\0signalStart\0"
    "signalStop\0signalFinishRPC\0signalSetFreq\0"
    "data\0signalReconnection\0registerRoute\0"
    "slotErrorRPCConnection\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "slotSetCommand\0rpcSlotServerAtlantDirection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcAtlantClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,
       6,    0,   73,    2, 0x06 /* Public */,
       7,    0,   74,    2, 0x06 /* Public */,
       8,    0,   75,    2, 0x06 /* Public */,
       9,    1,   76,    2, 0x06 /* Public */,
      11,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   80,    2, 0x08 /* Private */,
      13,    1,   81,    2, 0x08 /* Private */,
      16,    1,   84,    2, 0x08 /* Private */,
      17,    1,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QByteArray,   10,

       0        // eod
};

void RpcAtlantClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcAtlantClient *_t = static_cast<RpcAtlantClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 1: _t->signalFinished(); break;
        case 2: _t->signalStart(); break;
        case 3: _t->signalStop(); break;
        case 4: _t->signalFinishRPC(); break;
        case 5: _t->signalSetFreq((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->signalReconnection(); break;
        case 7: _t->registerRoute(); break;
        case 8: _t->slotErrorRPCConnection((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 9: _t->slotSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 10: _t->rpcSlotServerAtlantDirection((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcAtlantClient::*_t)(IMessage * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalSetCommand)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RpcAtlantClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalFinished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RpcAtlantClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalStart)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RpcAtlantClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalStop)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RpcAtlantClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalFinishRPC)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RpcAtlantClient::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalSetFreq)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (RpcAtlantClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcAtlantClient::signalReconnection)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject RpcAtlantClient::staticMetaObject = {
    { &RpcRoutedClient::staticMetaObject, qt_meta_stringdata_RpcAtlantClient.data,
      qt_meta_data_RpcAtlantClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcAtlantClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcAtlantClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcAtlantClient.stringdata0))
        return static_cast<void*>(const_cast< RpcAtlantClient*>(this));
    return RpcRoutedClient::qt_metacast(_clname);
}

int RpcAtlantClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RpcRoutedClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void RpcAtlantClient::signalSetCommand(IMessage * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RpcAtlantClient::signalFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RpcAtlantClient::signalStart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RpcAtlantClient::signalStop()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void RpcAtlantClient::signalFinishRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void RpcAtlantClient::signalSetFreq(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RpcAtlantClient::signalReconnection()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
