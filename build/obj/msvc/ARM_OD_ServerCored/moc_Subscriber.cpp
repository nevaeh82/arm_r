/****************************************************************************
** Meta object code from reading C++ file 'Subscriber.h'
**
** Created: Thu 6. Mar 17:47:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/Common/Subscriber.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Subscriber.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Subscriber[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,   12,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Subscriber[] = {
    "Subscriber\0\0type,msg_ptr\0"
    "signalGetData(int,rpc_msg)\0"
    "_slotGetData(int,rpc_msg)\0"
};

const QMetaObject Subscriber::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Subscriber,
      qt_meta_data_Subscriber, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Subscriber::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Subscriber::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Subscriber::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Subscriber))
        return static_cast<void*>(const_cast< Subscriber*>(this));
    if (!strcmp(_clname, "ISubscriber"))
        return static_cast< ISubscriber*>(const_cast< Subscriber*>(this));
    return QObject::qt_metacast(_clname);
}

int Subscriber::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalGetData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< rpc_msg(*)>(_a[2]))); break;
        case 1: _slotGetData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< rpc_msg(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Subscriber::signalGetData(int _t1, rpc_msg _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
