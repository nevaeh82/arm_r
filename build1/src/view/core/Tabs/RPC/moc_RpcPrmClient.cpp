/****************************************************************************
** Meta object code from reading C++ file 'RpcPrmClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/view/core/Tabs/RPC/RpcPrmClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcPrmClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcPrmClient_t {
    QByteArrayData data[31];
    char stringdata0[415];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcPrmClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcPrmClient_t qt_meta_stringdata_RpcPrmClient = {
    {
QT_MOC_LITERAL(0, 0, 12), // "RpcPrmClient"
QT_MOC_LITERAL(1, 13, 16), // "signalSetCommand"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 9), // "IMessage*"
QT_MOC_LITERAL(4, 41, 3), // "msg"
QT_MOC_LITERAL(5, 45, 14), // "signalFinished"
QT_MOC_LITERAL(6, 60, 11), // "signalStart"
QT_MOC_LITERAL(7, 72, 10), // "signalStop"
QT_MOC_LITERAL(8, 83, 15), // "signalFinishRPC"
QT_MOC_LITERAL(9, 99, 16), // "signalPRMSetFreq"
QT_MOC_LITERAL(10, 116, 4), // "name"
QT_MOC_LITERAL(11, 121, 4), // "freq"
QT_MOC_LITERAL(12, 126, 20), // "signalPRMRequestFreq"
QT_MOC_LITERAL(13, 147, 16), // "signalPRMSetAtt1"
QT_MOC_LITERAL(14, 164, 4), // "att1"
QT_MOC_LITERAL(15, 169, 16), // "signalPRMSetAtt2"
QT_MOC_LITERAL(16, 186, 4), // "att2"
QT_MOC_LITERAL(17, 191, 18), // "signalPRMSetFilter"
QT_MOC_LITERAL(18, 210, 5), // "index"
QT_MOC_LITERAL(19, 216, 19), // "signalRequestStatus"
QT_MOC_LITERAL(20, 236, 11), // "signalDataS"
QT_MOC_LITERAL(21, 248, 6), // "float*"
QT_MOC_LITERAL(22, 255, 10), // "signalData"
QT_MOC_LITERAL(23, 266, 24), // "rpcSlotGettingModulation"
QT_MOC_LITERAL(24, 291, 10), // "modulation"
QT_MOC_LITERAL(25, 302, 22), // "rpcSlotServerPrmStatus"
QT_MOC_LITERAL(26, 325, 19), // "rpcSlotServerStatus"
QT_MOC_LITERAL(27, 345, 7), // "message"
QT_MOC_LITERAL(28, 353, 29), // "rpcSlotPRM300FrequencyChanged"
QT_MOC_LITERAL(29, 383, 16), // "slotRpcConnetion"
QT_MOC_LITERAL(30, 400, 14) // "slotSetCommand"

    },
    "RpcPrmClient\0signalSetCommand\0\0IMessage*\0"
    "msg\0signalFinished\0signalStart\0"
    "signalStop\0signalFinishRPC\0signalPRMSetFreq\0"
    "name\0freq\0signalPRMRequestFreq\0"
    "signalPRMSetAtt1\0att1\0signalPRMSetAtt2\0"
    "att2\0signalPRMSetFilter\0index\0"
    "signalRequestStatus\0signalDataS\0float*\0"
    "signalData\0rpcSlotGettingModulation\0"
    "modulation\0rpcSlotServerPrmStatus\0"
    "rpcSlotServerStatus\0message\0"
    "rpcSlotPRM300FrequencyChanged\0"
    "slotRpcConnetion\0slotSetCommand"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcPrmClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       5,    0,  112,    2, 0x06 /* Public */,
       6,    0,  113,    2, 0x06 /* Public */,
       7,    0,  114,    2, 0x06 /* Public */,
       8,    0,  115,    2, 0x06 /* Public */,
       9,    2,  116,    2, 0x06 /* Public */,
      12,    1,  121,    2, 0x06 /* Public */,
      13,    2,  124,    2, 0x06 /* Public */,
      15,    2,  129,    2, 0x06 /* Public */,
      17,    2,  134,    2, 0x06 /* Public */,
      19,    1,  139,    2, 0x06 /* Public */,
      20,    2,  142,    2, 0x06 /* Public */,
      22,    2,  147,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      23,    1,  152,    2, 0x0a /* Public */,
      25,    1,  155,    2, 0x0a /* Public */,
      26,    1,  158,    2, 0x0a /* Public */,
      28,    1,  161,    2, 0x0a /* Public */,
      29,    0,  164,    2, 0x08 /* Private */,
      30,    1,  165,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,   10,   11,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   10,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   10,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   10,   18,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 21, 0x80000000 | 21,    2,    2,
    QMetaType::Void, 0x80000000 | 21, 0x80000000 | 21,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,   27,
    QMetaType::Void, QMetaType::QByteArray,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void RpcPrmClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcPrmClient *_t = static_cast<RpcPrmClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        case 1: _t->signalFinished(); break;
        case 2: _t->signalStart(); break;
        case 3: _t->signalStop(); break;
        case 4: _t->signalFinishRPC(); break;
        case 5: _t->signalPRMSetFreq((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 6: _t->signalPRMRequestFreq((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->signalPRMSetAtt1((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->signalPRMSetAtt2((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->signalPRMSetFilter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->signalRequestStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->signalDataS((*reinterpret_cast< float*(*)>(_a[1])),(*reinterpret_cast< float*(*)>(_a[2]))); break;
        case 12: _t->signalData((*reinterpret_cast< float*(*)>(_a[1])),(*reinterpret_cast< float*(*)>(_a[2]))); break;
        case 13: _t->rpcSlotGettingModulation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->rpcSlotServerPrmStatus((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 15: _t->rpcSlotServerStatus((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 16: _t->rpcSlotPRM300FrequencyChanged((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 17: _t->slotRpcConnetion(); break;
        case 18: _t->slotSetCommand((*reinterpret_cast< IMessage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcPrmClient::*_t)(IMessage * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalSetCommand)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalFinished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalStart)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalStop)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalFinishRPC)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(QString , short );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalPRMSetFreq)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalPRMRequestFreq)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalPRMSetAtt1)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalPRMSetAtt2)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalPRMSetFilter)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalRequestStatus)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(float * , float * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalDataS)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (RpcPrmClient::*_t)(float * , float * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPrmClient::signalData)) {
                *result = 12;
                return;
            }
        }
    }
}

const QMetaObject RpcPrmClient::staticMetaObject = {
    { &RpcRoutedClient::staticMetaObject, qt_meta_stringdata_RpcPrmClient.data,
      qt_meta_data_RpcPrmClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcPrmClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcPrmClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcPrmClient.stringdata0))
        return static_cast<void*>(const_cast< RpcPrmClient*>(this));
    return RpcRoutedClient::qt_metacast(_clname);
}

int RpcPrmClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RpcRoutedClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void RpcPrmClient::signalSetCommand(IMessage * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RpcPrmClient::signalFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RpcPrmClient::signalStart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RpcPrmClient::signalStop()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void RpcPrmClient::signalFinishRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void RpcPrmClient::signalPRMSetFreq(QString _t1, short _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RpcPrmClient::signalPRMRequestFreq(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RpcPrmClient::signalPRMSetAtt1(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void RpcPrmClient::signalPRMSetAtt2(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void RpcPrmClient::signalPRMSetFilter(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void RpcPrmClient::signalRequestStatus(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void RpcPrmClient::signalDataS(float * _t1, float * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void RpcPrmClient::signalData(float * _t1, float * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
