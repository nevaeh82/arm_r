/****************************************************************************
** Meta object code from reading C++ file 'BaseTcpServerClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Tcp/BaseTcpServerClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseTcpServerClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseTcpServerClient_t {
    QByteArrayData data[12];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseTcpServerClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseTcpServerClient_t qt_meta_stringdata_BaseTcpServerClient = {
    {
QT_MOC_LITERAL(0, 0, 19), // "BaseTcpServerClient"
QT_MOC_LITERAL(1, 20, 23), // "writeDataInternalSignal"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 4), // "data"
QT_MOC_LITERAL(4, 50, 17), // "onSocketConnected"
QT_MOC_LITERAL(5, 68, 20), // "onSocketDisconnected"
QT_MOC_LITERAL(6, 89, 17), // "onSocketReadyRead"
QT_MOC_LITERAL(7, 107, 20), // "onSocketDisplayError"
QT_MOC_LITERAL(8, 128, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(9, 157, 11), // "socketError"
QT_MOC_LITERAL(10, 169, 30), // "disconnectFromHostInternalSlot"
QT_MOC_LITERAL(11, 200, 21) // "writeDataInternalSlot"

    },
    "BaseTcpServerClient\0writeDataInternalSignal\0"
    "\0data\0onSocketConnected\0onSocketDisconnected\0"
    "onSocketReadyRead\0onSocketDisplayError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "disconnectFromHostInternalSlot\0"
    "writeDataInternalSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseTcpServerClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   52,    2, 0x08 /* Private */,
       5,    0,   53,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    1,   55,    2, 0x08 /* Private */,
      10,    0,   58,    2, 0x08 /* Private */,
      11,    1,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    3,

       0        // eod
};

void BaseTcpServerClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseTcpServerClient *_t = static_cast<BaseTcpServerClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->writeDataInternalSignal((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->onSocketConnected(); break;
        case 2: _t->onSocketDisconnected(); break;
        case 3: _t->onSocketReadyRead(); break;
        case 4: _t->onSocketDisplayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->disconnectFromHostInternalSlot(); break;
        case 6: _t->writeDataInternalSlot((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaseTcpServerClient::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseTcpServerClient::writeDataInternalSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BaseTcpServerClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseTcpServerClient.data,
      qt_meta_data_BaseTcpServerClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseTcpServerClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseTcpServerClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseTcpServerClient.stringdata0))
        return static_cast<void*>(const_cast< BaseTcpServerClient*>(this));
    if (!strcmp(_clname, "ITcpServerClient"))
        return static_cast< ITcpServerClient*>(const_cast< BaseTcpServerClient*>(this));
    if (!strcmp(_clname, "BaseSubject<ITcpReceiver>"))
        return static_cast< BaseSubject<ITcpReceiver>*>(const_cast< BaseTcpServerClient*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseTcpServerClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void BaseTcpServerClient::writeDataInternalSignal(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
