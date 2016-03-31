/****************************************************************************
** Meta object code from reading C++ file 'TabSpectrumWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/Tabs/TabSpectrumWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabSpectrumWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TabSpectrumWidget_t {
    QByteArrayData data[6];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TabSpectrumWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TabSpectrumWidget_t qt_meta_stringdata_TabSpectrumWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "TabSpectrumWidget"
QT_MOC_LITERAL(1, 18, 23), // "setIndicatorStateSignal"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 5), // "state"
QT_MOC_LITERAL(4, 49, 27), // "spectrumDoubleClickedSignal"
QT_MOC_LITERAL(5, 77, 21) // "setIndicatorStateSlot"

    },
    "TabSpectrumWidget\0setIndicatorStateSignal\0"
    "\0state\0spectrumDoubleClickedSignal\0"
    "setIndicatorStateSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TabSpectrumWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void TabSpectrumWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TabSpectrumWidget *_t = static_cast<TabSpectrumWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setIndicatorStateSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->spectrumDoubleClickedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setIndicatorStateSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TabSpectrumWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidget::setIndicatorStateSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TabSpectrumWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabSpectrumWidget::spectrumDoubleClickedSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TabSpectrumWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TabSpectrumWidget.data,
      qt_meta_data_TabSpectrumWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TabSpectrumWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabSpectrumWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TabSpectrumWidget.stringdata0))
        return static_cast<void*>(const_cast< TabSpectrumWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int TabSpectrumWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TabSpectrumWidget::setIndicatorStateSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TabSpectrumWidget::spectrumDoubleClickedSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
