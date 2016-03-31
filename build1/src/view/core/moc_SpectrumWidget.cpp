/****************************************************************************
** Meta object code from reading C++ file 'SpectrumWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/view/core/SpectrumWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SpectrumWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SpectrumWidget_t {
    QByteArrayData data[12];
    char stringdata0[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpectrumWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpectrumWidget_t qt_meta_stringdata_SpectrumWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SpectrumWidget"
QT_MOC_LITERAL(1, 15, 14), // "signalEnableKM"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "state"
QT_MOC_LITERAL(4, 37, 17), // "setPanoramaSignal"
QT_MOC_LITERAL(5, 55, 19), // "setAutoSearchSignal"
QT_MOC_LITERAL(6, 75, 26), // "selectionTypeChangedSignal"
QT_MOC_LITERAL(7, 102, 17), // "requestDataSignal"
QT_MOC_LITERAL(8, 120, 18), // "setShowPeaksSignal"
QT_MOC_LITERAL(9, 139, 17), // "setShowControlPRM"
QT_MOC_LITERAL(10, 157, 21), // "slotSetEnableSpactrum"
QT_MOC_LITERAL(11, 179, 12) // "slotEnableKM"

    },
    "SpectrumWidget\0signalEnableKM\0\0state\0"
    "setPanoramaSignal\0setAutoSearchSignal\0"
    "selectionTypeChangedSignal\0requestDataSignal\0"
    "setShowPeaksSignal\0setShowControlPRM\0"
    "slotSetEnableSpactrum\0slotEnableKM"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpectrumWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       5,    1,   65,    2, 0x06 /* Public */,
       6,    1,   68,    2, 0x06 /* Public */,
       7,    1,   71,    2, 0x06 /* Public */,
       8,    1,   74,    2, 0x06 /* Public */,
       9,    1,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   80,    2, 0x0a /* Public */,
      11,    1,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void SpectrumWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SpectrumWidget *_t = static_cast<SpectrumWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalEnableKM((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setPanoramaSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setAutoSearchSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->selectionTypeChangedSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->requestDataSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setShowPeaksSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setShowControlPRM((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->slotSetEnableSpactrum((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->slotEnableKM((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::signalEnableKM)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::setPanoramaSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::setAutoSearchSignal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::selectionTypeChangedSignal)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::requestDataSignal)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::setShowPeaksSignal)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (SpectrumWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpectrumWidget::setShowControlPRM)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject SpectrumWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SpectrumWidget.data,
      qt_meta_data_SpectrumWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SpectrumWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpectrumWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SpectrumWidget.stringdata0))
        return static_cast<void*>(const_cast< SpectrumWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SpectrumWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SpectrumWidget::signalEnableKM(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SpectrumWidget::setPanoramaSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SpectrumWidget::setAutoSearchSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SpectrumWidget::selectionTypeChangedSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SpectrumWidget::requestDataSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SpectrumWidget::setShowPeaksSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SpectrumWidget::setShowControlPRM(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
