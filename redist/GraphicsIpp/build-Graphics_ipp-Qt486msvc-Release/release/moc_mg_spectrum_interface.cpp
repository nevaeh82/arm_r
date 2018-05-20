/****************************************************************************
** Meta object code from reading C++ file 'mg_spectrum_interface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mg_spectrum_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Q_MG_SpectrumInterface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   24,   23,   23, 0x05,
      76,   24,   23,   23, 0x05,
     116,   23,   23,   23, 0x05,
     147,  135,   23,   23, 0x05,
     195,  135,   23,   23, 0x05,
     244,  242,   23,   23, 0x05,
     277,   23,   23,   23, 0x05,
     296,   23,   23,   23, 0x05,
     339,  309,   23,   23, 0x05,
     387,  384,   23,   23, 0x05,
     418,  416,   23,   23, 0x05,
     455,  416,   23,   23, 0x05,
     484,   23,   23,   23, 0x05,
     504,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     516,   23,   23,   23, 0x0a,
     534,   23,   23,   23, 0x0a,
     557,   23,   23,   23, 0x0a,
     582,   23,  577,   23, 0x0a,
     603,  597,   23,   23, 0x0a,
     625,   23,   23,   23, 0x0a,
     654,  135,   23,   23, 0x08,
     714,   23,   23,   23, 0x08,
     736,  135,   23,   23, 0x08,
     786,  242,   23,   23, 0x08,
     822,  384,   23,   23, 0x08,
     854,  416,   23,   23, 0x08,
     892,  416,   23,   23, 0x08,
     972,  924,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_SpectrumInterface[] = {
    "Q_MG_SpectrumInterface\0\0start,end\0"
    "HorizontalSelectionChanged(double,double)\0"
    "VerticalSelectionChanged(double,double)\0"
    "SelectionCleared()\0x1,y1,x2,y2\0"
    "SelectionChangedTo(double,double,double,double)\0"
    "SelectionFinished(double,double,double,double)\0"
    "y\0selectionFinishedRedLine(double)\0"
    "DeselectFinished()\0CrossMoved()\0"
    "start_hz,start_Y,end_hz,end_Y\0"
    "viewPortChanged(double,double,double,double)\0"
    ",,\0MarkerMoved(int,double,bool)\0,\0"
    "MarkersChanged(QVector<double>,bool)\0"
    "DoubleClicked(double,double)\0"
    "SpectrumActivated()\0plotReady()\0"
    "SetFillSpectrum()\0SetLinesOnlySpectrum()\0"
    "SetProgress(double)\0bool\0ShowProgress()\0"
    "value\0SetShowProgress(bool)\0"
    "slotCurSelectionChanged(int)\0"
    "on_selectedPixelChangeSpectrum(double,double,double,double)\0"
    "on_selectionCleared()\0"
    "on_selectionFinished(double,double,double,double)\0"
    "on_selectionFinishedRedLine(double)\0"
    "on_markerMoved(int,double,bool)\0"
    "on_markersChanged(QList<double>,bool)\0"
    "on_doubleClicked(double,double)\0"
    "value_startX,value_startY,value_endX,value_endY\0"
    "on_ViewPortChanged(double,double,double,double)\0"
};

void Q_MG_SpectrumInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_SpectrumInterface *_t = static_cast<Q_MG_SpectrumInterface *>(_o);
        switch (_id) {
        case 0: _t->HorizontalSelectionChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->VerticalSelectionChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->SelectionCleared(); break;
        case 3: _t->SelectionChangedTo((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 4: _t->SelectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 5: _t->selectionFinishedRedLine((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->DeselectFinished(); break;
        case 7: _t->CrossMoved(); break;
        case 8: _t->viewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 9: _t->MarkerMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: _t->MarkersChanged((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 11: _t->DoubleClicked((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 12: _t->SpectrumActivated(); break;
        case 13: _t->plotReady(); break;
        case 14: _t->SetFillSpectrum(); break;
        case 15: _t->SetLinesOnlySpectrum(); break;
        case 16: _t->SetProgress((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 17: { bool _r = _t->ShowProgress();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: _t->SetShowProgress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->slotCurSelectionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->on_selectedPixelChangeSpectrum((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 21: _t->on_selectionCleared(); break;
        case 22: _t->on_selectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 23: _t->on_selectionFinishedRedLine((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 24: _t->on_markerMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 25: _t->on_markersChanged((*reinterpret_cast< QList<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 26: _t->on_doubleClicked((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 27: _t->on_ViewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_SpectrumInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_SpectrumInterface::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Q_MG_SpectrumInterface,
      qt_meta_data_Q_MG_SpectrumInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_SpectrumInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_SpectrumInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_SpectrumInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_SpectrumInterface))
        return static_cast<void*>(const_cast< Q_MG_SpectrumInterface*>(this));
    return QWidget::qt_metacast(_clname);
}

int Q_MG_SpectrumInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void Q_MG_SpectrumInterface::HorizontalSelectionChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Q_MG_SpectrumInterface::VerticalSelectionChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Q_MG_SpectrumInterface::SelectionCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Q_MG_SpectrumInterface::SelectionChangedTo(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Q_MG_SpectrumInterface::SelectionFinished(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Q_MG_SpectrumInterface::selectionFinishedRedLine(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Q_MG_SpectrumInterface::DeselectFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Q_MG_SpectrumInterface::CrossMoved()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Q_MG_SpectrumInterface::viewPortChanged(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Q_MG_SpectrumInterface::MarkerMoved(int _t1, double _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Q_MG_SpectrumInterface::MarkersChanged(QVector<double> _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Q_MG_SpectrumInterface::DoubleClicked(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Q_MG_SpectrumInterface::SpectrumActivated()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void Q_MG_SpectrumInterface::plotReady()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}
QT_END_MOC_NAMESPACE
