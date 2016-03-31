/****************************************************************************
** Meta object code from reading C++ file 'ZCoord.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/server/core/Flakon/Correction/ZCoord.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ZCoord.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ZCoord_t {
    QByteArrayData data[30];
    char stringdata0[302];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ZCoord_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ZCoord_t qt_meta_stringdata_ZCoord = {
    {
QT_MOC_LITERAL(0, 0, 6), // "ZCoord"
QT_MOC_LITERAL(1, 7, 15), // "signal_sendText"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "aText"
QT_MOC_LITERAL(4, 30, 20), // "signal_sendBenchmark"
QT_MOC_LITERAL(5, 51, 14), // "QVector<float>"
QT_MOC_LITERAL(6, 66, 17), // "getPointsDistance"
QT_MOC_LITERAL(7, 84, 11), // "aNumOfPoint"
QT_MOC_LITERAL(8, 96, 5), // "aNumA"
QT_MOC_LITERAL(9, 102, 5), // "aNumB"
QT_MOC_LITERAL(10, 108, 13), // "decartToGeogr"
QT_MOC_LITERAL(11, 122, 12), // "aDecartCoord"
QT_MOC_LITERAL(12, 135, 8), // "QPointF&"
QT_MOC_LITERAL(13, 144, 11), // "lGeogrCoord"
QT_MOC_LITERAL(14, 156, 11), // "aCentrCoord"
QT_MOC_LITERAL(15, 168, 9), // "checkData"
QT_MOC_LITERAL(16, 178, 8), // "aNumMain"
QT_MOC_LITERAL(17, 187, 16), // "QVector<double>&"
QT_MOC_LITERAL(18, 204, 3), // "lDR"
QT_MOC_LITERAL(19, 208, 10), // "aBenchmark"
QT_MOC_LITERAL(20, 219, 10), // "aThreshold"
QT_MOC_LITERAL(21, 230, 10), // "distanceAB"
QT_MOC_LITERAL(22, 241, 7), // "aCoordA"
QT_MOC_LITERAL(23, 249, 5), // "aHgtA"
QT_MOC_LITERAL(24, 255, 7), // "aCoordB"
QT_MOC_LITERAL(25, 263, 5), // "aHgtB"
QT_MOC_LITERAL(26, 269, 8), // "getTitle"
QT_MOC_LITERAL(27, 278, 6), // "aIndex"
QT_MOC_LITERAL(28, 285, 8), // "getCoord"
QT_MOC_LITERAL(29, 294, 7) // "getSize"

    },
    "ZCoord\0signal_sendText\0\0aText\0"
    "signal_sendBenchmark\0QVector<float>\0"
    "getPointsDistance\0aNumOfPoint\0aNumA\0"
    "aNumB\0decartToGeogr\0aDecartCoord\0"
    "QPointF&\0lGeogrCoord\0aCentrCoord\0"
    "checkData\0aNumMain\0QVector<double>&\0"
    "lDR\0aBenchmark\0aThreshold\0distanceAB\0"
    "aCoordA\0aHgtA\0aCoordB\0aHgtB\0getTitle\0"
    "aIndex\0getCoord\0getSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ZCoord[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   70,    2, 0x0a /* Public */,
       6,    2,   73,    2, 0x0a /* Public */,
      10,    3,   78,    2, 0x0a /* Public */,
      15,    4,   85,    2, 0x0a /* Public */,
      21,    4,   94,    2, 0x0a /* Public */,
      26,    1,  103,    2, 0x0a /* Public */,
      28,    1,  106,    2, 0x0a /* Public */,
      29,    0,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Float, QMetaType::Int,    7,
    QMetaType::Float, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::QPointF, 0x80000000 | 12, QMetaType::QPointF,   11,   13,   14,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 17, 0x80000000 | 5, QMetaType::Float,   16,   18,   19,   20,
    QMetaType::Double, QMetaType::QPointF, QMetaType::Double, QMetaType::QPointF, QMetaType::Double,   22,   23,   24,   25,
    QMetaType::QString, QMetaType::Int,   27,
    QMetaType::QPointF, QMetaType::Int,   27,
    QMetaType::Int,

       0        // eod
};

void ZCoord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ZCoord *_t = static_cast<ZCoord *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_sendText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signal_sendBenchmark((*reinterpret_cast< QVector<float>(*)>(_a[1]))); break;
        case 2: { float _r = _t->getPointsDistance((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = _r; }  break;
        case 3: { float _r = _t->getPointsDistance((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = _r; }  break;
        case 4: _t->decartToGeogr((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3]))); break;
        case 5: _t->checkData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2])),(*reinterpret_cast< QVector<float>(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 6: { double _r = _t->distanceAB((*reinterpret_cast< const QPointF(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< const QPointF(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getTitle((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { QPointF _r = _t->getCoord((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->getSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ZCoord::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ZCoord::signal_sendText)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ZCoord::*_t)(QVector<float> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ZCoord::signal_sendBenchmark)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ZCoord::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ZCoord.data,
      qt_meta_data_ZCoord,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ZCoord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ZCoord::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ZCoord.stringdata0))
        return static_cast<void*>(const_cast< ZCoord*>(this));
    return QObject::qt_metacast(_clname);
}

int ZCoord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ZCoord::signal_sendText(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ZCoord::signal_sendBenchmark(QVector<float> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
