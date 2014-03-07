/****************************************************************************
** Meta object code from reading C++ file 'BLA.h'
**
** Created: Thu 6. Mar 17:47:31 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/TCP/UAV/BLA.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BLA.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BLA[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,    5,    4,    4, 0x0a,
      31,    5,    4,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BLA[] = {
    "BLA\0\0state\0_send_request(bool)\0"
    "_send_request2(bool)\0"
};

const QMetaObject BLA::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BLA,
      qt_meta_data_BLA, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BLA::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BLA::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BLA::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BLA))
        return static_cast<void*>(const_cast< BLA*>(this));
    return QObject::qt_metacast(_clname);
}

int BLA::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _send_request((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _send_request2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
