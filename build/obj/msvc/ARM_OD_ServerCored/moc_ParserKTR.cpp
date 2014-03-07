/****************************************************************************
** Meta object code from reading C++ file 'ParserKTR.h'
**
** Created: Thu 6. Mar 15:56:14 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/TCP/ParserKTR.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParserKTR.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ParserKTR[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   11,   10,   10, 0x05,
      46,   11,   10,   10, 0x05,
      80,   11,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ParserKTR[] = {
    "ParserKTR\0\0,\0signalFFT(QVector<QPointF>,bool)\0"
    "signalFFT2(QVector<QPointF>,bool)\0"
    "signalFFT3(QVector<QPointF>,bool)\0"
};

const QMetaObject ParserKTR::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ParserKTR,
      qt_meta_data_ParserKTR, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ParserKTR::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ParserKTR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ParserKTR::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParserKTR))
        return static_cast<void*>(const_cast< ParserKTR*>(this));
    if (!strcmp(_clname, "ITCPParser"))
        return static_cast< ITCPParser*>(const_cast< ParserKTR*>(this));
    return QObject::qt_metacast(_clname);
}

int ParserKTR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalFFT((*reinterpret_cast< QVector<QPointF>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: signalFFT2((*reinterpret_cast< QVector<QPointF>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: signalFFT3((*reinterpret_cast< QVector<QPointF>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ParserKTR::signalFFT(QVector<QPointF> _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ParserKTR::signalFFT2(QVector<QPointF> _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ParserKTR::signalFFT3(QVector<QPointF> _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
