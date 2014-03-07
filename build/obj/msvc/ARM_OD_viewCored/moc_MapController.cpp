/****************************************************************************
** Meta object code from reading C++ file 'MapController.h'
**
** Created: Thu 6. Mar 17:46:28 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/MapInterface/MapController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   14,   14,   14, 0x0a,
      55,   14,   14,   14, 0x0a,
      82,   14,   14,   14, 0x0a,
     101,   95,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MapController[] = {
    "MapController\0\0signalOpenedMap()\0"
    "open_map_from_Atlas()\0open_map_from_local_file()\0"
    "onMapReady()\0state\0_slot_station_visible(bool)\0"
};

const QMetaObject MapController::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapController,
      qt_meta_data_MapController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapController))
        return static_cast<void*>(const_cast< MapController*>(this));
    if (!strcmp(_clname, "IMapController"))
        return static_cast< IMapController*>(const_cast< MapController*>(this));
    return QWidget::qt_metacast(_clname);
}

int MapController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalOpenedMap(); break;
        case 1: open_map_from_Atlas(); break;
        case 2: open_map_from_local_file(); break;
        case 3: onMapReady(); break;
        case 4: _slot_station_visible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MapController::signalOpenedMap()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
