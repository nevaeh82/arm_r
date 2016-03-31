/****************************************************************************
** Meta object code from reading C++ file 'RpcFlakonClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/view/core/Tabs/RPC/RpcFlakonClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcFlakonClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcFlakonClient_t {
    QByteArrayData data[13];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcFlakonClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcFlakonClient_t qt_meta_stringdata_RpcFlakonClient = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RpcFlakonClient"
QT_MOC_LITERAL(1, 16, 23), // "signalEnableCorrelation"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 13), // "registerRoute"
QT_MOC_LITERAL(4, 55, 14), // "pointsReceived"
QT_MOC_LITERAL(5, 70, 4), // "data"
QT_MOC_LITERAL(6, 75, 17), // "bandwidthReceived"
QT_MOC_LITERAL(7, 93, 19), // "correlationReceived"
QT_MOC_LITERAL(8, 113, 20), // "flakonStatusReceived"
QT_MOC_LITERAL(9, 134, 20), // "sloverResultReceived"
QT_MOC_LITERAL(10, 155, 21), // "slotEnableCorrelation"
QT_MOC_LITERAL(11, 177, 27), // "solverQualityStatusReceived"
QT_MOC_LITERAL(12, 205, 20) // "solverErrorsReceived"

    },
    "RpcFlakonClient\0signalEnableCorrelation\0"
    "\0registerRoute\0pointsReceived\0data\0"
    "bandwidthReceived\0correlationReceived\0"
    "flakonStatusReceived\0sloverResultReceived\0"
    "slotEnableCorrelation\0solverQualityStatusReceived\0"
    "solverErrorsReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcFlakonClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   71,    2, 0x09 /* Protected */,
       4,    1,   72,    2, 0x09 /* Protected */,
       6,    1,   75,    2, 0x09 /* Protected */,
       7,    1,   78,    2, 0x09 /* Protected */,
       8,    1,   81,    2, 0x09 /* Protected */,
       9,    1,   84,    2, 0x09 /* Protected */,
      10,    3,   87,    2, 0x08 /* Private */,
      11,    1,   94,    2, 0x08 /* Private */,
      12,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Bool,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Bool,    2,    2,    2,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QByteArray,    5,

       0        // eod
};

void RpcFlakonClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcFlakonClient *_t = static_cast<RpcFlakonClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalEnableCorrelation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->registerRoute(); break;
        case 2: _t->pointsReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->bandwidthReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->correlationReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->flakonStatusReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->sloverResultReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->slotEnableCorrelation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 8: _t->solverQualityStatusReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 9: _t->solverErrorsReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcFlakonClient::*_t)(int , float , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcFlakonClient::signalEnableCorrelation)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RpcFlakonClient::staticMetaObject = {
    { &RpcRoutedClient::staticMetaObject, qt_meta_stringdata_RpcFlakonClient.data,
      qt_meta_data_RpcFlakonClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcFlakonClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcFlakonClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcFlakonClient.stringdata0))
        return static_cast<void*>(const_cast< RpcFlakonClient*>(this));
    return RpcRoutedClient::qt_metacast(_clname);
}

int RpcFlakonClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RpcRoutedClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void RpcFlakonClient::signalEnableCorrelation(int _t1, float _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
