/****************************************************************************
** Meta object code from reading C++ file 'BaseTcpServerController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Tcp/BaseTcpServerController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseTcpServerController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BaseTcpServerController_t {
    QByteArrayData data[1];
    char stringdata0[24];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseTcpServerController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseTcpServerController_t qt_meta_stringdata_BaseTcpServerController = {
    {
QT_MOC_LITERAL(0, 0, 23) // "BaseTcpServerController"

    },
    "BaseTcpServerController"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseTcpServerController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void BaseTcpServerController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject BaseTcpServerController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BaseTcpServerController.data,
      qt_meta_data_BaseTcpServerController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BaseTcpServerController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseTcpServerController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BaseTcpServerController.stringdata0))
        return static_cast<void*>(const_cast< BaseTcpServerController*>(this));
    if (!strcmp(_clname, "ITcpServerController"))
        return static_cast< ITcpServerController*>(const_cast< BaseTcpServerController*>(this));
    if (!strcmp(_clname, "ITcpReceiver"))
        return static_cast< ITcpReceiver*>(const_cast< BaseTcpServerController*>(this));
    if (!strcmp(_clname, "BaseSubject<ITcpListener>"))
        return static_cast< BaseSubject<ITcpListener>*>(const_cast< BaseTcpServerController*>(this));
    return QObject::qt_metacast(_clname);
}

int BaseTcpServerController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
