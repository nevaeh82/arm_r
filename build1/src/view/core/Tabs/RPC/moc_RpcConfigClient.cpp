/****************************************************************************
** Meta object code from reading C++ file 'RpcConfigClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/view/core/Tabs/RPC/RpcConfigClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcConfigClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcConfigClient_t {
    QByteArrayData data[9];
    char stringdata0[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcConfigClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcConfigClient_t qt_meta_stringdata_RpcConfigClient = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RpcConfigClient"
QT_MOC_LITERAL(1, 16, 20), // "getStationListSignal"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 21), // "getAtlantConfigSignal"
QT_MOC_LITERAL(4, 60, 27), // "connectionEstablishedSignal"
QT_MOC_LITERAL(5, 88, 23), // "receivedStationListSlot"
QT_MOC_LITERAL(6, 112, 4), // "data"
QT_MOC_LITERAL(7, 117, 24), // "receivedAtlantConfigSlot"
QT_MOC_LITERAL(8, 142, 27) // "receivedDbConfigurationSlot"

    },
    "RpcConfigClient\0getStationListSignal\0"
    "\0getAtlantConfigSignal\0"
    "connectionEstablishedSignal\0"
    "receivedStationListSlot\0data\0"
    "receivedAtlantConfigSlot\0"
    "receivedDbConfigurationSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcConfigClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       4,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   51,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       8,    1,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::QByteArray,    6,

       0        // eod
};

void RpcConfigClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcConfigClient *_t = static_cast<RpcConfigClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getStationListSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->getAtlantConfigSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->connectionEstablishedSignal(); break;
        case 3: _t->receivedStationListSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->receivedAtlantConfigSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->receivedDbConfigurationSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcConfigClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigClient::getStationListSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RpcConfigClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigClient::getAtlantConfigSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RpcConfigClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigClient::connectionEstablishedSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject RpcConfigClient::staticMetaObject = {
    { &RpcRoutedClient::staticMetaObject, qt_meta_stringdata_RpcConfigClient.data,
      qt_meta_data_RpcConfigClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcConfigClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcConfigClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcConfigClient.stringdata0))
        return static_cast<void*>(const_cast< RpcConfigClient*>(this));
    if (!strcmp(_clname, "IRpcConfigClient"))
        return static_cast< IRpcConfigClient*>(const_cast< RpcConfigClient*>(this));
    return RpcRoutedClient::qt_metacast(_clname);
}

int RpcConfigClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RpcRoutedClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void RpcConfigClient::getStationListSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RpcConfigClient::getAtlantConfigSignal(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RpcConfigClient::connectionEstablishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
