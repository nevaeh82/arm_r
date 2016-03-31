/****************************************************************************
** Meta object code from reading C++ file 'qxtrpcservice_p.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Qxt/qxtrpcservice_p.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxtrpcservice_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QxtRPCServicePrivate_t {
    QByteArrayData data[9];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxtRPCServicePrivate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxtRPCServicePrivate_t qt_meta_stringdata_QxtRPCServicePrivate = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QxtRPCServicePrivate"
QT_MOC_LITERAL(1, 21, 15), // "clientConnected"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 10), // "QIODevice*"
QT_MOC_LITERAL(4, 49, 3), // "dev"
QT_MOC_LITERAL(5, 53, 2), // "id"
QT_MOC_LITERAL(6, 56, 18), // "clientDisconnected"
QT_MOC_LITERAL(7, 75, 10), // "clientData"
QT_MOC_LITERAL(8, 86, 10) // "serverData"

    },
    "QxtRPCServicePrivate\0clientConnected\0"
    "\0QIODevice*\0dev\0id\0clientDisconnected\0"
    "clientData\0serverData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxtRPCServicePrivate[] = {

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
       1,    2,   34,    2, 0x0a /* Public */,
       6,    2,   39,    2, 0x0a /* Public */,
       7,    1,   44,    2, 0x0a /* Public */,
       8,    0,   47,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong,    4,    5,
    QMetaType::Void, QMetaType::ULongLong,    5,
    QMetaType::Void,

       0        // eod
};

void QxtRPCServicePrivate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QxtRPCServicePrivate *_t = static_cast<QxtRPCServicePrivate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clientConnected((*reinterpret_cast< QIODevice*(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 1: _t->clientDisconnected((*reinterpret_cast< QIODevice*(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 2: _t->clientData((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 3: _t->serverData(); break;
        default: ;
        }
    }
}

const QMetaObject QxtRPCServicePrivate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QxtRPCServicePrivate.data,
      qt_meta_data_QxtRPCServicePrivate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QxtRPCServicePrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxtRPCServicePrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QxtRPCServicePrivate.stringdata0))
        return static_cast<void*>(const_cast< QxtRPCServicePrivate*>(this));
    if (!strcmp(_clname, "QxtPrivate<QxtRPCService>"))
        return static_cast< QxtPrivate<QxtRPCService>*>(const_cast< QxtRPCServicePrivate*>(this));
    return QObject::qt_metacast(_clname);
}

int QxtRPCServicePrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
