/****************************************************************************
** Meta object code from reading C++ file 'qxtlogger.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Qxt/qxtlogger.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxtlogger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QxtLogger_t {
    QByteArrayData data[29];
    char stringdata0[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxtLogger_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxtLogger_t qt_meta_stringdata_QxtLogger = {
    {
QT_MOC_LITERAL(0, 0, 9), // "QxtLogger"
QT_MOC_LITERAL(1, 10, 17), // "loggerEngineAdded"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10), // "engineName"
QT_MOC_LITERAL(4, 40, 19), // "loggerEngineRemoved"
QT_MOC_LITERAL(5, 60, 19), // "loggerEngineEnabled"
QT_MOC_LITERAL(6, 80, 20), // "loggerEngineDisabled"
QT_MOC_LITERAL(7, 101, 4), // "info"
QT_MOC_LITERAL(8, 106, 7), // "message"
QT_MOC_LITERAL(9, 114, 4), // "msg1"
QT_MOC_LITERAL(10, 119, 4), // "msg2"
QT_MOC_LITERAL(11, 124, 4), // "msg3"
QT_MOC_LITERAL(12, 129, 4), // "msg4"
QT_MOC_LITERAL(13, 134, 4), // "msg5"
QT_MOC_LITERAL(14, 139, 4), // "msg6"
QT_MOC_LITERAL(15, 144, 4), // "msg7"
QT_MOC_LITERAL(16, 149, 4), // "msg8"
QT_MOC_LITERAL(17, 154, 4), // "msg9"
QT_MOC_LITERAL(18, 159, 5), // "trace"
QT_MOC_LITERAL(19, 165, 7), // "warning"
QT_MOC_LITERAL(20, 173, 5), // "error"
QT_MOC_LITERAL(21, 179, 5), // "debug"
QT_MOC_LITERAL(22, 185, 8), // "critical"
QT_MOC_LITERAL(23, 194, 5), // "fatal"
QT_MOC_LITERAL(24, 200, 5), // "write"
QT_MOC_LITERAL(25, 206, 4), // "args"
QT_MOC_LITERAL(26, 211, 3), // "log"
QT_MOC_LITERAL(27, 215, 8), // "LogLevel"
QT_MOC_LITERAL(28, 224, 5) // "level"

    },
    "QxtLogger\0loggerEngineAdded\0\0engineName\0"
    "loggerEngineRemoved\0loggerEngineEnabled\0"
    "loggerEngineDisabled\0info\0message\0"
    "msg1\0msg2\0msg3\0msg4\0msg5\0msg6\0msg7\0"
    "msg8\0msg9\0trace\0warning\0error\0debug\0"
    "critical\0fatal\0write\0args\0log\0LogLevel\0"
    "level"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxtLogger[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      93,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  479,    2, 0x06 /* Public */,
       4,    1,  482,    2, 0x06 /* Public */,
       5,    1,  485,    2, 0x06 /* Public */,
       6,    1,  488,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,   10,  491,    2, 0x0a /* Public */,
       7,    9,  512,    2, 0x2a /* Public | MethodCloned */,
       7,    8,  531,    2, 0x2a /* Public | MethodCloned */,
       7,    7,  548,    2, 0x2a /* Public | MethodCloned */,
       7,    6,  563,    2, 0x2a /* Public | MethodCloned */,
       7,    5,  576,    2, 0x2a /* Public | MethodCloned */,
       7,    4,  587,    2, 0x2a /* Public | MethodCloned */,
       7,    3,  596,    2, 0x2a /* Public | MethodCloned */,
       7,    2,  603,    2, 0x2a /* Public | MethodCloned */,
       7,    1,  608,    2, 0x2a /* Public | MethodCloned */,
      18,   10,  611,    2, 0x0a /* Public */,
      18,    9,  632,    2, 0x2a /* Public | MethodCloned */,
      18,    8,  651,    2, 0x2a /* Public | MethodCloned */,
      18,    7,  668,    2, 0x2a /* Public | MethodCloned */,
      18,    6,  683,    2, 0x2a /* Public | MethodCloned */,
      18,    5,  696,    2, 0x2a /* Public | MethodCloned */,
      18,    4,  707,    2, 0x2a /* Public | MethodCloned */,
      18,    3,  716,    2, 0x2a /* Public | MethodCloned */,
      18,    2,  723,    2, 0x2a /* Public | MethodCloned */,
      18,    1,  728,    2, 0x2a /* Public | MethodCloned */,
      19,   10,  731,    2, 0x0a /* Public */,
      19,    9,  752,    2, 0x2a /* Public | MethodCloned */,
      19,    8,  771,    2, 0x2a /* Public | MethodCloned */,
      19,    7,  788,    2, 0x2a /* Public | MethodCloned */,
      19,    6,  803,    2, 0x2a /* Public | MethodCloned */,
      19,    5,  816,    2, 0x2a /* Public | MethodCloned */,
      19,    4,  827,    2, 0x2a /* Public | MethodCloned */,
      19,    3,  836,    2, 0x2a /* Public | MethodCloned */,
      19,    2,  843,    2, 0x2a /* Public | MethodCloned */,
      19,    1,  848,    2, 0x2a /* Public | MethodCloned */,
      20,   10,  851,    2, 0x0a /* Public */,
      20,    9,  872,    2, 0x2a /* Public | MethodCloned */,
      20,    8,  891,    2, 0x2a /* Public | MethodCloned */,
      20,    7,  908,    2, 0x2a /* Public | MethodCloned */,
      20,    6,  923,    2, 0x2a /* Public | MethodCloned */,
      20,    5,  936,    2, 0x2a /* Public | MethodCloned */,
      20,    4,  947,    2, 0x2a /* Public | MethodCloned */,
      20,    3,  956,    2, 0x2a /* Public | MethodCloned */,
      20,    2,  963,    2, 0x2a /* Public | MethodCloned */,
      20,    1,  968,    2, 0x2a /* Public | MethodCloned */,
      21,   10,  971,    2, 0x0a /* Public */,
      21,    9,  992,    2, 0x2a /* Public | MethodCloned */,
      21,    8, 1011,    2, 0x2a /* Public | MethodCloned */,
      21,    7, 1028,    2, 0x2a /* Public | MethodCloned */,
      21,    6, 1043,    2, 0x2a /* Public | MethodCloned */,
      21,    5, 1056,    2, 0x2a /* Public | MethodCloned */,
      21,    4, 1067,    2, 0x2a /* Public | MethodCloned */,
      21,    3, 1076,    2, 0x2a /* Public | MethodCloned */,
      21,    2, 1083,    2, 0x2a /* Public | MethodCloned */,
      21,    1, 1088,    2, 0x2a /* Public | MethodCloned */,
      22,   10, 1091,    2, 0x0a /* Public */,
      22,    9, 1112,    2, 0x2a /* Public | MethodCloned */,
      22,    8, 1131,    2, 0x2a /* Public | MethodCloned */,
      22,    7, 1148,    2, 0x2a /* Public | MethodCloned */,
      22,    6, 1163,    2, 0x2a /* Public | MethodCloned */,
      22,    5, 1176,    2, 0x2a /* Public | MethodCloned */,
      22,    4, 1187,    2, 0x2a /* Public | MethodCloned */,
      22,    3, 1196,    2, 0x2a /* Public | MethodCloned */,
      22,    2, 1203,    2, 0x2a /* Public | MethodCloned */,
      22,    1, 1208,    2, 0x2a /* Public | MethodCloned */,
      23,   10, 1211,    2, 0x0a /* Public */,
      23,    9, 1232,    2, 0x2a /* Public | MethodCloned */,
      23,    8, 1251,    2, 0x2a /* Public | MethodCloned */,
      23,    7, 1268,    2, 0x2a /* Public | MethodCloned */,
      23,    6, 1283,    2, 0x2a /* Public | MethodCloned */,
      23,    5, 1296,    2, 0x2a /* Public | MethodCloned */,
      23,    4, 1307,    2, 0x2a /* Public | MethodCloned */,
      23,    3, 1316,    2, 0x2a /* Public | MethodCloned */,
      23,    2, 1323,    2, 0x2a /* Public | MethodCloned */,
      23,    1, 1328,    2, 0x2a /* Public | MethodCloned */,
      24,   10, 1331,    2, 0x0a /* Public */,
      24,    9, 1352,    2, 0x2a /* Public | MethodCloned */,
      24,    8, 1371,    2, 0x2a /* Public | MethodCloned */,
      24,    7, 1388,    2, 0x2a /* Public | MethodCloned */,
      24,    6, 1403,    2, 0x2a /* Public | MethodCloned */,
      24,    5, 1416,    2, 0x2a /* Public | MethodCloned */,
      24,    4, 1427,    2, 0x2a /* Public | MethodCloned */,
      24,    3, 1436,    2, 0x2a /* Public | MethodCloned */,
      24,    2, 1443,    2, 0x2a /* Public | MethodCloned */,
      24,    1, 1448,    2, 0x2a /* Public | MethodCloned */,
       7,    1, 1451,    2, 0x0a /* Public */,
      18,    1, 1454,    2, 0x0a /* Public */,
      19,    1, 1457,    2, 0x0a /* Public */,
      20,    1, 1460,    2, 0x0a /* Public */,
      21,    1, 1463,    2, 0x0a /* Public */,
      22,    1, 1466,    2, 0x0a /* Public */,
      23,    1, 1469,    2, 0x0a /* Public */,
      24,    1, 1472,    2, 0x0a /* Public */,
      26,    2, 1475,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,   12,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant, QMetaType::QVariant,    8,    9,   10,
    QMetaType::Void, QMetaType::QVariant, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::QVariant,    8,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, QMetaType::QVariantList,   25,
    QMetaType::Void, 0x80000000 | 27, QMetaType::QVariantList,   28,   25,

       0        // eod
};

