/****************************************************************************
** Meta object code from reading C++ file 'qxtrpcpeer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Qxt/qxtrpcpeer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxtrpcpeer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QxtRPCPeer_t {
    QByteArrayData data[6];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxtRPCPeer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxtRPCPeer_t qt_meta_stringdata_QxtRPCPeer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QxtRPCPeer"
QT_MOC_LITERAL(1, 11, 17), // "connectedToServer"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 22), // "disconnectedFromServer"
QT_MOC_LITERAL(4, 53, 11), // "serverError"
QT_MOC_LITERAL(5, 65, 28) // "QAbstractSocket::SocketError"

    },
    "QxtRPCPeer\0connectedToServer\0\0"
    "disconnectedFromServer\0serverError\0"
    "QAbstractSocket::SocketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxtRPCPeer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    1,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,

       0        // eod
};

void QxtRPCPeer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QxtRPCPeer *_t = static_cast<QxtRPCPeer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectedToServer(); break;
        case 1: _t->disconnectedFromServer(); break;
        case 2: _t->serverError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QxtRPCPeer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtRPCPeer::connectedToServer)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QxtRPCPeer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtRPCPeer::disconnectedFromServer)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QxtRPCPeer::*_t)(QAbstractSocket::SocketError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtRPCPeer::serverError)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QxtRPCPeer::staticMetaObject = {
    { &QxtRPCService::staticMetaObject, qt_meta_stringdata_QxtRPCPeer.data,
      qt_meta_data_QxtRPCPeer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QxtRPCPeer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxtRPCPeer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QxtRPCPeer.stringdata0))
        return static_cast<void*>(const_cast< QxtRPCPeer*>(this));
    return QxtRPCService::qt_metacast(_clname);
}

int QxtRPCPeer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QxtRPCService::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QxtRPCPeer::connectedToServer()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QxtRPCPeer::disconnectedFromServer()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QxtRPCPeer::serverError(QAbstractSocket::SocketError _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
