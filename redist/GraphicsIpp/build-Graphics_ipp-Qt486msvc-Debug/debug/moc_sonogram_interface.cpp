/****************************************************************************
** Meta object code from reading C++ file 'sonogram_interface.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics_sonogram_interface/sonogram_interface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sonogram_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SonogramInterface[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      38,   18,   18,   18, 0x05,
      67,   57,   18,   18, 0x05,
     109,   57,   18,   18, 0x05,
     161,  149,   18,   18, 0x05,
     209,  149,   18,   18, 0x05,
     259,  256,   18,   18, 0x05,
     290,  288,   18,   18, 0x05,
     363,  327,   18,   18, 0x05,
     408,   18,   18,   18, 0x05,
     454,  427,   18,   18, 0x05,
     523,  506,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     555,   18,   18,   18, 0x0a,
     573,  327,   18,   18, 0x0a,
     623,  614,   18,   18, 0x0a,
     648,   18,   18,   18, 0x2a,
     690,  669,   18,   18, 0x0a,
     737,  149,   18,   18, 0x2a,
     779,   18,   18,   18, 0x0a,
     793,   18,   18,   18, 0x0a,
     807,   18,   18,   18, 0x0a,
     827,   18,   18,   18, 0x0a,
     857,  846,   18,   18, 0x0a,
     895,  288,   18,   18, 0x2a,
     928,  256,   18,   18, 0x0a,
     958,   18,   18,   18, 0x0a,
     976,  149,   18,   18, 0x08,
    1036,  149,   18,   18, 0x08,
    1134, 1086,   18,   18, 0x08,
    1182,   18,   18,   18, 0x08,
    1204,  256,   18,   18, 0x08,
    1236,  288,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SonogramInterface[] = {
    "SonogramInterface\0\0SelectionCleared()\0"
    "DeselectFinished()\0start,end\0"
    "HorizontalSelectionChanged(double,double)\0"
    "VerticalSelectionChanged(double,double)\0"
    "x1,y1,x2,y2\0SelectionChangedTo(double,double,double,double)\0"
    "selectionFinished(double,double,double,double)\0"
    ",,\0MarkerMoved(int,double,bool)\0,\0"
    "MarkersChanged(QVector<double>,bool)\0"
    "start_hz,start_smpl,end_hz,end_smpl\0"
    "viewPortChanged(double,double,double,double)\0"
    "dragScreenFinish()\0mean_spectrum,max_spectrum\0"
    "SonogramScreenSpectrumsChanged(FFTLine_32f,FFTLine)\0"
    "color_level_data\0CalcPallateFinished(QByteArray)\0"
    "ZoomToSelection()\0"
    "SetViewport(double,double,double,double)\0"
    "noSignal\0ClearAllSelections(bool)\0"
    "ClearAllSelections()\0x1,y1,x2,y2,noSignal\0"
    "SetSelection(double,double,double,double,bool)\0"
    "SetSelection(double,double,double,double)\0"
    "ShowMarkers()\0HideMarkers()\0"
    "SetMoveAllMarkers()\0SetMoveOneMarker()\0"
    ",,noSignal\0SetMarkers(QVector<double>,bool,bool)\0"
    "SetMarkers(QVector<double>,bool)\0"
    "UpdateMarker(int,double,bool)\0"
    "ClearAllMarkers()\0"
    "on_selectedPixelChangeSonogram(double,double,double,double)\0"
    "on_selectionFinished(double,double,double,double)\0"
    "value_startX,value_startY,value_endX,value_endY\0"
    "on_ViewPortChanged(double,double,double,double)\0"
    "on_selectionCleared()\0"
    "on_markerMoved(int,double,bool)\0"
    "on_markersChanged(QList<double>,bool)\0"
};

void SonogramInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SonogramInterface *_t = static_cast<SonogramInterface *>(_o);
        switch (_id) {
        case 0: _t->SelectionCleared(); break;
        case 1: _t->DeselectFinished(); break;
        case 2: _t->HorizontalSelectionChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->VerticalSelectionChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->SelectionChangedTo((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 5: _t->selectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 6: _t->MarkerMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 7: _t->MarkersChanged((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->viewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 9: _t->dragScreenFinish(); break;
        case 10: _t->SonogramScreenSpectrumsChanged((*reinterpret_cast< FFTLine_32f(*)>(_a[1])),(*reinterpret_cast< FFTLine(*)>(_a[2]))); break;
        case 11: _t->CalcPallateFinished((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 12: _t->ZoomToSelection(); break;
        case 13: _t->SetViewport((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 14: _t->ClearAllSelections((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->ClearAllSelections(); break;
        case 16: _t->SetSelection((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 17: _t->SetSelection((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 18: _t->ShowMarkers(); break;
        case 19: _t->HideMarkers(); break;
        case 20: _t->SetMoveAllMarkers(); break;
        case 21: _t->SetMoveOneMarker(); break;
        case 22: _t->SetMarkers((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 23: _t->SetMarkers((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 24: _t->UpdateMarker((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 25: _t->ClearAllMarkers(); break;
        case 26: _t->on_selectedPixelChangeSonogram((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 27: _t->on_selectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 28: _t->on_ViewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 29: _t->on_selectionCleared(); break;
        case 30: _t->on_markerMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 31: _t->on_markersChanged((*reinterpret_cast< QList<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SonogramInterface::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SonogramInterface::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SonogramInterface,
      qt_meta_data_SonogramInterface, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SonogramInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SonogramInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SonogramInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SonogramInterface))
        return static_cast<void*>(const_cast< SonogramInterface*>(this));
    return QWidget::qt_metacast(_clname);
}

int SonogramInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void SonogramInterface::SelectionCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SonogramInterface::DeselectFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SonogramInterface::HorizontalSelectionChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SonogramInterface::VerticalSelectionChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SonogramInterface::SelectionChangedTo(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SonogramInterface::selectionFinished(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SonogramInterface::MarkerMoved(int _t1, double _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void SonogramInterface::MarkersChanged(QVector<double> _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void SonogramInterface::viewPortChanged(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SonogramInterface::dragScreenFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void SonogramInterface::SonogramScreenSpectrumsChanged(FFTLine_32f _t1, FFTLine _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void SonogramInterface::CalcPallateFinished(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
