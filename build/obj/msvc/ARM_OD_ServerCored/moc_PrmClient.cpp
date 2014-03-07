/****************************************************************************
** Meta object code from reading C++ file 'PrmClient.h'
**
** Created: Thu 6. Mar 15:53:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/TCP/PrmClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PrmClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PrmClient[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x05,
      57,   35,   10,   10, 0x05,
     106,   99,   10,   10, 0x05,
     128,   10,   10,   10, 0x05,
     142,   10,   10,   10, 0x05,
     163,   10,   10,   10, 0x05,
     184,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     200,   10,   10,   10, 0x0a,
     216,  210,   10,   10, 0x0a,
     240,  233,   10,   10, 0x0a,
     263,  233,   10,   10, 0x0a,
     286,  233,   10,   10, 0x0a,
     303,   10,   10,   10, 0x08,
     319,   10,   10,   10, 0x08,
     359,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PrmClient[] = {
    "PrmClient\0\0aText\0sendText(QString)\0"
    "aFreq,aAt1,aAt2,aFltr\0"
    "sendPrmFreq(quint16,quint8,quint8,quint8)\0"
    "aState\0deviceConnected(bool)\0freqChanged()\0"
    "attenuator1Changed()\0attenuator2Changed()\0"
    "filterChanged()\0getFreq()\0aFreq\0"
    "setFreq(quint16)\0aValue\0setAttenuator1(quint8)\0"
    "setAttenuator2(quint8)\0setFiltr(quint8)\0"
    "slotReadyRead()\0slotError(QAbstractSocket::SocketError)\0"
    "slotConnected()\0"
};

const QMetaObject PrmClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PrmClient,
      qt_meta_data_PrmClient, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PrmClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PrmClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PrmClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PrmClient))
        return static_cast<void*>(const_cast< PrmClient*>(this));
    return QObject::qt_metacast(_clname);
}

int PrmClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: sendPrmFreq((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])),(*reinterpret_cast< quint8(*)>(_a[4]))); break;
        case 2: deviceConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: freqChanged(); break;
        case 4: attenuator1Changed(); break;
        case 5: attenuator2Changed(); break;
        case 6: filterChanged(); break;
        case 7: getFreq(); break;
        case 8: setFreq((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 9: setAttenuator1((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 10: setAttenuator2((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 11: setFiltr((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 12: slotReadyRead(); break;
        case 13: slotError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 14: slotConnected(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void PrmClient::sendText(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PrmClient::sendPrmFreq(quint16 _t1, quint8 _t2, quint8 _t3, quint8 _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PrmClient::deviceConnected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PrmClient::freqChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void PrmClient::attenuator1Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void PrmClient::attenuator2Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void PrmClient::filterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
