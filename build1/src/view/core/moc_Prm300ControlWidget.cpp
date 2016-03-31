/****************************************************************************
** Meta object code from reading C++ file 'Prm300ControlWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/view/core/Prm300ControlWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Prm300ControlWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Prm300ControlWidget_t {
    QByteArrayData data[11];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Prm300ControlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Prm300ControlWidget_t qt_meta_stringdata_Prm300ControlWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "Prm300ControlWidget"
QT_MOC_LITERAL(1, 20, 18), // "signalSetAtt1Value"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "value"
QT_MOC_LITERAL(4, 46, 18), // "signalSetAtt2Value"
QT_MOC_LITERAL(5, 65, 15), // "signalSetFilter"
QT_MOC_LITERAL(6, 81, 5), // "index"
QT_MOC_LITERAL(7, 87, 10), // "slotAtt1Up"
QT_MOC_LITERAL(8, 98, 12), // "slotAtt1Down"
QT_MOC_LITERAL(9, 111, 10), // "slotAtt2Up"
QT_MOC_LITERAL(10, 122, 12) // "slotAtt2Down"

    },
    "Prm300ControlWidget\0signalSetAtt1Value\0"
    "\0value\0signalSetAtt2Value\0signalSetFilter\0"
    "index\0slotAtt1Up\0slotAtt1Down\0slotAtt2Up\0"
    "slotAtt2Down"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Prm300ControlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       5,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   58,    2, 0x08 /* Private */,
       8,    0,   59,    2, 0x08 /* Private */,
       9,    0,   60,    2, 0x08 /* Private */,
      10,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Prm300ControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Prm300ControlWidget *_t = static_cast<Prm300ControlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSetAtt1Value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signalSetAtt2Value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signalSetFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotAtt1Up(); break;
        case 4: _t->slotAtt1Down(); break;
        case 5: _t->slotAtt2Up(); break;
        case 6: _t->slotAtt2Down(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Prm300ControlWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Prm300ControlWidget::signalSetAtt1Value)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Prm300ControlWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Prm300ControlWidget::signalSetAtt2Value)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Prm300ControlWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Prm300ControlWidget::signalSetFilter)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Prm300ControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Prm300ControlWidget.data,
      qt_meta_data_Prm300ControlWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Prm300ControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Prm300ControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Prm300ControlWidget.stringdata0))
        return static_cast<void*>(const_cast< Prm300ControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Prm300ControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Prm300ControlWidget::signalSetAtt1Value(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Prm300ControlWidget::signalSetAtt2Value(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Prm300ControlWidget::signalSetFilter(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
