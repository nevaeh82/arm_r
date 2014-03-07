/****************************************************************************
** Meta object code from reading C++ file 'ServiceHandler.h'
**
** Created: Thu 6. Mar 17:46:23 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_Common/ServiceHandler/ServiceHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ServiceHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Pw__Common__ServiceControl__ServiceHandler[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   43,   43,   43, 0x05,
      67,   43,   43,   43, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   43,   94,   43, 0x0a,
     110,   43,   94,   43, 0x0a,
     131,   43,  126,   43, 0x0a,
     158,   43,  150,   43, 0x0a,
     180,  172,  126,   43, 0x0a,
     197,   43,   43,   43, 0x0a,
     219,  215,   43,   43, 0x0a,
     242,  240,   43,   43, 0x09,
     287,   43,   43,   43, 0x09,
     309,   43,   43,   43, 0x09,
     330,   43,   43,   43, 0x09,
     349,   43,   43,   43, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Pw__Common__ServiceControl__ServiceHandler[] = {
    "Pw::Common::ServiceControl::ServiceHandler\0"
    "\0processStartedSignal()\0"
    "processStartFailedSignal()\0int\0"
    "terminate()\0serviceHandle()\0bool\0"
    "isServiceStarted()\0QString\0serviceName()\0"
    "timeout\0waitService(int)\0waitForFinished()\0"
    "arg\0addArgument(QString)\0,\0"
    "onProcessDestroyed(int,QProcess::ExitStatus)\0"
    "onReadProcessOutput()\0onReadProcessError()\0"
    "onProcessStarted()\0"
    "onProcessStartFailed(QProcess::ProcessError)\0"
};

const QMetaObject Pw::Common::ServiceControl::ServiceHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Pw__Common__ServiceControl__ServiceHandler,
      qt_meta_data_Pw__Common__ServiceControl__ServiceHandler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Pw::Common::ServiceControl::ServiceHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Pw::Common::ServiceControl::ServiceHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Pw::Common::ServiceControl::ServiceHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Pw__Common__ServiceControl__ServiceHandler))
        return static_cast<void*>(const_cast< ServiceHandler*>(this));
    if (!strcmp(_clname, "IServiceHandler"))
        return static_cast< IServiceHandler*>(const_cast< ServiceHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int Pw::Common::ServiceControl::ServiceHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: processStartedSignal(); break;
        case 1: processStartFailedSignal(); break;
        case 2: { int _r = terminate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = serviceHandle();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { bool _r = isServiceStarted();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { QString _r = serviceName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { bool _r = waitService((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: waitForFinished(); break;
        case 8: addArgument((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: onProcessDestroyed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 10: onReadProcessOutput(); break;
        case 11: onReadProcessError(); break;
        case 12: onProcessStarted(); break;
        case 13: onProcessStartFailed((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Pw::Common::ServiceControl::ServiceHandler::processStartedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Pw::Common::ServiceControl::ServiceHandler::processStartFailedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
