/****************************************************************************
** Meta object code from reading C++ file 'mg1_label_instrument_item.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics/labels/mg1_label_instrument_item.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mg1_label_instrument_item.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_gui__mg1__LabelInstrumentItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      53,   31,   30,   30, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_gui__mg1__LabelInstrumentItem[] = {
    "gui::mg1::LabelInstrumentItem\0\0"
    "item_text,scene_value\0"
    "ItemClicked(QString,DoubleScene_MG1)\0"
};

void gui::mg1::LabelInstrumentItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LabelInstrumentItem *_t = static_cast<LabelInstrumentItem *>(_o);
        switch (_id) {
        case 0: _t->ItemClicked((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< DoubleScene_MG1(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData gui::mg1::LabelInstrumentItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject gui::mg1::LabelInstrumentItem::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_gui__mg1__LabelInstrumentItem,
      qt_meta_data_gui__mg1__LabelInstrumentItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gui::mg1::LabelInstrumentItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gui::mg1::LabelInstrumentItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gui::mg1::LabelInstrumentItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gui__mg1__LabelInstrumentItem))
        return static_cast<void*>(const_cast< LabelInstrumentItem*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int gui::mg1::LabelInstrumentItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void gui::mg1::LabelInstrumentItem::ItemClicked(QString _t1, DoubleScene_MG1 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_gui__mg1__LabelInstrumentUnion[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_gui__mg1__LabelInstrumentUnion[] = {
    "gui::mg1::LabelInstrumentUnion\0"
};

void gui::mg1::LabelInstrumentUnion::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData gui::mg1::LabelInstrumentUnion::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject gui::mg1::LabelInstrumentUnion::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_gui__mg1__LabelInstrumentUnion,
      qt_meta_data_gui__mg1__LabelInstrumentUnion, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gui::mg1::LabelInstrumentUnion::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gui::mg1::LabelInstrumentUnion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gui::mg1::LabelInstrumentUnion::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gui__mg1__LabelInstrumentUnion))
        return static_cast<void*>(const_cast< LabelInstrumentUnion*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int gui::mg1::LabelInstrumentUnion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
