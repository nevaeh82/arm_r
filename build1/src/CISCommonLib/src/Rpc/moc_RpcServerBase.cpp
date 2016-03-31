/****************************************************************************
** Meta object code from reading C++ file 'RpcServerBase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Rpc/RpcServerBase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcServerBase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcServerBase_t {
    QByteArrayData data[8];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcServerBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcServerBase_t qt_meta_stringdata_RpcServerBase = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RpcServerBase"
QT_MOC_LITERAL(1, 14, 12), // "methodCalled"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "method"
QT_MOC_LITERAL(4, 35, 8), // "argument"
QT_MOC_LITERAL(5, 44, 13), // "IRpcListener*"
QT_MOC_LITERAL(6, 58, 6), // "sender"
QT_MOC_LITERAL(7, 65, 12) // "internalCall"

    },
    "RpcServerBase\0methodCalled\0\0method\0"
    "argument\0IRpcListener*\0sender\0"
    "internalCall"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcServerBase[] = {

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
       1,    3,   34,    2, 0x06 /* Public */,
       1,    2,   41,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       7,    3,   46,    2, 0x09 /* Protected */,
       7,    2,   53,    2, 0x29 /* Protected | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    3,    4,

       0        // eod
};

void RpcServerBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcServerBase *_t = static_cast<RpcServerBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->methodCalled((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< IRpcListener*(*)>(_a[3]))); break;
        case 1: _t->methodCalled((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 2: _t->internalCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< IRpcListener*(*)>(_a[3]))); break;
        case 3: _t->internalCall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcServerBase::*_t)(const QString & , const QVariant & , IRpcListener * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcServerBase::methodCalled)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RpcServerBase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RpcServerBase.data,
      qt_meta_data_RpcServerBase,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcServerBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcServerBase::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcServerBase.stringdata0))
        return static_cast<void*>(const_cast< RpcServerBase*>(this));
    if (!strcmp(_clname, "IRpcControllerBase"))
        return static_cast< IRpcControllerBase*>(const_cast< RpcServerBase*>(this));
    return QObject::qt_metacast(_clname);
}

int RpcServerBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void RpcServerBase::methodCalled(const QString & _t1, const QVariant & _t2, IRpcListener * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