void QxtLogger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QxtLogger *_t = static_cast<QxtLogger *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loggerEngineAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->loggerEngineRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->loggerEngineEnabled((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->loggerEngineDisabled((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 5: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 6: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 7: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 8: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 9: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 10: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 11: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 12: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 13: _t->info((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 14: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 15: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 16: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 17: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 18: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 19: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 20: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 21: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 22: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 23: _t->trace((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 24: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 25: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 26: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 27: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 28: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 29: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 30: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 31: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 32: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 33: _t->warning((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 34: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 35: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 36: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 37: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 38: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 39: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 40: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 41: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 42: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 43: _t->error((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 44: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 45: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 46: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 47: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 48: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 49: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 50: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 51: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 52: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 53: _t->debug((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 54: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 55: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 56: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 57: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 58: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 59: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 60: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 61: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 62: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 63: _t->critical((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 64: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 65: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 66: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 67: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 68: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 69: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 70: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 71: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 72: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 73: _t->fatal((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 74: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9])),(*reinterpret_cast< const QVariant(*)>(_a[10]))); break;
        case 75: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8])),(*reinterpret_cast< const QVariant(*)>(_a[9]))); break;
        case 76: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7])),(*reinterpret_cast< const QVariant(*)>(_a[8]))); break;
        case 77: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6])),(*reinterpret_cast< const QVariant(*)>(_a[7]))); break;
        case 78: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5])),(*reinterpret_cast< const QVariant(*)>(_a[6]))); break;
        case 79: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 80: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 81: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 82: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 83: _t->write((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 84: _t->info((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 85: _t->trace((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 86: _t->warning((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 87: _t->error((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 88: _t->debug((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 89: _t->critical((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 90: _t->fatal((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 91: _t->write((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 92: _t->log((*reinterpret_cast< LogLevel(*)>(_a[1])),(*reinterpret_cast< const QList<QVariant>(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QxtLogger::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtLogger::loggerEngineAdded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QxtLogger::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtLogger::loggerEngineRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QxtLogger::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtLogger::loggerEngineEnabled)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QxtLogger::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QxtLogger::loggerEngineDisabled)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject QxtLogger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QxtLogger.data,
      qt_meta_data_QxtLogger,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QxtLogger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxtLogger::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QxtLogger.stringdata0))
        return static_cast<void*>(const_cast< QxtLogger*>(this));
    return QObject::qt_metacast(_clname);
}

int QxtLogger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 93)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 93;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 93)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 93;
    }
    return _id;
}

// SIGNAL 0
void QxtLogger::loggerEngineAdded(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QxtLogger::loggerEngineRemoved(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QxtLogger::loggerEngineEnabled(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QxtLogger::loggerEngineDisabled(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
