/****************************************************************************
** Meta object code from reading C++ file 'RpcPeerAbstract.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/Rpc/RpcPeerAbstract.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcPeerAbstract.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcPeerAbstract_t {
    QByteArrayData data[26];
    char stringdata0[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcPeerAbstract_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcPeerAbstract_t qt_meta_stringdata_RpcPeerAbstract = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RpcPeerAbstract"
QT_MOC_LITERAL(1, 16, 15), // "clientConnected"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 2), // "id"
QT_MOC_LITERAL(4, 36, 18), // "clientDisconnected"
QT_MOC_LITERAL(5, 55, 17), // "connectedToServer"
QT_MOC_LITERAL(6, 73, 22), // "disconnectedFromServer"
QT_MOC_LITERAL(7, 96, 11), // "serverError"
QT_MOC_LITERAL(8, 108, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(9, 137, 16), // "disconnectClient"
QT_MOC_LITERAL(10, 154, 16), // "disconnectServer"
QT_MOC_LITERAL(11, 171, 13), // "disconnectAll"
QT_MOC_LITERAL(12, 185, 4), // "call"
QT_MOC_LITERAL(13, 190, 2), // "fn"
QT_MOC_LITERAL(14, 193, 2), // "p1"
QT_MOC_LITERAL(15, 196, 2), // "p2"
QT_MOC_LITERAL(16, 199, 2), // "p3"
QT_MOC_LITERAL(17, 202, 2), // "p4"
QT_MOC_LITERAL(18, 205, 2), // "p5"
QT_MOC_LITERAL(19, 208, 2), // "p6"
QT_MOC_LITERAL(20, 211, 2), // "p7"
QT_MOC_LITERAL(21, 214, 2), // "p8"
QT_MOC_LITERAL(22, 217, 14), // "QList<quint64>"
QT_MOC_LITERAL(23, 232, 3), // "ids"
QT_MOC_LITERAL(24, 236, 10), // "callExcept"
QT_MOC_LITERAL(25, 247, 12) // "detachSender"

    },
    "RpcPeerAbstract\0clientConnected\0\0id\0"
    "clientDisconnected\0connectedToServer\0"
    "disconnectedFromServer\0serverError\0"
    "QAbstractSocket::SocketError\0"
    "disconnectClient\0disconnectServer\0"
    "disconnectAll\0call\0fn\0p1\0p2\0p3\0p4\0p5\0"
    "p6\0p7\0p8\0QList<quint64>\0ids\0callExcept\0"
    "detachSender"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcPeerAbstract[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      45,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  239,    2, 0x06 /* Public */,
       4,    1,  242,    2, 0x06 /* Public */,
       5,    0,  245,    2, 0x06 /* Public */,
       6,    0,  246,    2, 0x06 /* Public */,
       7,    1,  247,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,  250,    2, 0x0a /* Public */,
      10,    0,  253,    2, 0x0a /* Public */,
      11,    0,  254,    2, 0x0a /* Public */,
      12,    9,  255,    2, 0x0a /* Public */,
      12,    8,  274,    2, 0x2a /* Public | MethodCloned */,
      12,    7,  291,    2, 0x2a /* Public | MethodCloned */,
      12,    6,  306,    2, 0x2a /* Public | MethodCloned */,
      12,    5,  319,    2, 0x2a /* Public | MethodCloned */,
      12,    4,  330,    2, 0x2a /* Public | MethodCloned */,
      12,    3,  339,    2, 0x2a /* Public | MethodCloned */,
      12,    2,  346,    2, 0x2a /* Public | MethodCloned */,
      12,    1,  351,    2, 0x2a /* Public | MethodCloned */,
      12,   10,  354,    2, 0x0a /* Public */,
      12,    9,  375,    2, 0x2a /* Public | MethodCloned */,
      12,    8,  394,    2, 0x2a /* Public | MethodCloned */,
      12,    7,  411,    2, 0x2a /* Public | MethodCloned */,
      12,    6,  426,    2, 0x2a /* Public | MethodCloned */,
      12,    5,  439,    2, 0x2a /* Public | MethodCloned */,
      12,    4,  450,    2, 0x2a /* Public | MethodCloned */,
      12,    3,  459,    2, 0x2a /* Public | MethodCloned */,
      12,    2,  466,    2, 0x2a /* Public | MethodCloned */,
      12,   10,  471,    2, 0x0a /* Public */,
      12,    9,  492,    2, 0x2a /* Public | MethodCloned */,
      12,    8,  511,    2, 0x2a /* Public | MethodCloned */,
      12,    7,  528,    2, 0x2a /* Public | MethodCloned */,
      12,    6,  543,    2, 0x2a /* Public | MethodCloned */,
      12,    5,  556,    2, 0x2a /* Public | MethodCloned */,
      12,    4,  567,    2, 0x2a /* Public | MethodCloned */,
      12,    3,  576,    2, 0x2a /* Public | MethodCloned */,
      12,    2,  583,    2, 0x2a /* Public | MethodCloned */,
      24,   10,  588,    2, 0x0a /* Public */,
      24,    9,  609,    2, 0x2a /* Public | MethodCloned */,
      24,    8,  628,    2, 0x2a /* Public | MethodCloned */,
      24,    7,  645,    2, 0x2a /* Public | MethodCloned */,
      24,    6,  660,    2, 0x2a /* Public | MethodCloned */,
      24,    5,  673,    2, 0x2a /* Public | MethodCloned */,
      24,    4,  684,    2, 0x2a /* Public | MethodCloned */,
      24,    3,  693,    2, 0x2a /* Public | MethodCloned */,
      24,    2,  700,    2, 0x2a /* Public | MethodCloned */,
      25,    0,  705,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::ULongLong,    3,
    QMetaType::Void, QMetaType::ULongLong,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::ULongLong,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,   16,   17,   18,   19,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,   16,   17,   18,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,   16,   17,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant,   13,   14,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,   13,   14,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,   16,   17,   18,   19,   20,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,   16,   17,   18,   19,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,   16,   17,   18,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,   16,   17,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,   16,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant,   23,   13,   14,   15,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString, QMetaType::QVariant,   23,   13,   14,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QString,   23,   13,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,   19,   20,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,   19,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant,    3,   13,   14,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    3,   13,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,   19,   20,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,   19,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,   18,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant,    3,   13,   14,   15,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QVariant,    3,   13,   14,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString,    3,   13,
    QMetaType::Void,

       0        // eod
};

