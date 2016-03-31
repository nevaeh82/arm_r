/****************************************************************************
** Meta object code from reading C++ file 'ControlPanelWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/ControlPanel/ControlPanelWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlPanelWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ControlPanelWidget_t {
    QByteArrayData data[27];
    char stringdata0[399];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlPanelWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlPanelWidget_t qt_meta_stringdata_ControlPanelWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ControlPanelWidget"
QT_MOC_LITERAL(1, 19, 23), // "commonFreqChangedSignal"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 5), // "value"
QT_MOC_LITERAL(4, 50, 22), // "bandwidthChangedSignal"
QT_MOC_LITERAL(5, 73, 5), // "start"
QT_MOC_LITERAL(6, 79, 3), // "end"
QT_MOC_LITERAL(7, 83, 21), // "panoramaCheckedSignal"
QT_MOC_LITERAL(8, 105, 9), // "isChecked"
QT_MOC_LITERAL(9, 115, 23), // "autoSearchCheckedSignal"
QT_MOC_LITERAL(10, 139, 16), // "signalManualMode"
QT_MOC_LITERAL(11, 156, 14), // "signalScanMode"
QT_MOC_LITERAL(12, 171, 6), // "finish"
QT_MOC_LITERAL(13, 178, 15), // "signalCheckMode"
QT_MOC_LITERAL(14, 194, 14), // "signalViewMode"
QT_MOC_LITERAL(15, 209, 14), // "signalDown1Mhz"
QT_MOC_LITERAL(16, 224, 15), // "signalDown10Mhz"
QT_MOC_LITERAL(17, 240, 16), // "signalDown100Mhz"
QT_MOC_LITERAL(18, 257, 12), // "signalUp1Mhz"
QT_MOC_LITERAL(19, 270, 13), // "signalUp10Mhz"
QT_MOC_LITERAL(20, 284, 14), // "signalUp100Mhz"
QT_MOC_LITERAL(21, 299, 13), // "signalSetMode"
QT_MOC_LITERAL(22, 313, 24), // "onSetCommonFrequencySlot"
QT_MOC_LITERAL(23, 338, 18), // "onSetBandWidthSlot"
QT_MOC_LITERAL(24, 357, 14), // "slotChangeMode"
QT_MOC_LITERAL(25, 372, 5), // "index"
QT_MOC_LITERAL(26, 378, 20) // "slotChangeCommonFreq"

    },
    "ControlPanelWidget\0commonFreqChangedSignal\0"
    "\0value\0bandwidthChangedSignal\0start\0"
    "end\0panoramaCheckedSignal\0isChecked\0"
    "autoSearchCheckedSignal\0signalManualMode\0"
    "signalScanMode\0finish\0signalCheckMode\0"
    "signalViewMode\0signalDown1Mhz\0"
    "signalDown10Mhz\0signalDown100Mhz\0"
    "signalUp1Mhz\0signalUp10Mhz\0signalUp100Mhz\0"
    "signalSetMode\0onSetCommonFrequencySlot\0"
    "onSetBandWidthSlot\0slotChangeMode\0"
    "index\0slotChangeCommonFreq"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlPanelWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       4,    2,  112,    2, 0x06 /* Public */,
       7,    1,  117,    2, 0x06 /* Public */,
       9,    1,  120,    2, 0x06 /* Public */,
      10,    0,  123,    2, 0x06 /* Public */,
      11,    2,  124,    2, 0x06 /* Public */,
      13,    0,  129,    2, 0x06 /* Public */,
      14,    0,  130,    2, 0x06 /* Public */,
      15,    0,  131,    2, 0x06 /* Public */,
      16,    0,  132,    2, 0x06 /* Public */,
      17,    0,  133,    2, 0x06 /* Public */,
      18,    0,  134,    2, 0x06 /* Public */,
      19,    0,  135,    2, 0x06 /* Public */,
      20,    0,  136,    2, 0x06 /* Public */,
      21,    1,  137,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      22,    0,  140,    2, 0x08 /* Private */,
      23,    0,  141,    2, 0x08 /* Private */,
      24,    1,  142,    2, 0x08 /* Private */,
      26,    1,  145,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void ControlPanelWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ControlPanelWidget *_t = static_cast<ControlPanelWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commonFreqChangedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->bandwidthChangedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->panoramaCheckedSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->autoSearchCheckedSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->signalManualMode(); break;
        case 5: _t->signalScanMode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->signalCheckMode(); break;
        case 7: _t->signalViewMode(); break;
        case 8: _t->signalDown1Mhz(); break;
        case 9: _t->signalDown10Mhz(); break;
        case 10: _t->signalDown100Mhz(); break;
        case 11: _t->signalUp1Mhz(); break;
        case 12: _t->signalUp10Mhz(); break;
        case 13: _t->signalUp100Mhz(); break;
        case 14: _t->signalSetMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->onSetCommonFrequencySlot(); break;
        case 16: _t->onSetBandWidthSlot(); break;
        case 17: _t->slotChangeMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->slotChangeCommonFreq((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ControlPanelWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::commonFreqChangedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::bandwidthChangedSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::panoramaCheckedSignal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::autoSearchCheckedSignal)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalManualMode)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalScanMode)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalCheckMode)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalViewMode)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalDown1Mhz)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalDown10Mhz)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalDown100Mhz)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalUp1Mhz)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalUp10Mhz)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalUp100Mhz)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (ControlPanelWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControlPanelWidget::signalSetMode)) {
                *result = 14;
                return;
            }
        }
    }
}

const QMetaObject ControlPanelWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ControlPanelWidget.data,
      qt_meta_data_ControlPanelWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ControlPanelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanelWidget.stringdata0))
        return static_cast<void*>(const_cast< ControlPanelWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ControlPanelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void ControlPanelWidget::commonFreqChangedSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControlPanelWidget::bandwidthChangedSignal(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ControlPanelWidget::panoramaCheckedSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ControlPanelWidget::autoSearchCheckedSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ControlPanelWidget::signalManualMode()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void ControlPanelWidget::signalScanMode(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ControlPanelWidget::signalCheckMode()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void ControlPanelWidget::signalViewMode()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void ControlPanelWidget::signalDown1Mhz()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}

// SIGNAL 9
void ControlPanelWidget::signalDown10Mhz()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}

// SIGNAL 10
void ControlPanelWidget::signalDown100Mhz()
{
    QMetaObject::activate(this, &staticMetaObject, 10, Q_NULLPTR);
}

// SIGNAL 11
void ControlPanelWidget::signalUp1Mhz()
{
    QMetaObject::activate(this, &staticMetaObject, 11, Q_NULLPTR);
}

// SIGNAL 12
void ControlPanelWidget::signalUp10Mhz()
{
    QMetaObject::activate(this, &staticMetaObject, 12, Q_NULLPTR);
}

// SIGNAL 13
void ControlPanelWidget::signalUp100Mhz()
{
    QMetaObject::activate(this, &staticMetaObject, 13, Q_NULLPTR);
}

// SIGNAL 14
void ControlPanelWidget::signalSetMode(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
QT_END_MOC_NAMESPACE
