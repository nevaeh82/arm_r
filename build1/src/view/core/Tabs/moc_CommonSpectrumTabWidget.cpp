/****************************************************************************
** Meta object code from reading C++ file 'CommonSpectrumTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/Tabs/CommonSpectrumTabWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommonSpectrumTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommonSpectrumTabWidget_t {
    QByteArrayData data[6];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommonSpectrumTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommonSpectrumTabWidget_t qt_meta_stringdata_CommonSpectrumTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 23), // "CommonSpectrumTabWidget"
QT_MOC_LITERAL(1, 24, 23), // "setIndicatorStateSignal"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 5), // "state"
QT_MOC_LITERAL(4, 55, 21), // "setIndicatorStateSlot"
QT_MOC_LITERAL(5, 77, 15) // "slotCheckStatus"

    },
    "CommonSpectrumTabWidget\0setIndicatorStateSignal\0"
    "\0state\0setIndicatorStateSlot\0"
    "slotCheckStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommonSpectrumTabWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   32,    2, 0x08 /* Private */,
       5,    0,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

       0        // eod
};

void CommonSpectrumTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommonSpectrumTabWidget *_t = static_cast<CommonSpectrumTabWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setIndicatorStateSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setIndicatorStateSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotCheckStatus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommonSpectrumTabWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommonSpectrumTabWidget::setIndicatorStateSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CommonSpectrumTabWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CommonSpectrumTabWidget.data,
      qt_meta_data_CommonSpectrumTabWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CommonSpectrumTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommonSpectrumTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CommonSpectrumTabWidget.stringdata0))
        return static_cast<void*>(const_cast< CommonSpectrumTabWidget*>(this));
    if (!strcmp(_clname, "ITabWidget"))
        return static_cast< ITabWidget*>(const_cast< CommonSpectrumTabWidget*>(this));
    if (!strcmp(_clname, "IRpcListener"))
        return static_cast< IRpcListener*>(const_cast< CommonSpectrumTabWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CommonSpectrumTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CommonSpectrumTabWidget::setIndicatorStateSignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
