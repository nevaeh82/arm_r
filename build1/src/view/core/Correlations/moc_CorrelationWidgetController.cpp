/****************************************************************************
** Meta object code from reading C++ file 'CorrelationWidgetController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/Correlations/CorrelationWidgetController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CorrelationWidgetController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CorrelationWidgetController_t {
    QByteArrayData data[9];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CorrelationWidgetController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CorrelationWidgetController_t qt_meta_stringdata_CorrelationWidgetController = {
    {
QT_MOC_LITERAL(0, 0, 27), // "CorrelationWidgetController"
QT_MOC_LITERAL(1, 28, 21), // "signalonDataArrivedLS"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 6), // "method"
QT_MOC_LITERAL(4, 58, 3), // "arg"
QT_MOC_LITERAL(5, 62, 15), // "signalOnVisible"
QT_MOC_LITERAL(6, 78, 15), // "onDataArrivedLS"
QT_MOC_LITERAL(7, 94, 9), // "onVisible"
QT_MOC_LITERAL(8, 104, 1) // "b"

    },
    "CorrelationWidgetController\0"
    "signalonDataArrivedLS\0\0method\0arg\0"
    "signalOnVisible\0onDataArrivedLS\0"
    "onVisible\0b"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CorrelationWidgetController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       5,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   42,    2, 0x08 /* Private */,
       7,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    4,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    4,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void CorrelationWidgetController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CorrelationWidgetController *_t = static_cast<CorrelationWidgetController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalonDataArrivedLS((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 1: _t->signalOnVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 2: _t->onDataArrivedLS((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 3: _t->onVisible((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CorrelationWidgetController::*_t)(const QString & , const QVariant & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CorrelationWidgetController::signalonDataArrivedLS)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CorrelationWidgetController::*_t)(const bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CorrelationWidgetController::signalOnVisible)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CorrelationWidgetController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CorrelationWidgetController.data,
      qt_meta_data_CorrelationWidgetController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CorrelationWidgetController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CorrelationWidgetController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CorrelationWidgetController.stringdata0))
        return static_cast<void*>(const_cast< CorrelationWidgetController*>(this));
    if (!strcmp(_clname, "ICorrelationWidget"))
        return static_cast< ICorrelationWidget*>(const_cast< CorrelationWidgetController*>(this));
    if (!strcmp(_clname, "IController<CorrelationWidget>"))
        return static_cast< IController<CorrelationWidget>*>(const_cast< CorrelationWidgetController*>(this));
    return QObject::qt_metacast(_clname);
}

int CorrelationWidgetController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CorrelationWidgetController::signalonDataArrivedLS(const QString & _t1, const QVariant & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CorrelationWidgetController::signalOnVisible(const bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
