/****************************************************************************
** Meta object code from reading C++ file 'SpectrumWidgetDataSource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/view/core/SpectrumWidgetDataSource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SpectrumWidgetDataSource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SpectrumWidgetDataSource_t {
    QByteArrayData data[6];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpectrumWidgetDataSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpectrumWidgetDataSource_t qt_meta_stringdata_SpectrumWidgetDataSource = {
    {
QT_MOC_LITERAL(0, 0, 24), // "SpectrumWidgetDataSource"
QT_MOC_LITERAL(1, 25, 20), // "onMethodCalledSignal"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 18), // "onMethodCalledSlot"
QT_MOC_LITERAL(4, 66, 22), // "slotRepeatSetFrequency"
QT_MOC_LITERAL(5, 89, 14) // "slotChangeFreq"

    },
    "SpectrumWidgetDataSource\0onMethodCalledSignal\0"
    "\0onMethodCalledSlot\0slotRepeatSetFrequency\0"
    "slotChangeFreq"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpectrumWidgetDataSource[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   39,    2, 0x08 /* Private */,
       4,    0,   44,    2, 0x08 /* Private */,
       5,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SpectrumWidgetDataSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SpectrumWidgetDataSource *_t = static_cast<SpectrumWidgetDataSource *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onMethodCalledSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 1: _t->onMethodCalledSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 2: _t->slotRepeatSetFrequency(); break;
        case 3: _t->slotChangeFreq(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SpectrumWidgetDataSource::*_t)(QString , QVariant );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidgetDataSource::onMethodCalledSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SpectrumWidgetDataSource::staticMetaObject = {
    { &BaseDataSource::staticMetaObject, qt_meta_stringdata_SpectrumWidgetDataSource.data,
      qt_meta_data_SpectrumWidgetDataSource,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SpectrumWidgetDataSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpectrumWidgetDataSource::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SpectrumWidgetDataSource.stringdata0))
        return static_cast<void*>(const_cast< SpectrumWidgetDataSource*>(this));
    if (!strcmp(_clname, "IRpcListener"))
        return static_cast< IRpcListener*>(const_cast< SpectrumWidgetDataSource*>(this));
    return BaseDataSource::qt_metacast(_clname);
}

int SpectrumWidgetDataSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseDataSource::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SpectrumWidgetDataSource::onMethodCalledSignal(QString _t1, QVariant _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
