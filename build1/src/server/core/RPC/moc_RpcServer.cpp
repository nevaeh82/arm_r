/****************************************************************************
** Meta object code from reading C++ file 'RpcServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/server/core/RPC/RpcServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcServer_t {
    QByteArrayData data[45];
    char stringdata0[609];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcServer_t qt_meta_stringdata_RpcServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RpcServer"
QT_MOC_LITERAL(1, 10, 18), // "logConnectionError"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(4, 59, 11), // "socketError"
QT_MOC_LITERAL(5, 71, 20), // "logConnectionSuccess"
QT_MOC_LITERAL(6, 92, 6), // "client"
QT_MOC_LITERAL(7, 99, 20), // "logClientDisconected"
QT_MOC_LITERAL(8, 120, 25), // "requestGetStationListSlot"
QT_MOC_LITERAL(9, 146, 14), // "configFilename"
QT_MOC_LITERAL(10, 161, 33), // "requestGetAtlantConfiguration..."
QT_MOC_LITERAL(11, 195, 29), // "requestGetDbConfigurationSlot"
QT_MOC_LITERAL(12, 225, 25), // "setMainStationCorrelation"
QT_MOC_LITERAL(13, 251, 2), // "id"
QT_MOC_LITERAL(14, 254, 7), // "station"
QT_MOC_LITERAL(15, 262, 12), // "setBandwidth"
QT_MOC_LITERAL(16, 275, 9), // "bandwidth"
QT_MOC_LITERAL(17, 285, 8), // "setShift"
QT_MOC_LITERAL(18, 294, 5), // "shift"
QT_MOC_LITERAL(19, 300, 9), // "setCenter"
QT_MOC_LITERAL(20, 310, 6), // "center"
QT_MOC_LITERAL(21, 317, 9), // "recognize"
QT_MOC_LITERAL(22, 327, 13), // "ssCorrelation"
QT_MOC_LITERAL(23, 341, 9), // "frequency"
QT_MOC_LITERAL(24, 351, 6), // "enable"
QT_MOC_LITERAL(25, 358, 18), // "setAvarageSpectrum"
QT_MOC_LITERAL(26, 377, 7), // "avarage"
QT_MOC_LITERAL(27, 385, 15), // "setPrmFrequency"
QT_MOC_LITERAL(28, 401, 4), // "name"
QT_MOC_LITERAL(29, 406, 4), // "freq"
QT_MOC_LITERAL(30, 411, 19), // "requestPrmFrequency"
QT_MOC_LITERAL(31, 431, 10), // "setPrmAtt1"
QT_MOC_LITERAL(32, 442, 5), // "value"
QT_MOC_LITERAL(33, 448, 10), // "setPrmAtt2"
QT_MOC_LITERAL(34, 459, 12), // "setPrmFilter"
QT_MOC_LITERAL(35, 472, 5), // "index"
QT_MOC_LITERAL(36, 478, 15), // "setDataToSolver"
QT_MOC_LITERAL(37, 494, 4), // "data"
QT_MOC_LITERAL(38, 499, 16), // "setClearToSolver"
QT_MOC_LITERAL(39, 516, 18), // "setAtlantFrequency"
QT_MOC_LITERAL(40, 535, 5), // "clint"
QT_MOC_LITERAL(41, 541, 13), // "requestStatus"
QT_MOC_LITERAL(42, 555, 19), // "requestFlakonStatus"
QT_MOC_LITERAL(43, 575, 22), // "sendResponseModulation"
QT_MOC_LITERAL(44, 598, 10) // "modulation"

    },
    "RpcServer\0logConnectionError\0\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "logConnectionSuccess\0client\0"
    "logClientDisconected\0requestGetStationListSlot\0"
    "configFilename\0requestGetAtlantConfigurationSlot\0"
    "requestGetDbConfigurationSlot\0"
    "setMainStationCorrelation\0id\0station\0"
    "setBandwidth\0bandwidth\0setShift\0shift\0"
    "setCenter\0center\0recognize\0ssCorrelation\0"
    "frequency\0enable\0setAvarageSpectrum\0"
    "avarage\0setPrmFrequency\0name\0freq\0"
    "requestPrmFrequency\0setPrmAtt1\0value\0"
    "setPrmAtt2\0setPrmFilter\0index\0"
    "setDataToSolver\0data\0setClearToSolver\0"
    "setAtlantFrequency\0clint\0requestStatus\0"
    "requestFlakonStatus\0sendResponseModulation\0"
    "modulation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  134,    2, 0x08 /* Private */,
       5,    1,  137,    2, 0x08 /* Private */,
       7,    1,  140,    2, 0x08 /* Private */,
       8,    2,  143,    2, 0x08 /* Private */,
      10,    2,  148,    2, 0x08 /* Private */,
      11,    2,  153,    2, 0x08 /* Private */,
      12,    3,  158,    2, 0x08 /* Private */,
      15,    3,  165,    2, 0x08 /* Private */,
      17,    3,  172,    2, 0x08 /* Private */,
      19,    3,  179,    2, 0x08 /* Private */,
      21,    3,  186,    2, 0x08 /* Private */,
      22,    4,  193,    2, 0x08 /* Private */,
      25,    3,  202,    2, 0x08 /* Private */,
      27,    3,  209,    2, 0x08 /* Private */,
      30,    2,  216,    2, 0x08 /* Private */,
      31,    3,  221,    2, 0x08 /* Private */,
      33,    3,  228,    2, 0x08 /* Private */,
      34,    3,  235,    2, 0x08 /* Private */,
      36,    2,  242,    2, 0x08 /* Private */,
      38,    2,  247,    2, 0x08 /* Private */,
      39,    2,  252,    2, 0x08 /* Private */,
      41,    2,  257,    2, 0x08 /* Private */,
      42,    1,  262,    2, 0x08 /* Private */,
      43,    2,  265,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::ULongLong,    6,
    QMetaType::Void, QMetaType::ULongLong,    6,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    6,    9,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::QString,    6,   13,   14,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::Float,    6,   13,   16,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::Float,    6,   13,   18,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::Float,    6,   13,   20,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::Int,    6,   13,    2,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::Float, QMetaType::Bool,    6,   13,   23,   24,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Int, QMetaType::Int,    6,   13,   26,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::Short,    6,   28,   29,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    6,   28,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::Int,    6,   28,   32,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::Int,    6,   28,   32,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::Int,    6,   28,   35,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QByteArray,    6,   37,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QByteArray,    6,   37,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QByteArray,   40,   37,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    6,   28,
    QMetaType::Void, QMetaType::ULongLong,    6,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    6,   44,

       0        // eod
};

void RpcServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcServer *_t = static_cast<RpcServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->logConnectionError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 1: _t->logConnectionSuccess((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 2: _t->logClientDisconected((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 3: _t->requestGetStationListSlot((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->requestGetAtlantConfigurationSlot((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->requestGetDbConfigurationSlot((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->setMainStationCorrelation((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->setBandwidth((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 8: _t->setShift((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 9: _t->setCenter((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 10: _t->recognize((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: _t->ssCorrelation((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 12: _t->setAvarageSpectrum((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 13: _t->setPrmFrequency((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< short(*)>(_a[3]))); break;
        case 14: _t->requestPrmFrequency((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 15: _t->setPrmAtt1((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 16: _t->setPrmAtt2((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 17: _t->setPrmFilter((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 18: _t->setDataToSolver((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 19: _t->setClearToSolver((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 20: _t->setAtlantFrequency((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 21: _t->requestStatus((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 22: _t->requestFlakonStatus((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 23: _t->sendResponseModulation((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject RpcServer::staticMetaObject = {
    { &RpcRoutedServer::staticMetaObject, qt_meta_stringdata_RpcServer.data,
      qt_meta_data_RpcServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcServer.stringdata0))
        return static_cast<void*>(const_cast< RpcServer*>(this));
    return RpcRoutedServer::qt_metacast(_clname);
}

int RpcServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RpcRoutedServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
