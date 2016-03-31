/****************************************************************************
** Meta object code from reading C++ file 'ServiceHandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/CISCommonLib/src/ServiceHandler/ServiceHandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ServiceHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SkyHobbit__Common__ServiceControl__ServiceHandler_t {
    QByteArrayData data[22];
    char stringdata0[363];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SkyHobbit__Common__ServiceControl__ServiceHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SkyHobbit__Common__ServiceControl__ServiceHandler_t qt_meta_stringdata_SkyHobbit__Common__ServiceControl__ServiceHandler = {
    {
QT_MOC_LITERAL(0, 0, 49), // "SkyHobbit::Common::ServiceCon..."
QT_MOC_LITERAL(1, 50, 20), // "processStartedSignal"
QT_MOC_LITERAL(2, 71, 0), // ""
QT_MOC_LITERAL(3, 72, 24), // "processStartFailedSignal"
QT_MOC_LITERAL(4, 97, 9), // "terminate"
QT_MOC_LITERAL(5, 107, 13), // "serviceHandle"
QT_MOC_LITERAL(6, 121, 16), // "isServiceStarted"
QT_MOC_LITERAL(7, 138, 11), // "serviceName"
QT_MOC_LITERAL(8, 150, 11), // "waitService"
QT_MOC_LITERAL(9, 162, 7), // "timeout"
QT_MOC_LITERAL(10, 170, 15), // "waitForFinished"
QT_MOC_LITERAL(11, 186, 14), // "waitForStarted"
QT_MOC_LITERAL(12, 201, 5), // "msecs"
QT_MOC_LITERAL(13, 207, 11), // "addArgument"
QT_MOC_LITERAL(14, 219, 3), // "arg"
QT_MOC_LITERAL(15, 223, 18), // "onProcessDestroyed"
QT_MOC_LITERAL(16, 242, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(17, 263, 19), // "onReadProcessOutput"
QT_MOC_LITERAL(18, 283, 18), // "onReadProcessError"
QT_MOC_LITERAL(19, 302, 16), // "onProcessStarted"
QT_MOC_LITERAL(20, 319, 20), // "onProcessStartFailed"
QT_MOC_LITERAL(21, 340, 22) // "QProcess::ProcessError"

    },
    "SkyHobbit::Common::ServiceControl::ServiceHandler\0"
    "processStartedSignal\0\0processStartFailedSignal\0"
    "terminate\0serviceHandle\0isServiceStarted\0"
    "serviceName\0waitService\0timeout\0"
    "waitForFinished\0waitForStarted\0msecs\0"
    "addArgument\0arg\0onProcessDestroyed\0"
    "QProcess::ExitStatus\0onReadProcessOutput\0"
    "onReadProcessError\0onProcessStarted\0"
    "onProcessStartFailed\0QProcess::ProcessError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SkyHobbit__Common__ServiceControl__ServiceHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   96,    2, 0x0a /* Public */,
       5,    0,   97,    2, 0x0a /* Public */,
       6,    0,   98,    2, 0x0a /* Public */,
       7,    0,   99,    2, 0x0a /* Public */,
       8,    1,  100,    2, 0x0a /* Public */,
      10,    0,  103,    2, 0x0a /* Public */,
      11,    1,  104,    2, 0x0a /* Public */,
      11,    0,  107,    2, 0x2a /* Public | MethodCloned */,
      13,    1,  108,    2, 0x0a /* Public */,
      15,    2,  111,    2, 0x09 /* Protected */,
      17,    0,  116,    2, 0x09 /* Protected */,
      18,    0,  117,    2, 0x09 /* Protected */,
      19,    0,  118,    2, 0x09 /* Protected */,
      20,    1,  119,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::QString,
    QMetaType::Bool, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 16,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,    2,

       0        // eod
};

void SkyHobbit::Common::ServiceControl::ServiceHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ServiceHandler *_t = static_cast<ServiceHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processStartedSignal(); break;
        case 1: _t->processStartFailedSignal(); break;
        case 2: { int _r = _t->terminate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->serviceHandle();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->isServiceStarted();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->serviceName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->waitService((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->waitForFinished(); break;
        case 8: _t->waitForStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->waitForStarted(); break;
        case 10: _t->addArgument((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->onProcessDestroyed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 12: _t->onReadProcessOutput(); break;
        case 13: _t->onReadProcessError(); break;
        case 14: _t->onProcessStarted(); break;
        case 15: _t->onProcessStartFailed((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ServiceHandler::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ServiceHandler::processStartedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ServiceHandler::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ServiceHandler::processStartFailedSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SkyHobbit::Common::ServiceControl::ServiceHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SkyHobbit__Common__ServiceControl__ServiceHandler.data,
      qt_meta_data_SkyHobbit__Common__ServiceControl__ServiceHandler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SkyHobbit::Common::ServiceControl::ServiceHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SkyHobbit::Common::ServiceControl::ServiceHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SkyHobbit__Common__ServiceControl__ServiceHandler.stringdata0))
        return static_cast<void*>(const_cast< ServiceHandler*>(this));
    if (!strcmp(_clname, "IServiceHandler"))
        return static_cast< IServiceHandler*>(const_cast< ServiceHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int SkyHobbit::Common::ServiceControl::ServiceHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void SkyHobbit::Common::ServiceControl::ServiceHandler::processStartedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void SkyHobbit::Common::ServiceControl::ServiceHandler::processStartFailedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
