/****************************************************************************
** Meta object code from reading C++ file 'qxtabstractconnectionmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Qxt/qxtabstractconnectionmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxtabstractconnectionmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QxtAbstractConnectionManager_t {
    QByteArrayData data[8];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxtAbstractConnectionManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxtAbstractConnectionManager_t qt_meta_stringdata_QxtAbstractConnectionManager = {
    {
QT_MOC_LITERAL(0, 0, 28), // "QxtAbstractConnectionManager"
QT_MOC_LITERAL(1, 29, 13), // "newConnection"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 10), // "QIODevice*"
QT_MOC_LITERAL(4, 55, 6), // "device"
QT_MOC_LITERAL(5, 62, 8), // "clientID"
QT_MOC_LITERAL(6, 71, 12), // "disconnected"
QT_MOC_LITERAL(7, 84, 10) // "disconnect"

    },
    "QxtAbstractConnectionManager\0newConnection\0"
    "\0QIODevice*\0device\0clientID\0disconnected\0"
    "disconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxtAbstractConnectionManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       6,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::ULongLong,    5,

       0        // eod
};

void QxtAbstractConnectionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QxtAbstractConnectionManager *_t = static_cast<QxtAbstractConnectionManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newConnection((*reinterpret_cast< QIODevice*(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 1: _t->disconnected((*reinterpret_cast< QIODevice*(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2]))); break;
        case 2: _t->disconnect((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QxtAbstractConnectionManager::*_t)(QIODevice * , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtAbstractConnectionManager::newConnection)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QxtAbstractConnectionManager::*_t)(QIODevice * , quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtAbstractConnectionManager::disconnected)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QxtAbstractConnectionManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QxtAbstractConnectionManager.data,
      qt_meta_data_QxtAbstractConnectionManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QxtAbstractConnectionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxtAbstractConnectionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QxtAbstractConnectionManager.stringdata0))
        return static_cast<void*>(const_cast< QxtAbstractConnectionManager*>(this));
    return QObject::qt_metacast(_clname);
}

int QxtAbstractConnectionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void QxtAbstractConnectionManager::newConnection(QIODevice * _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QxtAbstractConnectionManager::disconnected(QIODevice * _t1, quint64 _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
