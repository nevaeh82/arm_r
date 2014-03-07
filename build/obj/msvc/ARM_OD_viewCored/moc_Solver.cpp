/****************************************************************************
** Meta object code from reading C++ file 'Solver.h'
**
** Created: Thu 6. Mar 17:46:31 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/Tabs/Solver/Solver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Solver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Solver[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      23,    7,    7,    7, 0x08,
      44,   38,    7,    7, 0x08,
      61,    7,    7,    7, 0x08,
      81,   75,    7,    7, 0x08,
     104,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Solver[] = {
    "Solver\0\0_slot_accept()\0_slot_cancel()\0"
    "state\0_slot_auto(bool)\0_slot_clear()\0"
    "count\0_slot_count_track(int)\0slot_show()\0"
};

const QMetaObject Solver::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Solver,
      qt_meta_data_Solver, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Solver::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Solver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Solver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Solver))
        return static_cast<void*>(const_cast< Solver*>(this));
    return QWidget::qt_metacast(_clname);
}

int Solver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _slot_accept(); break;
        case 1: _slot_cancel(); break;
        case 2: _slot_auto((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _slot_clear(); break;
        case 4: _slot_count_track((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: slot_show(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
