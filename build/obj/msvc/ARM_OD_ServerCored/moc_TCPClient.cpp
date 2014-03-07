/****************************************************************************
** Meta object code from reading C++ file 'TCPClient.h'
**
** Created: Thu 6. Mar 17:47:30 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_ServerCore/TCP/TCPClient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TCPClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TCPClient[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      33,   10,   10,   10, 0x05,
      50,   10,   10,   10, 0x05,
      65,   61,   10,   10, 0x05,
      80,   10,   10,   10, 0x05,
      98,   92,   10,   10, 0x05,
     120,   10,   10,   10, 0x05,
     146,  141,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     169,   10,   10,   10, 0x0a,
     180,   10,   10,   10, 0x0a,
     187,   10,   10,   10, 0x0a,
     201,  141,   10,   10, 0x08,
     224,   10,   10,   10, 0x08,
     244,   10,   10,   10, 0x08,
     267,   10,   10,   10, 0x08,
     299,  287,   10,   10, 0x08,
     350,   10,   10,   10, 0x08,
     370,   10,   10,   10, 0x08,
     385,   92,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TCPClient[] = {
    "TCPClient\0\0recv_msg(const char*)\0"
    "err_connection()\0finished()\0err\0"
    "error(QString)\0connected()\0state\0"
    "signalConnected(bool)\0signalReconnection()\0"
    "data\0signalSend(QByteArray)\0proccess()\0"
    "stop()\0slotDestroy()\0_slotWrite(QByteArray)\0"
    "onSocketConnected()\0onSocketDisconnected()\0"
    "onSocketReadyRead()\0socketError\0"
    "onSocketDisplayError(QAbstractSocket::SocketError)\0"
    "finish_connection()\0reconnection()\0"
    "slotState(QAbstractSocket::SocketState)\0"
};

const QMetaObject TCPClient::staticMetaObject = {
    { &ITCPClient::staticMetaObject, qt_meta_stringdata_TCPClient,
      qt_meta_data_TCPClient, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TCPClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TCPClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TCPClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TCPClient))
        return static_cast<void*>(const_cast< TCPClient*>(this));
    if (!strcmp(_clname, "IClient"))
        return static_cast< IClient*>(const_cast< TCPClient*>(this));
    return ITCPClient::qt_metacast(_clname);
}

int TCPClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ITCPClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: recv_msg((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 1: err_connection(); break;
        case 2: finished(); break;
        case 3: error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: connected(); break;
        case 5: signalConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: signalReconnection(); break;
        case 7: signalSend((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: proccess(); break;
        case 9: stop(); break;
        case 10: slotDestroy(); break;
        case 11: _slotWrite((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 12: onSocketConnected(); break;
        case 13: onSocketDisconnected(); break;
        case 14: onSocketReadyRead(); break;
        case 15: onSocketDisplayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 16: finish_connection(); break;
        case 17: reconnection(); break;
        case 18: slotState((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void TCPClient::recv_msg(const char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TCPClient::err_connection()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TCPClient::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TCPClient::error(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TCPClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void TCPClient::signalConnected(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TCPClient::signalReconnection()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void TCPClient::signalSend(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
