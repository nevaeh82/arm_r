/****************************************************************************
** Meta object code from reading C++ file 'TabMap.h'
**
** Created: Thu 6. Mar 17:46:30 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/Tabs/TabMap.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TabMap[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      25,    7,    7,    7, 0x05,
      41,    7,    7,    7, 0x05,
      66,   59,    7,    7, 0x05,
     113,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     138,   59,    7,    7, 0x08,
     188,  182,    7,    7, 0x08,
     233,  231,    7,    7, 0x08,
     281,    7,    7,    7, 0x0a,
     302,    7,    7,    7, 0x0a,
     324,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TabMap[] = {
    "TabMap\0\0signalStartRPC()\0signalStopRPC()\0"
    "signalFinishRPC()\0points\0"
    "signalGetPointsFromRPCFlakon(QVector<QPointF>)\0"
    "signalTreeBLAUpdate(int)\0"
    "_slot_get_points_from_rpc(QVector<QPointF>)\0"
    "index\0_slot_tree_bla_double_clicked(QModelIndex)\0"
    ",\0_slot_tree_BLA_Changed(QModelIndex,QModelIndex)\0"
    "slot_show_tree_BLA()\0slot_show_tree_BPLA()\0"
    "slot_show_niipp()\0"
};

const QMetaObject TabMap::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TabMap,
      qt_meta_data_TabMap, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TabMap::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TabMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TabMap::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabMap))
        return static_cast<void*>(const_cast< TabMap*>(this));
    if (!strcmp(_clname, "ISubModule"))
        return static_cast< ISubModule*>(const_cast< TabMap*>(this));
    if (!strcmp(_clname, "ITabMap"))
        return static_cast< ITabMap*>(const_cast< TabMap*>(this));
    return QWidget::qt_metacast(_clname);
}

int TabMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalStartRPC(); break;
        case 1: signalStopRPC(); break;
        case 2: signalFinishRPC(); break;
        case 3: signalGetPointsFromRPCFlakon((*reinterpret_cast< QVector<QPointF>(*)>(_a[1]))); break;
        case 4: signalTreeBLAUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _slot_get_points_from_rpc((*reinterpret_cast< QVector<QPointF>(*)>(_a[1]))); break;
        case 6: _slot_tree_bla_double_clicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 7: _slot_tree_BLA_Changed((*reinterpret_cast< QModelIndex(*)>(_a[1])),(*reinterpret_cast< QModelIndex(*)>(_a[2]))); break;
        case 8: slot_show_tree_BLA(); break;
        case 9: slot_show_tree_BPLA(); break;
        case 10: slot_show_niipp(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TabMap::signalStartRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void TabMap::signalStopRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TabMap::signalFinishRPC()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TabMap::signalGetPointsFromRPCFlakon(QVector<QPointF> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TabMap::signalTreeBLAUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
