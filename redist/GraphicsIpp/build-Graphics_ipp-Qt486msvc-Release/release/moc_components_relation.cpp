/****************************************************************************
** Meta object code from reading C++ file 'components_relation.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics/components_relation.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'components_relation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ComponentsRelation[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      56,   20,   19,   19, 0x08,
     142,  112,   19,   19, 0x08,
     225,  198,   19,   19, 0x08,
     296,  280,   19,   19, 0x08,
     342,  280,   19,   19, 0x08,
     400,  388,   19,   19, 0x08,
     458,  388,   19,   19, 0x08,
     516,  388,   19,   19, 0x08,
     574,  388,   19,   19, 0x08,
     632,   19,   19,   19, 0x08,
     665,   19,   19,   19, 0x08,
     698,   19,   19,   19, 0x08,
     728,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ComponentsRelation[] = {
    "ComponentsRelation\0\0"
    "start_hz,start_smpl,end_hz,end_smpl\0"
    "my_SonogramViewPortChanged(double,double,double,double)\0"
    "start_hz,start_Y,end_hz,end_Y\0"
    "my_SpectrumViewPortChanged(double,double,double,double)\0"
    "mean_spectrum,max_spectrum\0"
    "my_SonogramScreenSpectrumsChanged(FFTLine_32f,FFTLine)\0"
    "Mark,isVertical\0"
    "my_SonogramMarkerChange(QVector<double>,bool)\0"
    "my_SpectrumMarkerChange(QVector<double>,bool)\0"
    "x1,y1,x2,y2\0"
    "my_SonogramSelectionChangeTo(double,double,double,double)\0"
    "my_SpectrumSelectionChangeTo(double,double,double,double)\0"
    "my_SpectrumSelectionFinished(double,double,double,double)\0"
    "my_SonorgamSelectionFinished(double,double,double,double)\0"
    "my_SonorgamDeselectionFinished()\0"
    "my_SpectrumDeselectionFinished()\0"
    "my_SpectrumSelectionCleared()\0"
    "my_SonogramSelectionCleared()\0"
};

void ComponentsRelation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ComponentsRelation *_t = static_cast<ComponentsRelation *>(_o);
        switch (_id) {
        case 0: _t->my_SonogramViewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 1: _t->my_SpectrumViewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 2: _t->my_SonogramScreenSpectrumsChanged((*reinterpret_cast< FFTLine_32f(*)>(_a[1])),(*reinterpret_cast< FFTLine(*)>(_a[2]))); break;
        case 3: _t->my_SonogramMarkerChange((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->my_SpectrumMarkerChange((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->my_SonogramSelectionChangeTo((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 6: _t->my_SpectrumSelectionChangeTo((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 7: _t->my_SpectrumSelectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 8: _t->my_SonorgamSelectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 9: _t->my_SonorgamDeselectionFinished(); break;
        case 10: _t->my_SpectrumDeselectionFinished(); break;
        case 11: _t->my_SpectrumSelectionCleared(); break;
        case 12: _t->my_SonogramSelectionCleared(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ComponentsRelation::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ComponentsRelation::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ComponentsRelation,
      qt_meta_data_ComponentsRelation, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ComponentsRelation::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ComponentsRelation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ComponentsRelation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ComponentsRelation))
        return static_cast<void*>(const_cast< ComponentsRelation*>(this));
    return QObject::qt_metacast(_clname);
}

int ComponentsRelation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
