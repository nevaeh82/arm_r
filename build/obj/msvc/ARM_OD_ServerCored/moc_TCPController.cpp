/****************************************************************************
** Meta object code from reading C++ file 'TCPController.h'
**
** Created: Thu 6. Mar 17:47:30 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/TCP/TCPController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TCPController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TCPController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      34,   14,   14,   14, 0x05,
      55,   14,   14,   14, 0x05,
      74,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      97,   14,   14,   14, 0x0a,
     117,  113,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TCPController[] = {
    "TCPController\0\0error_connection()\0"
    "disconnectfromhost()\0add_error(QString)\0"
    "finished_connections()\0stopped_error()\0"
    "err\0errorHandler(QString)\0"
};

const QMetaObject TCPController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TCPController,
      qt_meta_data_TCPController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TCPController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TCPController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TCPController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TCPController))
        return static_cast<void*>(const_cast< TCPController*>(this));
    if (!strcmp(_clname, "ITCPController"))
        return static_cast< ITCPController*>(const_cast< TCPController*>(this));
    return QObject::qt_metacast(_clname);
}

int TCPController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: error_connection(); break;
        case 1: disconnectfromhost(); break;
        case 2: add_error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: finished_connections(); break;
        case 4: stopped_error(); break;
        case 5: errorHandler((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TCPController::error_connection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void TCPController::disconnectfromhost()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TCPController::add_error(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TCPController::finished_connections()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
