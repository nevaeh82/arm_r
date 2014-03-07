/****************************************************************************
** Meta object code from reading C++ file 'NIIPPController.h'
**
** Created: Thu 6. Mar 17:47:31 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/TCP/NIIPP/NIIPPController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NIIPPController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NIIPPController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   58,   16,   16, 0x08,
     105,   17,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NIIPPController[] = {
    "NIIPPController\0\0data\0"
    "signalSendToEncodeBpla(QByteArray*)\0"
    "msg_ptr\0_slotGetData(QSharedPointer<IMessage>)\0"
    "_slot_encode(QByteArray*)\0"
};

const QMetaObject NIIPPController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NIIPPController,
      qt_meta_data_NIIPPController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NIIPPController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NIIPPController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NIIPPController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NIIPPController))
        return static_cast<void*>(const_cast< NIIPPController*>(this));
    if (!strcmp(_clname, "IClient"))
        return static_cast< IClient*>(const_cast< NIIPPController*>(this));
    return QObject::qt_metacast(_clname);
}

int NIIPPController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalSendToEncodeBpla((*reinterpret_cast< QByteArray*(*)>(_a[1]))); break;
        case 1: _slotGetData((*reinterpret_cast< QSharedPointer<IMessage>(*)>(_a[1]))); break;
        case 2: _slot_encode((*reinterpret_cast< QByteArray*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NIIPPController::signalSendToEncodeBpla(QByteArray * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
