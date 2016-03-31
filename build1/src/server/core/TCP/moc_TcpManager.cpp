/****************************************************************************
** Meta object code from reading C++ file 'TcpManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/server/core/TCP/TcpManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TcpManager_t {
    QByteArrayData data[11];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpManager_t qt_meta_stringdata_TcpManager = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TcpManager"
QT_MOC_LITERAL(1, 11, 21), // "threadTerminateSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 33), // "threadTerminateSignalForMapSo..."
QT_MOC_LITERAL(4, 68, 28), // "onMethodCalledInternalSignal"
QT_MOC_LITERAL(5, 97, 6), // "method"
QT_MOC_LITERAL(6, 104, 8), // "argument"
QT_MOC_LITERAL(7, 113, 26), // "onMethodCalledInternalSlot"
QT_MOC_LITERAL(8, 140, 16), // "emulateBplaPoint"
QT_MOC_LITERAL(9, 157, 13), // "IRpcListener*"
QT_MOC_LITERAL(10, 171, 6) // "sender"

    },
    "TcpManager\0threadTerminateSignal\0\0"
    "threadTerminateSignalForMapSolver\0"
    "onMethodCalledInternalSignal\0method\0"
    "argument\0onMethodCalledInternalSlot\0"
    "emulateBplaPoint\0IRpcListener*\0sender"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpManager[] = {

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
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    2,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   51,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x08 /* Private */,
       8,    0,   59,    2, 0x28 /* Private | MethodCloned */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    5,    6,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

       0        // eod
};

void TcpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TcpManager *_t = static_cast<TcpManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->threadTerminateSignal(); break;
        case 1: _t->threadTerminateSignalForMapSolver(); break;
        case 2: _t->onMethodCalledInternalSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 3: _t->onMethodCalledInternalSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 4: _t->emulateBplaPoint((*reinterpret_cast< IRpcListener*(*)>(_a[1]))); break;
        case 5: _t->emulateBplaPoint(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TcpManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpManager::threadTerminateSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TcpManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpManager::threadTerminateSignalForMapSolver)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TcpManager::*_t)(const QString & , const QVariant & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpManager::onMethodCalledInternalSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject TcpManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TcpManager.data,
      qt_meta_data_TcpManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TcpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TcpManager.stringdata0))
        return static_cast<void*>(const_cast< TcpManager*>(this));
    if (!strcmp(_clname, "ITcpManager"))
        return static_cast< ITcpManager*>(const_cast< TcpManager*>(this));
    if (!strcmp(_clname, "ITcpListener"))
        return static_cast< ITcpListener*>(const_cast< TcpManager*>(this));
    if (!strcmp(_clname, "IRpcListener"))
        return static_cast< IRpcListener*>(const_cast< TcpManager*>(this));
    return QObject::qt_metacast(_clname);
}

int TcpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void TcpManager::threadTerminateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void TcpManager::threadTerminateSignalForMapSolver()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void TcpManager::onMethodCalledInternalSignal(const QString & _t1, const QVariant & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
