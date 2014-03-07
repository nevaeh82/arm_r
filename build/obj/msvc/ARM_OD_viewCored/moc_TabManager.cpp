/****************************************************************************
** Meta object code from reading C++ file 'TabManager.h'
**
** Created: Thu 6. Mar 17:46:29 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/Tabs/TabManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TabManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   55,   11,   11, 0x08,
      89,   83,   11,   11, 0x08,
     121,  113,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TabManager[] = {
    "TabManager\0\0,\0signalSendToNIIPPControl(int,QByteArray)\0"
    "index\0_slot_change_tab(int)\0state\0"
    "_slot_show_niipp1(bool)\0id,data\0"
    "_slot_send_data_to_niipp_control(int,QByteArray)\0"
};

const QMetaObject TabManager::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_TabManager,
      qt_meta_data_TabManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TabManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TabManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TabManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabManager))
        return static_cast<void*>(const_cast< TabManager*>(this));
    if (!strcmp(_clname, "IModuleController"))
        return static_cast< IModuleController*>(const_cast< TabManager*>(this));
    if (!strcmp(_clname, "ITabManager"))
        return static_cast< ITabManager*>(const_cast< TabManager*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int TabManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalSendToNIIPPControl((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 1: _slot_change_tab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _slot_show_niipp1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _slot_send_data_to_niipp_control((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TabManager::signalSendToNIIPPControl(int _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
