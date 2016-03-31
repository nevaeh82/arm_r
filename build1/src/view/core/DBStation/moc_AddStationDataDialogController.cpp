/****************************************************************************
** Meta object code from reading C++ file 'AddStationDataDialogController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/DBStation/AddStationDataDialogController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddStationDataDialogController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AddStationDataDialogController_t {
    QByteArrayData data[5];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AddStationDataDialogController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AddStationDataDialogController_t qt_meta_stringdata_AddStationDataDialogController = {
    {
QT_MOC_LITERAL(0, 0, 30), // "AddStationDataDialogController"
QT_MOC_LITERAL(1, 31, 6), // "insert"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "StationData"
QT_MOC_LITERAL(4, 51, 4) // "data"

    },
    "AddStationDataDialogController\0insert\0"
    "\0StationData\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AddStationDataDialogController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void AddStationDataDialogController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AddStationDataDialogController *_t = static_cast<AddStationDataDialogController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->insert((*reinterpret_cast< const StationData(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AddStationDataDialogController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AddStationDataDialogController.data,
      qt_meta_data_AddStationDataDialogController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AddStationDataDialogController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AddStationDataDialogController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AddStationDataDialogController.stringdata0))
        return static_cast<void*>(const_cast< AddStationDataDialogController*>(this));
    if (!strcmp(_clname, "IController<AddStationDataDialog>"))
        return static_cast< IController<AddStationDataDialog>*>(const_cast< AddStationDataDialogController*>(this));
    return QObject::qt_metacast(_clname);
}

int AddStationDataDialogController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
