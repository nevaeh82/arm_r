/****************************************************************************
** Meta object code from reading C++ file 'Ais.h'
**
** Created: Thu 6. Mar 17:47:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/AIS/Ais.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Ais.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ais[] = {

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
       5,    4,    4,    4, 0x05,

 // slots: signature, parameters, type, tag, flags
      16,    4,    4,    4, 0x0a,
      23,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Ais[] = {
    "Ais\0\0finished()\0stop()\0_slot_update()\0"
};

const QMetaObject Ais::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Ais,
      qt_meta_data_Ais, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ais::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ais::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ais::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ais))
        return static_cast<void*>(const_cast< Ais*>(this));
    return QObject::qt_metacast(_clname);
}

int Ais::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        case 1: stop(); break;
        case 2: _slot_update(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Ais::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