void RpcPeerAbstract::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcPeerAbstract *_t = static_cast<RpcPeerAbstract *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clientConnected((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 1: _t->clientDisconnected((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 2: _t->connectedToServer(); break;
        case 3: _t->disconnectedFromServer(); break;
        case 4: _t->serverError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->disconnectClient((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 6: _t->disconnectServer(); break;
        case 7: _t->disconnectAll(); break;
        case 8: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 9: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 10: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 11: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 12: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 13: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 14: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 15: _t->call((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 16: _t->call((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 18: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 19: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 20: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 21: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 22: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 23: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 24: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 25: _t->call((*reinterpret_cast< QList<quint64>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 26: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 27: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 28: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 29: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 30: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 31: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 32: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 33: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 34: _t->call((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 35: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 36: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 37: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 38: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 39: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 40: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 41: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 42: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 43: _t->callExcept((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 44: _t->detachSender(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 20:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 21:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 23:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 24:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        case 25:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint64> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcPeerAbstract::*_t)(quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPeerAbstract::clientConnected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RpcPeerAbstract::*_t)(quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPeerAbstract::clientDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RpcPeerAbstract::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPeerAbstract::connectedToServer)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RpcPeerAbstract::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPeerAbstract::disconnectedFromServer)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RpcPeerAbstract::*_t)(QAbstractSocket::SocketError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcPeerAbstract::serverError)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject RpcPeerAbstract::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RpcPeerAbstract.data,
      qt_meta_data_RpcPeerAbstract,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcPeerAbstract::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcPeerAbstract::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcPeerAbstract.stringdata0))
        return static_cast<void*>(const_cast< RpcPeerAbstract*>(this));
    return QObject::qt_metacast(_clname);
}

int RpcPeerAbstract::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 45)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 45;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 45)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 45;
    }
    return _id;
}

// SIGNAL 0
void RpcPeerAbstract::clientConnected(quint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RpcPeerAbstract::clientDisconnected(quint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RpcPeerAbstract::connectedToServer()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RpcPeerAbstract::disconnectedFromServer()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void RpcPeerAbstract::serverError(QAbstractSocket::SocketError _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
