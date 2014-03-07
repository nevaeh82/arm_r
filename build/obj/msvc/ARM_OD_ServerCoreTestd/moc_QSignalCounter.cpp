/****************************************************************************
** Meta object code from reading C++ file 'QSignalCounter.h'
**
** Created: Thu 6. Mar 17:47:59 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/TestShared/QSignalCounter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QSignalCounter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QSignalCounter[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      35,   26,   15,   15, 0x0a,
      48,   26,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QSignalCounter[] = {
    "QSignalCounter\0\0onEvent()\0argument\0"
    "onEvent(int)\0onEvent(QVariant)\0"
};

const QMetaObject QSignalCounter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QSignalCounter,
      qt_meta_data_QSignalCounter, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSignalCounter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSignalCounter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSignalCounter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSignalCounter))
        return static_cast<void*>(const_cast< QSignalCounter*>(this));
    return QObject::qt_metacast(_clname);
}

int QSignalCounter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onEvent(); break;
        case 1: onEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: onEvent((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
