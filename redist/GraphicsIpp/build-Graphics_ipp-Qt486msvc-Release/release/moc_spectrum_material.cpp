/****************************************************************************
** Meta object code from reading C++ file 'spectrum_material.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/modules/gui/multi_graphics_spectrum_interface/spectrum_material.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectrum_material.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SpectrumMaterial[] = {

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

static const char qt_meta_stringdata_SpectrumMaterial[] = {
    "SpectrumMaterial\0"
};

void SpectrumMaterial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SpectrumMaterial::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SpectrumMaterial::staticMetaObject = {
    { &Q_MG_Material::staticMetaObject, qt_meta_stringdata_SpectrumMaterial,
      qt_meta_data_SpectrumMaterial, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SpectrumMaterial::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SpectrumMaterial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SpectrumMaterial::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SpectrumMaterial))
        return static_cast<void*>(const_cast< SpectrumMaterial*>(this));
    return Q_MG_Material::qt_metacast(_clname);
}

int SpectrumMaterial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Q_MG_Material::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
