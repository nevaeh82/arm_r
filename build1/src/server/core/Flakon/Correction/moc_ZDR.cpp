/****************************************************************************
** Meta object code from reading C++ file 'ZDR.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/server/core/Flakon/Correction/ZDR.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ZDR.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ZDR_t {
    QByteArrayData data[11];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ZDR_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ZDR_t qt_meta_stringdata_ZDR = {
    {
QT_MOC_LITERAL(0, 0, 3), // "ZDR"
QT_MOC_LITERAL(1, 4, 20), // "signal_sendCorrectDR"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "QVector<double>"
QT_MOC_LITERAL(4, 42, 18), // "getDataFromFlackon"
QT_MOC_LITERAL(5, 61, 8), // "aNumMain"
QT_MOC_LITERAL(6, 70, 26), // "QVector<QVector<QPointF> >"
QT_MOC_LITERAL(7, 97, 7), // "aCorrel"
QT_MOC_LITERAL(8, 105, 10), // "aThreshold"
QT_MOC_LITERAL(9, 116, 16), // "QVector<double>&"
QT_MOC_LITERAL(10, 133, 3) // "lDR"

    },
    "ZDR\0signal_sendCorrectDR\0\0QVector<double>\0"
    "getDataFromFlackon\0aNumMain\0"
    "QVector<QVector<QPointF> >\0aCorrel\0"
    "aThreshold\0QVector<double>&\0lDR"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ZDR[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    4,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 3,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 6, QMetaType::Float, 0x80000000 | 9,    5,    7,    8,   10,

       0        // eod
};

void ZDR::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ZDR *_t = static_cast<ZDR *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_sendCorrectDR((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2]))); break;
        case 1: _t->getDataFromFlackon((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVector<QVector<QPointF> >(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< QVector<double>(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector<QPointF> > >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ZDR::*_t)(int , QVector<double> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ZDR::signal_sendCorrectDR)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ZDR::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ZDR.data,
      qt_meta_data_ZDR,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ZDR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ZDR::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ZDR.stringdata0))
        return static_cast<void*>(const_cast< ZDR*>(this));
    return QObject::qt_metacast(_clname);
}

int ZDR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ZDR::signal_sendCorrectDR(int _t1, QVector<double> _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
