/****************************************************************************
** Meta object code from reading C++ file 'multi_graphics.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics/multi_graphics.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multi_graphics.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Q_MultiGraphics[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   17,   16,   16, 0x0a,
      59,   47,   16,   16, 0x0a,
      98,   16,   16,   16, 0x0a,
     119,   16,   16,   16, 0x0a,
     140,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MultiGraphics[] = {
    "Q_MultiGraphics\0\0pf\0ScreenDragByMouse(QPointF)\0"
    "pf,MousePos\0ScreenScaleByMidMouse(QPointF,QPointF)\0"
    "on_wantContextMenu()\0on_menuAboutToShow()\0"
    "on_menuAboutToHide()\0"
};

void Q_MultiGraphics::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MultiGraphics *_t = static_cast<Q_MultiGraphics *>(_o);
        switch (_id) {
        case 0: _t->ScreenDragByMouse((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: _t->ScreenScaleByMidMouse((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 2: _t->on_wantContextMenu(); break;
        case 3: _t->on_menuAboutToShow(); break;
        case 4: _t->on_menuAboutToHide(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MultiGraphics::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MultiGraphics::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_Q_MultiGraphics,
      qt_meta_data_Q_MultiGraphics, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MultiGraphics::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MultiGraphics::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MultiGraphics::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MultiGraphics))
        return static_cast<void*>(const_cast< Q_MultiGraphics*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int Q_MultiGraphics::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_Q_MG_BaseClass[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      41,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   16,   15,   15, 0x0a,
     123,   15,  116,   15, 0x0a,
     139,   15,  116,   15, 0x0a,
     155,   15,  116,   15, 0x0a,
     174,   15,  116,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_BaseClass[] = {
    "Q_MG_BaseClass\0\0ToObj,CommandName,params\0"
    "sendCommand(QString,QString,QVariant)\0"
    "getCommand(QString,QString,QVariant)\0"
    "double\0getCurKf_SQ_X()\0getCurKf_SQ_Y()\0"
    "getCurTrans_SQ_X()\0getCurTrans_SQ_Y()\0"
};

void Q_MG_BaseClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_BaseClass *_t = static_cast<Q_MG_BaseClass *>(_o);
        switch (_id) {
        case 0: _t->sendCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 1: _t->getCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 2: { double _r = _t->getCurKf_SQ_X();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 3: { double _r = _t->getCurKf_SQ_Y();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 4: { double _r = _t->getCurTrans_SQ_X();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 5: { double _r = _t->getCurTrans_SQ_Y();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_BaseClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_BaseClass::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_Q_MG_BaseClass,
      qt_meta_data_Q_MG_BaseClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_BaseClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_BaseClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_BaseClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_BaseClass))
        return static_cast<void*>(const_cast< Q_MG_BaseClass*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int Q_MG_BaseClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Q_MG_BaseClass::sendCommand(QString _t1, QString _t2, QVariant _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_Q_MG_MouseCursor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   18,   17,   17, 0x05,
      48,   17,   17,   17, 0x05,
      86,   67,   17,   17, 0x05,
     115,   17,   17,   17, 0x05,
     146,  134,   17,   17, 0x05,
     195,  134,   17,   17, 0x05,
     244,  242,   17,   17, 0x05,
     277,   17,   17,   17, 0x05,
     319,  296,   17,   17, 0x05,
     365,  349,   17,   17, 0x05,
     402,  400,   17,   17, 0x05,
     431,   17,   17,   17, 0x05,
     451,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     494,  469,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_MouseCursor[] = {
    "Q_MG_MouseCursor\0\0dragPoint\0"
    "dragScreen(QPointF)\0dragScreenFinish()\0"
    "dragPoint,MousePos\0scaleScreen(QPointF,QPointF)\0"
    "selectionCleared()\0x1,y1,x2,y2\0"
    "selectedPixelChange(double,double,double,double)\0"
    "selectionFinished(double,double,double,double)\0"
    "y\0selectionFinishedRedLine(double)\0"
    "deselectFinished()\0index,value,isVertical\0"
    "markerMooved(int,double,bool)\0"
    "Mark,isVertical\0markersChanged(QList<double>,bool)\0"
    ",\0DoubleClicked(double,double)\0"
    "StrangeCrossMoved()\0wantContextMenu()\0"
    "ToObj,CommandName,params\0"
    "getCommand(QString,QString,QVariant)\0"
};

void Q_MG_MouseCursor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_MouseCursor *_t = static_cast<Q_MG_MouseCursor *>(_o);
        switch (_id) {
        case 0: _t->dragScreen((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: _t->dragScreenFinish(); break;
        case 2: _t->scaleScreen((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 3: _t->selectionCleared(); break;
        case 4: _t->selectedPixelChange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 5: _t->selectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 6: _t->selectionFinishedRedLine((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->deselectFinished(); break;
        case 8: _t->markerMooved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 9: _t->markersChanged((*reinterpret_cast< QList<double>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->DoubleClicked((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 11: _t->StrangeCrossMoved(); break;
        case 12: _t->wantContextMenu(); break;
        case 13: _t->getCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_MouseCursor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_MouseCursor::staticMetaObject = {
    { &Q_MG_BaseClass::staticMetaObject, qt_meta_stringdata_Q_MG_MouseCursor,
      qt_meta_data_Q_MG_MouseCursor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_MouseCursor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_MouseCursor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_MouseCursor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_MouseCursor))
        return static_cast<void*>(const_cast< Q_MG_MouseCursor*>(this));
    return Q_MG_BaseClass::qt_metacast(_clname);
}

int Q_MG_MouseCursor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_BaseClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Q_MG_MouseCursor::dragScreen(QPointF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Q_MG_MouseCursor::dragScreenFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Q_MG_MouseCursor::scaleScreen(QPointF _t1, QPointF _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Q_MG_MouseCursor::selectionCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Q_MG_MouseCursor::selectedPixelChange(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Q_MG_MouseCursor::selectionFinished(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Q_MG_MouseCursor::selectionFinishedRedLine(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Q_MG_MouseCursor::deselectFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Q_MG_MouseCursor::markerMooved(int _t1, double _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Q_MG_MouseCursor::markersChanged(QList<double> _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Q_MG_MouseCursor::DoubleClicked(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Q_MG_MouseCursor::StrangeCrossMoved()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void Q_MG_MouseCursor::wantContextMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}
static const uint qt_meta_data_Q_MG_SelectionAreas[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      52,   40,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
     124,   99,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_SelectionAreas[] = {
    "Q_MG_SelectionAreas\0\0selectionCleared()\0"
    "x1,y1,x2,y2\0selectionFinished(double,double,double,double)\0"
    "ToObj,CommandName,params\0"
    "getCommand(QString,QString,QVariant)\0"
};

void Q_MG_SelectionAreas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_SelectionAreas *_t = static_cast<Q_MG_SelectionAreas *>(_o);
        switch (_id) {
        case 0: _t->selectionCleared(); break;
        case 1: _t->selectionFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 2: _t->getCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_SelectionAreas::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_SelectionAreas::staticMetaObject = {
    { &Q_MG_BaseClass::staticMetaObject, qt_meta_stringdata_Q_MG_SelectionAreas,
      qt_meta_data_Q_MG_SelectionAreas, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_SelectionAreas::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_SelectionAreas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_SelectionAreas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_SelectionAreas))
        return static_cast<void*>(const_cast< Q_MG_SelectionAreas*>(this));
    return Q_MG_BaseClass::qt_metacast(_clname);
}

int Q_MG_SelectionAreas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_BaseClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Q_MG_SelectionAreas::selectionCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Q_MG_SelectionAreas::selectionFinished(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_Q_MG_Grid[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      36,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_Grid[] = {
    "Q_MG_Grid\0\0ToObj,CommandName,params\0"
    "getCommand(QString,QString,QVariant)\0"
};

void Q_MG_Grid::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_Grid *_t = static_cast<Q_MG_Grid *>(_o);
        switch (_id) {
        case 0: _t->getCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_Grid::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_Grid::staticMetaObject = {
    { &Q_MG_BaseClass::staticMetaObject, qt_meta_stringdata_Q_MG_Grid,
      qt_meta_data_Q_MG_Grid, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_Grid::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_Grid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_Grid::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_Grid))
        return static_cast<void*>(const_cast< Q_MG_Grid*>(this));
    return Q_MG_BaseClass::qt_metacast(_clname);
}

int Q_MG_Grid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_BaseClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_Q_MG_InfoWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   17,   16,   16, 0x0a,
      58,   54,   16,   16, 0x2a,
      94,   79,   16,   16, 0x0a,
     141,  116,   16,   16, 0x0a,
     178,   16,   16,   16, 0x0a,
     205,   16,  198,   16, 0x0a,
     224,   16,  219,   16, 0x0a,
     245,  239,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_InfoWidget[] = {
    "Q_MG_InfoWidget\0\0str,isHtml\0"
    "setUserText(QString,bool)\0str\0"
    "setUserText(QString)\0_PrintTextMode\0"
    "setPrintTextMode(int)\0ToObj,CommandName,params\0"
    "getCommand(QString,QString,QVariant)\0"
    "SetProgress(double)\0double\0GetProgress()\0"
    "bool\0ShowProgress()\0value\0"
    "SetShowProgress(bool)\0"
};

void Q_MG_InfoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_InfoWidget *_t = static_cast<Q_MG_InfoWidget *>(_o);
        switch (_id) {
        case 0: _t->setUserText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->setUserText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setPrintTextMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->getCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 4: _t->SetProgress((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: { double _r = _t->GetProgress();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->ShowProgress();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->SetShowProgress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_InfoWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_InfoWidget::staticMetaObject = {
    { &Q_MG_BaseClass::staticMetaObject, qt_meta_stringdata_Q_MG_InfoWidget,
      qt_meta_data_Q_MG_InfoWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_InfoWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_InfoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_InfoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_InfoWidget))
        return static_cast<void*>(const_cast< Q_MG_InfoWidget*>(this));
    return Q_MG_BaseClass::qt_metacast(_clname);
}

int Q_MG_InfoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_BaseClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
static const uint qt_meta_data_Q_MG_RenderThreadClass[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   24,   23,   23, 0x05,
      67,   23,   23,   23, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_RenderThreadClass[] = {
    "Q_MG_RenderThreadClass\0\0,\0"
    "ElementReady(Q_MG_MaterialElement*,bool)\0"
    "RenderFinished()\0"
};

void Q_MG_RenderThreadClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_RenderThreadClass *_t = static_cast<Q_MG_RenderThreadClass *>(_o);
        switch (_id) {
        case 0: _t->ElementReady((*reinterpret_cast< Q_MG_MaterialElement*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->RenderFinished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_RenderThreadClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_RenderThreadClass::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Q_MG_RenderThreadClass,
      qt_meta_data_Q_MG_RenderThreadClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_RenderThreadClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_RenderThreadClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_RenderThreadClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_RenderThreadClass))
        return static_cast<void*>(const_cast< Q_MG_RenderThreadClass*>(this));
    return QThread::qt_metacast(_clname);
}

int Q_MG_RenderThreadClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Q_MG_RenderThreadClass::ElementReady(Q_MG_MaterialElement * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Q_MG_RenderThreadClass::RenderFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_Q_MG_Material[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      63,   15,   14,   14, 0x05,
     108,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     145,  120,   14,   14, 0x0a,
     182,   14,   14,   14, 0x0a,
     221,  203,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Q_MG_Material[] = {
    "Q_MG_Material\0\0"
    "value_startX,value_startY,value_endX,value_endY\0"
    "ViewPortChanged(double,double,double,double)\0"
    "plotReady()\0ToObj,CommandName,params\0"
    "getCommand(QString,QString,QVariant)\0"
    "RenderFinishedSlot()\0el,ElementIsEmpty\0"
    "ElementReady(Q_MG_MaterialElement*,bool)\0"
};

void Q_MG_Material::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q_MG_Material *_t = static_cast<Q_MG_Material *>(_o);
        switch (_id) {
        case 0: _t->ViewPortChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 1: _t->plotReady(); break;
        case 2: _t->getCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 3: _t->RenderFinishedSlot(); break;
        case 4: _t->ElementReady((*reinterpret_cast< Q_MG_MaterialElement*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q_MG_Material::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q_MG_Material::staticMetaObject = {
    { &Q_MG_BaseClass::staticMetaObject, qt_meta_stringdata_Q_MG_Material,
      qt_meta_data_Q_MG_Material, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q_MG_Material::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q_MG_Material::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q_MG_Material::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q_MG_Material))
        return static_cast<void*>(const_cast< Q_MG_Material*>(this));
    return Q_MG_BaseClass::qt_metacast(_clname);
}

int Q_MG_Material::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_BaseClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Q_MG_Material::ViewPortChanged(double _t1, double _t2, double _t3, double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Q_MG_Material::plotReady()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
