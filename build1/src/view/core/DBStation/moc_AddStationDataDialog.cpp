/****************************************************************************
** Meta object code from reading C++ file 'AddStationDataDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/view/core/DBStation/AddStationDataDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddStationDataDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AddStationDataDialog_t {
    QByteArrayData data[7];
    char stringdata0[61];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AddStationDataDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AddStationDataDialog_t qt_meta_stringdata_AddStationDataDialog = {
    {
QT_MOC_LITERAL(0, 0, 20), // "AddStationDataDialog"
QT_MOC_LITERAL(1, 21, 8), // "accepted"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "StationData"
QT_MOC_LITERAL(4, 43, 4), // "data"
QT_MOC_LITERAL(5, 48, 6), // "accept"
QT_MOC_LITERAL(6, 55, 5) // "clear"

    },
    "AddStationDataDialog\0accepted\0\0"
    "StationData\0data\0accept\0clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AddStationDataDialog[] = {

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
       5,    0,   32,    2, 0x08 /* Private */,
       6,    0,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AddStationDataDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AddStationDataDialog *_t = static_cast<AddStationDataDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->accepted((*reinterpret_cast< const StationData(*)>(_a[1]))); break;
        case 1: _t->accept(); break;
        case 2: _t->clear(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AddStationDataDialog::*_t)(const StationData & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AddStationDataDialog::accepted)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AddStationDataDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AddStationDataDialog.data,
      qt_meta_data_AddStationDataDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AddStationDataDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AddStationDataDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AddStationDataDialog.stringdata0))
        return static_cast<void*>(const_cast< AddStationDataDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int AddStationDataDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void AddStationDataDialog::accepted(const StationData & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
