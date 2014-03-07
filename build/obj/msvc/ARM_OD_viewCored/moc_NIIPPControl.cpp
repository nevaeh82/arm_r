/****************************************************************************
** Meta object code from reading C++ file 'NIIPPControl.h'
**
** Created: Thu 6. Mar 17:46:28 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/NIIPP/NIIPPControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NIIPPControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NIIPPControl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      42,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   68,   13,   13, 0x08,
     111,  105,   13,   13, 0x08,
     141,  105,   13,   13, 0x08,
     168,  105,   13,   13, 0x08,
     191,   68,   13,   13, 0x08,
     218,   13,   13,   13, 0x08,
     232,  105,   13,   13, 0x0a,
     250,   68,   13,   13, 0x0a,
     270,  105,   13,   13, 0x0a,
     296,   13,  292,   13, 0x0a,
     311,  305,   13,   13, 0x0a,
     360,  330,   13,   13, 0x0a,
     407,  401,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NIIPPControl[] = {
    "NIIPPControl\0\0signalChangeValuePower(int)\0"
    "signalChangeAngle(double)\0state\0"
    "_slot_start_stop_clicked(bool)\0value\0"
    "_slot_change_value_power(int)\0"
    "_slot_change_angel(double)\0"
    "_slot_change_mode(int)\0"
    "_slot_enable_complex(bool)\0_slot_clear()\0"
    "set_power(double)\0set_switch_on(bool)\0"
    "set_antenna_type(int)\0int\0get_id()\0"
    "coord\0set_point(QPointF)\0"
    "point,point_uvoda,alt,bearing\0"
    "send_evil(QPointF,QPointF,double,double)\0"
    "angle\0set_angle(double)\0"
};

const QMetaObject NIIPPControl::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_NIIPPControl,
      qt_meta_data_NIIPPControl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NIIPPControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NIIPPControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NIIPPControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NIIPPControl))
        return static_cast<void*>(const_cast< NIIPPControl*>(this));
    if (!strcmp(_clname, "INiiPPController"))
        return static_cast< INiiPPController*>(const_cast< NIIPPControl*>(this));
    return QWidget::qt_metacast(_clname);
}

int NIIPPControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalChangeValuePower((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: signalChangeAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _slot_start_stop_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _slot_change_value_power((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _slot_change_angel((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _slot_change_mode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _slot_enable_complex((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _slot_clear(); break;
        case 8: set_power((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: set_switch_on((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: set_antenna_type((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: { int _r = get_id();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: set_point((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 13: send_evil((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 14: set_angle((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void NIIPPControl::signalChangeValuePower(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NIIPPControl::signalChangeAngle(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
