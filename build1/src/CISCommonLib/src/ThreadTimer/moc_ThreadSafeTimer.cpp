/****************************************************************************
** Meta object code from reading C++ file 'ThreadSafeTimer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/ThreadTimer/ThreadSafeTimer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ThreadSafeTimer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ThreadSafeTimer_t {
    QByteArrayData data[7];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThreadSafeTimer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThreadSafeTimer_t qt_meta_stringdata_ThreadSafeTimer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ThreadSafeTimer"
QT_MOC_LITERAL(1, 16, 11), // "startSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10), // "stopSignal"
QT_MOC_LITERAL(4, 40, 9), // "startSlot"
QT_MOC_LITERAL(5, 50, 4), // "msec"
QT_MOC_LITERAL(6, 55, 8) // "stopSlot"

    },
    "ThreadSafeTimer\0startSignal\0\0stopSignal\0"
    "startSlot\0msec\0stopSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThreadSafeTimer[] = {

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
       1,    0,   47,    2, 0x06 /* Public */,
       3,    0,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   49,    2, 0x08 /* Private */,
       4,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ThreadSafeTimer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThreadSafeTimer *_t = static_cast<ThreadSafeTimer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->startSignal(); break;
        case 2: _t->stopSignal(); break;
        case 3: _t->startSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->startSlot(); break;
        case 5: _t->stopSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ThreadSafeTimer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadSafeTimer::startSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ThreadSafeTimer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadSafeTimer::startSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ThreadSafeTimer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThreadSafeTimer::stopSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject ThreadSafeTimer::staticMetaObject = {
    { &QTimer::staticMetaObject, qt_meta_stringdata_ThreadSafeTimer.data,
      qt_meta_data_ThreadSafeTimer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ThreadSafeTimer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThreadSafeTimer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ThreadSafeTimer.stringdata0))
        return static_cast<void*>(const_cast< ThreadSafeTimer*>(this));
    return QTimer::qt_metacast(_clname);
}

int ThreadSafeTimer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTimer::qt_metacall(_c, _id, _a);
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
void ThreadSafeTimer::startSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ThreadSafeTimer::startSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void ThreadSafeTimer::stopSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
