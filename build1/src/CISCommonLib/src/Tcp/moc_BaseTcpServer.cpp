/****************************************************************************
** Meta object code from reading C++ file 'BaseTcpServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Tcp/BaseTcpServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseTcpServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseTcpServer_t {
    QByteArrayData data[6];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseTcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseTcpServer_t qt_meta_stringdata_BaseTcpServer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BaseTcpServer"
QT_MOC_LITERAL(1, 14, 15), // "newClientSignal"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 8), // "clientId"
QT_MOC_LITERAL(4, 40, 17), // "ITcpServerClient*"
QT_MOC_LITERAL(5, 58, 19) // "onNewConnectionSlot"

    },
    "BaseTcpServer\0newClientSignal\0\0clientId\0"
    "ITcpServerClient*\0onNewConnectionSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseTcpServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 4,    3,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void BaseTcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseTcpServer *_t = static_cast<BaseTcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newClientSignal((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ITcpServerClient*(*)>(_a[2]))); break;
        case 1: _t->onNewConnectionSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaseTcpServer::*_t)(uint , ITcpServerClient * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseTcpServer::newClientSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BaseTcpServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseTcpServer.data,
      qt_meta_data_BaseTcpServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseTcpServer.stringdata0))
        return static_cast<void*>(const_cast< BaseTcpServer*>(this));
    if (!strcmp(_clname, "ITcpServer"))
        return static_cast< ITcpServer*>(const_cast< BaseTcpServer*>(this));
    if (!strcmp(_clname, "ITcpReceiver"))
        return static_cast< ITcpReceiver*>(const_cast< BaseTcpServer*>(this));
    if (!strcmp(_clname, "BaseSubject<ITcpReceiver>"))
        return static_cast< BaseSubject<ITcpReceiver>*>(const_cast< BaseTcpServer*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void BaseTcpServer::newClientSignal(uint _t1, ITcpServerClient * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
