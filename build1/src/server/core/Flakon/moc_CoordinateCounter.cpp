/****************************************************************************
** Meta object code from reading C++ file 'CoordinateCounter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/server/core/Flakon/CoordinateCounter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CoordinateCounter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CoordinateCounter_t {
    QByteArrayData data[6];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CoordinateCounter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CoordinateCounter_t qt_meta_stringdata_CoordinateCounter = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CoordinateCounter"
QT_MOC_LITERAL(1, 18, 14), // "signalFinished"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 24), // "signalSetCenterFrequency"
QT_MOC_LITERAL(4, 59, 9), // "frequency"
QT_MOC_LITERAL(5, 69, 22) // "slotSetCenterFrequency"

    },
    "CoordinateCounter\0signalFinished\0\0"
    "signalSetCenterFrequency\0frequency\0"
    "slotSetCenterFrequency"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CoordinateCounter[] = {

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
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    4,

       0        // eod
};

void CoordinateCounter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CoordinateCounter *_t = static_cast<CoordinateCounter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalFinished(); break;
        case 1: _t->signalSetCenterFrequency((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 2: _t->slotSetCenterFrequency((*reinterpret_cast< const double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CoordinateCounter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoordinateCounter::signalFinished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CoordinateCounter::*_t)(const double & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoordinateCounter::signalSetCenterFrequency)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CoordinateCounter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CoordinateCounter.data,
      qt_meta_data_CoordinateCounter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CoordinateCounter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CoordinateCounter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CoordinateCounter.stringdata0))
        return static_cast<void*>(const_cast< CoordinateCounter*>(this));
    if (!strcmp(_clname, "ITcpListener"))
        return static_cast< ITcpListener*>(const_cast< CoordinateCounter*>(this));
    if (!strcmp(_clname, "BaseSubject<ITcpListener>"))
        return static_cast< BaseSubject<ITcpListener>*>(const_cast< CoordinateCounter*>(this));
    if (!strcmp(_clname, "ICoordinateCounter"))
        return static_cast< ICoordinateCounter*>(const_cast< CoordinateCounter*>(this));
    return QObject::qt_metacast(_clname);
}

int CoordinateCounter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CoordinateCounter::signalFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CoordinateCounter::signalSetCenterFrequency(const double & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
