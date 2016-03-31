/****************************************************************************
** Meta object code from reading C++ file 'RpcRoutedServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Rpc/RpcRoutedServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcRoutedServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcRoutedServer_t {
    QByteArrayData data[11];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcRoutedServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcRoutedServer_t qt_meta_stringdata_RpcRoutedServer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RpcRoutedServer"
QT_MOC_LITERAL(1, 16, 12), // "internalCall"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 6), // "method"
QT_MOC_LITERAL(4, 37, 8), // "argument"
QT_MOC_LITERAL(5, 46, 13), // "IRpcListener*"
QT_MOC_LITERAL(6, 60, 6), // "sender"
QT_MOC_LITERAL(7, 67, 14), // "registerClient"
QT_MOC_LITERAL(8, 82, 8), // "clientId"
QT_MOC_LITERAL(9, 91, 7), // "routeId"
QT_MOC_LITERAL(10, 99, 16) // "deregisterClient"

    },
    "RpcRoutedServer\0internalCall\0\0method\0"
    "argument\0IRpcListener*\0sender\0"
    "registerClient\0clientId\0routeId\0"
    "deregisterClient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcRoutedServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x09 /* Protected */,
       1,    2,   41,    2, 0x29 /* Protected | MethodCloned */,
       7,    2,   46,    2, 0x09 /* Protected */,
      10,    1,   51,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    4,
    QMetaType::Bool, QMetaType::ULongLong, QMetaType::UInt,    8,    9,
    QMetaType::Void, QMetaType::ULongLong,    8,

       0        // eod
};

void RpcRoutedServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcRoutedServer *_t = static_cast<RpcRoutedServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->internalCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< IRpcListener*(*)>(_a[3]))); break;
        case 1: _t->internalCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 2: { bool _r = _t->registerClient((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->deregisterClient((*reinterpret_cast< const quint64(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject RpcRoutedServer::staticMetaObject = {
    { &RpcServerBase::staticMetaObject, qt_meta_stringdata_RpcRoutedServer.data,
      qt_meta_data_RpcRoutedServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcRoutedServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcRoutedServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcRoutedServer.stringdata0))
        return static_cast<void*>(const_cast< RpcRoutedServer*>(this));
    return RpcServerBase::qt_metacast(_clname);
}

int RpcRoutedServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RpcServerBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
