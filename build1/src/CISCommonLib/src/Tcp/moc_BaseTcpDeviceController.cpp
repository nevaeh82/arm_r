/****************************************************************************
** Meta object code from reading C++ file 'BaseTcpDeviceController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Tcp/BaseTcpDeviceController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseTcpDeviceController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseTcpDeviceController_t {
    QByteArrayData data[4];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseTcpDeviceController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseTcpDeviceController_t qt_meta_stringdata_BaseTcpDeviceController = {
    {
QT_MOC_LITERAL(0, 0, 23), // "BaseTcpDeviceController"
QT_MOC_LITERAL(1, 24, 30), // "signalTcpDeviceConnectedToHost"
QT_MOC_LITERAL(2, 55, 0), // ""
QT_MOC_LITERAL(3, 56, 6) // "status"

    },
    "BaseTcpDeviceController\0"
    "signalTcpDeviceConnectedToHost\0\0status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseTcpDeviceController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void BaseTcpDeviceController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaseTcpDeviceController *_t = static_cast<BaseTcpDeviceController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalTcpDeviceConnectedToHost((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaseTcpDeviceController::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaseTcpDeviceController::signalTcpDeviceConnectedToHost)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BaseTcpDeviceController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseTcpDeviceController.data,
      qt_meta_data_BaseTcpDeviceController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseTcpDeviceController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseTcpDeviceController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseTcpDeviceController.stringdata0))
        return static_cast<void*>(const_cast< BaseTcpDeviceController*>(this));
    if (!strcmp(_clname, "ITcpDeviceController"))
        return static_cast< ITcpDeviceController*>(const_cast< BaseTcpDeviceController*>(this));
    if (!strcmp(_clname, "ITcpReceiver"))
        return static_cast< ITcpReceiver*>(const_cast< BaseTcpDeviceController*>(this));
    if (!strcmp(_clname, "BaseSubject<ITcpListener>"))
        return static_cast< BaseSubject<ITcpListener>*>(const_cast< BaseTcpDeviceController*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseTcpDeviceController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void BaseTcpDeviceController::signalTcpDeviceConnectedToHost(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
