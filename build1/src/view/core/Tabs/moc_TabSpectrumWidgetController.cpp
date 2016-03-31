/****************************************************************************
** Meta object code from reading C++ file 'TabSpectrumWidgetController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/Tabs/TabSpectrumWidgetController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabSpectrumWidgetController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TabSpectrumWidgetController_t {
    QByteArrayData data[17];
    char stringdata0[295];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TabSpectrumWidgetController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TabSpectrumWidgetController_t qt_meta_stringdata_TabSpectrumWidgetController = {
    {
QT_MOC_LITERAL(0, 0, 27), // "TabSpectrumWidgetController"
QT_MOC_LITERAL(1, 28, 28), // "signalGetPointsFromRPCFlakon"
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 6), // "points"
QT_MOC_LITERAL(4, 65, 21), // "signalChangeIndicator"
QT_MOC_LITERAL(5, 87, 5), // "state"
QT_MOC_LITERAL(6, 93, 19), // "signalDoubleClicked"
QT_MOC_LITERAL(7, 113, 2), // "id"
QT_MOC_LITERAL(8, 116, 19), // "signalPanoramaState"
QT_MOC_LITERAL(9, 136, 27), // "signalUpdateDBStationsLists"
QT_MOC_LITERAL(10, 164, 19), // "signalUpdateDBAreas"
QT_MOC_LITERAL(11, 184, 20), // "slotGetPointsFromRpc"
QT_MOC_LITERAL(12, 205, 18), // "slotShowControlPrm"
QT_MOC_LITERAL(13, 224, 25), // "spectrumDoubleClickedSlot"
QT_MOC_LITERAL(14, 250, 18), // "enablePanoramaSlot"
QT_MOC_LITERAL(15, 269, 9), // "isEnabled"
QT_MOC_LITERAL(16, 279, 15) // "slotCheckStatus"

    },
    "TabSpectrumWidgetController\0"
    "signalGetPointsFromRPCFlakon\0\0points\0"
    "signalChangeIndicator\0state\0"
    "signalDoubleClicked\0id\0signalPanoramaState\0"
    "signalUpdateDBStationsLists\0"
    "signalUpdateDBAreas\0slotGetPointsFromRpc\0"
    "slotShowControlPrm\0spectrumDoubleClickedSlot\0"
    "enablePanoramaSlot\0isEnabled\0"
    "slotCheckStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TabSpectrumWidgetController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       6,    3,   75,    2, 0x06 /* Public */,
       8,    1,   82,    2, 0x06 /* Public */,
       9,    0,   85,    2, 0x06 /* Public */,
      10,    0,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   87,    2, 0x08 /* Private */,
      12,    1,   90,    2, 0x08 /* Private */,
      13,    1,   93,    2, 0x08 /* Private */,
      14,    1,   96,    2, 0x08 /* Private */,
      16,    0,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double,    7,    2,    2,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,

       0        // eod
};

void TabSpectrumWidgetController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TabSpectrumWidgetController *_t = static_cast<TabSpectrumWidgetController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalGetPointsFromRPCFlakon((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->signalChangeIndicator((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signalDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 3: _t->signalPanoramaState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->signalUpdateDBStationsLists(); break;
        case 5: _t->signalUpdateDBAreas(); break;
        case 6: _t->slotGetPointsFromRpc((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->slotShowControlPrm((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->spectrumDoubleClickedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->enablePanoramaSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->slotCheckStatus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TabSpectrumWidgetController::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidgetController::signalGetPointsFromRPCFlakon)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TabSpectrumWidgetController::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidgetController::signalChangeIndicator)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TabSpectrumWidgetController::*_t)(int , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidgetController::signalDoubleClicked)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TabSpectrumWidgetController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidgetController::signalPanoramaState)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (TabSpectrumWidgetController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidgetController::signalUpdateDBStationsLists)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (TabSpectrumWidgetController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidgetController::signalUpdateDBAreas)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject TabSpectrumWidgetController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TabSpectrumWidgetController.data,
      qt_meta_data_TabSpectrumWidgetController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TabSpectrumWidgetController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabSpectrumWidgetController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TabSpectrumWidgetController.stringdata0))
        return static_cast<void*>(const_cast< TabSpectrumWidgetController*>(this));
    if (!strcmp(_clname, "IController<TabSpectrumWidget>"))
        return static_cast< IController<TabSpectrumWidget>*>(const_cast< TabSpectrumWidgetController*>(this));
    if (!strcmp(_clname, "ITabWidget"))
        return static_cast< ITabWidget*>(const_cast< TabSpectrumWidgetController*>(this));
    if (!strcmp(_clname, "ITabSpectrum"))
        return static_cast< ITabSpectrum*>(const_cast< TabSpectrumWidgetController*>(this));
    if (!strcmp(_clname, "IDbChangedListener"))
        return static_cast< IDbChangedListener*>(const_cast< TabSpectrumWidgetController*>(this));
    if (!strcmp(_clname, "IRpcListener"))
        return static_cast< IRpcListener*>(const_cast< TabSpectrumWidgetController*>(this));
    return QObject::qt_metacast(_clname);
}

int TabSpectrumWidgetController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TabSpectrumWidgetController::signalGetPointsFromRPCFlakon(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TabSpectrumWidgetController::signalChangeIndicator(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TabSpectrumWidgetController::signalDoubleClicked(int _t1, double _t2, double _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TabSpectrumWidgetController::signalPanoramaState(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TabSpectrumWidgetController::signalUpdateDBStationsLists()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void TabSpectrumWidgetController::signalUpdateDBAreas()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
