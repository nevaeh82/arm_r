/****************************************************************************
** Meta object code from reading C++ file 'TreeWidgetDelegate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/SettingsTree/TreeWidgetDelegate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TreeWidgetDelegate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TreeWidgetDelegate_t {
    QByteArrayData data[7];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TreeWidgetDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TreeWidgetDelegate_t qt_meta_stringdata_TreeWidgetDelegate = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TreeWidgetDelegate"
QT_MOC_LITERAL(1, 19, 21), // "connectToDeviceSignal"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 10), // "deviceName"
QT_MOC_LITERAL(4, 53, 26), // "disconnectFromDeviceSignal"
QT_MOC_LITERAL(5, 80, 18), // "commitSBEditorSlot"
QT_MOC_LITERAL(6, 99, 21) // "onCurrentIndexChanged"

    },
    "TreeWidgetDelegate\0connectToDeviceSignal\0"
    "\0deviceName\0disconnectFromDeviceSignal\0"
    "commitSBEditorSlot\0onCurrentIndexChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TreeWidgetDelegate[] = {

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
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   40,    2, 0x08 /* Private */,
       6,    1,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void TreeWidgetDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TreeWidgetDelegate *_t = static_cast<TreeWidgetDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectToDeviceSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->disconnectFromDeviceSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->commitSBEditorSlot(); break;
        case 3: _t->onCurrentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TreeWidgetDelegate::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TreeWidgetDelegate::connectToDeviceSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TreeWidgetDelegate::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TreeWidgetDelegate::disconnectFromDeviceSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TreeWidgetDelegate::staticMetaObject = {
    { &QStyledItemDelegate::staticMetaObject, qt_meta_stringdata_TreeWidgetDelegate.data,
      qt_meta_data_TreeWidgetDelegate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TreeWidgetDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TreeWidgetDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TreeWidgetDelegate.stringdata0))
        return static_cast<void*>(const_cast< TreeWidgetDelegate*>(this));
    return QStyledItemDelegate::qt_metacast(_clname);
}

int TreeWidgetDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStyledItemDelegate::qt_metacall(_c, _id, _a);
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
void TreeWidgetDelegate::connectToDeviceSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TreeWidgetDelegate::disconnectFromDeviceSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
