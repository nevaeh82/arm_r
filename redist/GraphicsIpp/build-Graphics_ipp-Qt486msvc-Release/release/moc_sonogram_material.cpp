/****************************************************************************
** Meta object code from reading C++ file 'sonogram_material.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics_sonogram_interface/sonogram_material.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sonogram_material.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CalculatePallateThreadClass[] = {

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
      34,   29,   28,   28, 0x05,
      55,   52,   28,   28, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CalculatePallateThreadClass[] = {
    "CalculatePallateThreadClass\0\0perc\0"
    "CalcProgress(int)\0Ok\0CalcFinished(bool)\0"
};

void CalculatePallateThreadClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CalculatePallateThreadClass *_t = static_cast<CalculatePallateThreadClass *>(_o);
        switch (_id) {
        case 0: _t->CalcProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->CalcFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CalculatePallateThreadClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CalculatePallateThreadClass::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CalculatePallateThreadClass,
      qt_meta_data_CalculatePallateThreadClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CalculatePallateThreadClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CalculatePallateThreadClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CalculatePallateThreadClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CalculatePallateThreadClass))
        return static_cast<void*>(const_cast< CalculatePallateThreadClass*>(this));
    return QThread::qt_metacast(_clname);
}

int CalculatePallateThreadClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CalculatePallateThreadClass::CalcProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CalculatePallateThreadClass::CalcFinished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_SonogramMaterial[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      45,   18,   17,   17, 0x05,
     101,   89,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     145,  140,   17,   17, 0x0a,
     180,  177,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SonogramMaterial[] = {
    "SonogramMaterial\0\0mean_spectrum,max_spectrum\0"
    "ScreenSpectrumsChanged(FFTLine_32f,FFTLine)\0"
    "levels_data\0CalcPallateFinished_signal(QByteArray)\0"
    "perc\0CalcPallateProgressChanged(int)\0"
    "Ok\0CalcPallateFinished(bool)\0"
};

void SonogramMaterial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SonogramMaterial *_t = static_cast<SonogramMaterial *>(_o);
        switch (_id) {
        case 0: _t->ScreenSpectrumsChanged((*reinterpret_cast< FFTLine_32f(*)>(_a[1])),(*reinterpret_cast< FFTLine(*)>(_a[2]))); break;
        case 1: _t->CalcPallateFinished_signal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->CalcPallateProgressChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->CalcPallateFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SonogramMaterial::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SonogramMaterial::staticMetaObject = {
    { &Q_MG_Material::staticMetaObject, qt_meta_stringdata_SonogramMaterial,
      qt_meta_data_SonogramMaterial, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SonogramMaterial::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SonogramMaterial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SonogramMaterial::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SonogramMaterial))
        return static_cast<void*>(const_cast< SonogramMaterial*>(this));
    return Q_MG_Material::qt_metacast(_clname);
}

int SonogramMaterial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_Material::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SonogramMaterial::ScreenSpectrumsChanged(FFTLine_32f _t1, FFTLine _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SonogramMaterial::CalcPallateFinished_signal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_functorClassDrawImage[] = {

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

static const char qt_meta_stringdata_functorClassDrawImage[] = {
    "functorClassDrawImage\0"
};

void functorClassDrawImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData functorClassDrawImage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject functorClassDrawImage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_functorClassDrawImage,
      qt_meta_data_functorClassDrawImage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &functorClassDrawImage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *functorClassDrawImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *functorClassDrawImage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_functorClassDrawImage))
        return static_cast<void*>(const_cast< functorClassDrawImage*>(this));
    return QObject::qt_metacast(_clname);
}

int functorClassDrawImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SonogramGraphicsRenderThread[] = {

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

static const char qt_meta_stringdata_SonogramGraphicsRenderThread[] = {
    "SonogramGraphicsRenderThread\0"
};

void SonogramGraphicsRenderThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SonogramGraphicsRenderThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SonogramGraphicsRenderThread::staticMetaObject = {
    { &Q_MG_RenderThreadClass::staticMetaObject, qt_meta_stringdata_SonogramGraphicsRenderThread,
      qt_meta_data_SonogramGraphicsRenderThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SonogramGraphicsRenderThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SonogramGraphicsRenderThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SonogramGraphicsRenderThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SonogramGraphicsRenderThread))
        return static_cast<void*>(const_cast< SonogramGraphicsRenderThread*>(this));
    return Q_MG_RenderThreadClass::qt_metacast(_clname);
}

int SonogramGraphicsRenderThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_RenderThreadClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
