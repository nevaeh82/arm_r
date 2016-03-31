/****************************************************************************
** Meta object code from reading C++ file 'RpcConfigReader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/server/core/RPC/RpcConfigReader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RpcConfigReader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RpcConfigReader_t {
    QByteArrayData data[13];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RpcConfigReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RpcConfigReader_t qt_meta_stringdata_RpcConfigReader = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RpcConfigReader"
QT_MOC_LITERAL(1, 16, 29), // "readStationListInternalSignal"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 8), // "fileName"
QT_MOC_LITERAL(4, 56, 37), // "readAtlantConfigurationIntern..."
QT_MOC_LITERAL(5, 94, 8), // "filename"
QT_MOC_LITERAL(6, 103, 33), // "readDbConfigurationInternalSi..."
QT_MOC_LITERAL(7, 137, 14), // "getStationList"
QT_MOC_LITERAL(8, 152, 27), // "readStationListInternalSlot"
QT_MOC_LITERAL(9, 180, 35), // "readAtlantConfigurationIntern..."
QT_MOC_LITERAL(10, 216, 31), // "readDbConfigurationInternalSlot"
QT_MOC_LITERAL(11, 248, 14), // "inStationsList"
QT_MOC_LITERAL(12, 263, 10) // "dataToSend"

    },
    "RpcConfigReader\0readStationListInternalSignal\0"
    "\0fileName\0readAtlantConfigurationInternalSignal\0"
    "filename\0readDbConfigurationInternalSignal\0"
    "getStationList\0readStationListInternalSlot\0"
    "readAtlantConfigurationInternalSlot\0"
    "readDbConfigurationInternalSlot\0"
    "inStationsList\0dataToSend"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RpcConfigReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,
       7,    0,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   64,    2, 0x08 /* Private */,
       9,    1,   67,    2, 0x08 /* Private */,
      10,    1,   70,    2, 0x08 /* Private */,
      11,    1,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QVariant,   12,

       0        // eod
};

void RpcConfigReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RpcConfigReader *_t = static_cast<RpcConfigReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readStationListInternalSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->readAtlantConfigurationInternalSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->readDbConfigurationInternalSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->getStationList(); break;
        case 4: _t->readStationListInternalSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->readAtlantConfigurationInternalSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->readDbConfigurationInternalSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->inStationsList((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RpcConfigReader::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigReader::readStationListInternalSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RpcConfigReader::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigReader::readAtlantConfigurationInternalSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RpcConfigReader::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigReader::readDbConfigurationInternalSignal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RpcConfigReader::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RpcConfigReader::getStationList)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject RpcConfigReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RpcConfigReader.data,
      qt_meta_data_RpcConfigReader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RpcConfigReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RpcConfigReader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RpcConfigReader.stringdata0))
        return static_cast<void*>(const_cast< RpcConfigReader*>(this));
    if (!strcmp(_clname, "IRpcConfigReader"))
        return static_cast< IRpcConfigReader*>(const_cast< RpcConfigReader*>(this));
    if (!strcmp(_clname, "IRpcListener"))
        return static_cast< IRpcListener*>(const_cast< RpcConfigReader*>(this));
    return QObject::qt_metacast(_clname);
}

int RpcConfigReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void RpcConfigReader::readStationListInternalSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RpcConfigReader::readAtlantConfigurationInternalSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RpcConfigReader::readDbConfigurationInternalSignal(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RpcConfigReader::getStationList()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
