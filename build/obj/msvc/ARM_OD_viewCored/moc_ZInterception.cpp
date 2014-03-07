/****************************************************************************
** Meta object code from reading C++ file 'ZInterception.h'
**
** Created: Thu 6. Mar 17:46:34 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/UAV/ZInterception.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ZInterception.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ZInterception[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      64,   15,   14,   14, 0x05,
     130,   14,   14,   14, 0x05,
     161,  141,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     202,  141,   14,   14, 0x08,
     296,  243,   14,   14, 0x0a,
     386,  335,   14,   14, 0x0a,
     424,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ZInterception[] = {
    "ZInterception\0\0"
    "aCoord,aHgt,aRadius,aTime,aIntcCourse,aIntcSpeed\0"
    "signal_sendInterceptionPoint(QPointF,float,float,int,float,float)\0"
    "finished()\0bla,bpla,per,target\0"
    "signalSet(int,int,QByteArray,QByteArray)\0"
    "_slot_set(int,int,QByteArray,QByteArray)\0"
    "aCoordCurrentIntc,aHgtCurrentIntc,aMaxSpd,aMaxHgtSpd\0"
    "getIntcData(QPointF,float,float,float)\0"
    "aCoordCurrentAim,aHgtCurrentAim,aSpdAim,aCourseAim\0"
    "getAimData(QPointF,float,float,float)\0"
    "mainProcessing()\0"
};

const QMetaObject ZInterception::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ZInterception,
      qt_meta_data_ZInterception, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ZInterception::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ZInterception::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ZInterception::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ZInterception))
        return static_cast<void*>(const_cast< ZInterception*>(this));
    return QObject::qt_metacast(_clname);
}

int ZInterception::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_sendInterceptionPoint((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< float(*)>(_a[6]))); break;
        case 1: finished(); break;
        case 2: signalSet((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4]))); break;
        case 3: _slot_set((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4]))); break;
        case 4: getIntcData((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 5: getAimData((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 6: mainProcessing(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ZInterception::signal_sendInterceptionPoint(QPointF _t1, float _t2, float _t3, int _t4, float _t5, float _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ZInterception::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ZInterception::signalSet(int _t1, int _t2, QByteArray _t3, QByteArray _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
