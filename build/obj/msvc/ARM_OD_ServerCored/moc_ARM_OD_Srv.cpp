/****************************************************************************
** Meta object code from reading C++ file 'ARM_OD_Srv.h'
**
** Created: Thu 6. Mar 17:47:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/ARM_OD_Srv.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ARM_OD_Srv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ARM_OD_Srv[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      29,   11,   11,   11, 0x05,
      45,   11,   11,   11, 0x05,
      63,   11,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ARM_OD_Srv[] = {
    "ARM_OD_Srv\0\0signalStartRPC()\0"
    "signalStopRPC()\0signalFinishRPC()\0"
    "closeALL()\0"
};

const QMetaObject ARM_OD_Srv::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ARM_OD_Srv,
      qt_meta_data_ARM_OD_Srv, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ARM_OD_Srv::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ARM_OD_Srv::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ARM_OD_Srv::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ARM_OD_Srv))
        return static_cast<void*>(const_cast< ARM_OD_Srv*>(this));
    return QObject::qt_metacast(_clname);
}

int ARM_OD_Srv::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalStartRPC(); break;
        case 1: signalStopRPC(); break;
        case 2: signalFinishRPC(); break;
        case 3: closeALL(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ARM_OD_Srv::signalStartRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ARM_OD_Srv::signalStopRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ARM_OD_Srv::signalFinishRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ARM_OD_Srv::closeALL()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
