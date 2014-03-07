/****************************************************************************
** Meta object code from reading C++ file 'mapclient1.h'
**
** Created: Thu 6. Mar 17:46:28 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ARM_OD_viewCore/MapInterface/mapclient1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapclient1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapClient1[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,
      48,   40,   11,   11, 0x05,
      85,   40,   11,   11, 0x05,
     123,   11,   11,   11, 0x05,
     145,   11,   11,   11, 0x05,
     174,  170,   11,   11, 0x05,
     226,  223,   11,   11, 0x05,
     271,  267,   11,   11, 0x05,
     349,  320,   11,   11, 0x05,
     404,  393,   11,   11, 0x05,
     457,  442,   11,   11, 0x05,
     484,  442,   11,   11, 0x05,
     578,  514,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     643,   11,   11,   11, 0x0a,
     655,   11,   11,   11, 0x0a,
     668,   11,   11,   11, 0x0a,
     682,   40,   11,   11, 0x0a,
     711,   40,   11,   11, 0x0a,
     741,  170,   11,   11, 0x0a,
     796,  783,   11,   11, 0x0a,
     859,  842,   11,   11, 0x0a,
     937,  908,   11,   11, 0x0a,
    1011,  979,   11,   11, 0x08,
    1069,   40,   11,   11, 0x08,
    1115, 1106,   11,   11, 0x08,
    1156,  170,   11,   11, 0x08,
    1204,   40,   11,   11, 0x08,
    1240,   11,   11,   11, 0x08,
    1262,   11,   11,   11, 0x08,
    1280,  320,   11,   11, 0x08,
    1330,  842,   11,   11, 0x08,
    1380,  783,   11,   11, 0x08,
    1422,  442,   11,   11, 0x08,
    1450,  442,   11,   11, 0x08,
    1481,  514,   11,   11, 0x08,
    1547,   11,   11,   11, 0x08,
    1562,   11,   11,   11, 0x08,
    1576,   11,   11,   11, 0x08,
    1591, 1589,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MapClient1[] = {
    "MapClient1\0\0bpla\0signalNewBPLA(QString)\0"
    "id,data\0signal_new_coord_bla(int,QByteArray)\0"
    "signal_new_coord_evil(int,QByteArray)\0"
    "signalUpdateTumeOut()\0signalTimeoutSimulator()\0"
    "vec\0signal_new_ais_data(QMap<int,QVector<QString> >)\0"
    ",,\0signalUpdateCicle(int,double,QByteArray)\0"
    ",,,\0signalUpdateSector(int,double,double,QByteArray)\0"
    "id,post_id,lat,lon,direction\0"
    "signal_peleng(int,int,double,double,double)\0"
    "id,lat,lon\0signal_peleng_evil(int,double,double)\0"
    "bla_id,bpla_id\0signalAddPerehvat(int,int)\0"
    "signalRemovePerehvat(int,int)\0"
    "bla_id,bpla_id,aCoord,aHgt,aRadius,aTime,aIntcCourse,aIntcSpeed\0"
    "signalPerehvatPoint(int,int,QPointF,float,float,int,float,float)\0"
    "set_Point()\0center_map()\0justify_map()\0"
    "slot_add_BLA(int,QByteArray)\0"
    "slot_add_evil(int,QByteArray)\0"
    "slot_add_ais(QMap<int,QVector<QString> >)\0"
    "id,radius,ba\0slot_niipp_power_cicle(int,double,QByteArray)\0"
    "id,radius,bis,ba\0"
    "slot_update_sector(int,double,double,QByteArray)\0"
    "id,id_post,lat,lon,direction\0"
    "slot_peleng(int,int,double,double,double)\0"
    "id,point,alt,speed,course,state\0"
    "_slot_set_point_bla(int,QPointF,double,double,double,int)\0"
    "_slot_set_point_evil(int,QByteArray)\0"
    "id,point\0_slot_set_point_evil_peleng(int,QPointF)\0"
    "_slot_set_ais_data(QMap<int,QVector<QString> >)\0"
    "_slot_set_cur_point(int,QByteArray)\0"
    "_slot_update_points()\0_slot_simulator()\0"
    "_slot_update_peleng(int,int,double,double,double)\0"
    "_slot_update_sector(int,double,double,QByteArray)\0"
    "_slot_update_cicle(int,double,QByteArray)\0"
    "_slot_add_perehvat(int,int)\0"
    "_slot_remove_perehvat(int,int)\0"
    "_slot_perehvat_point(int,int,QPointF,float,float,int,float,float)\0"
    "updateCircle()\0updateSlice()\0onMapReady()\0"
    ",\0_slot_mouse_clicked(double,double)\0"
};

const QMetaObject MapClient1::staticMetaObject = {
    { &IMapClient::staticMetaObject, qt_meta_stringdata_MapClient1,
      qt_meta_data_MapClient1, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapClient1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapClient1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapClient1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapClient1))
        return static_cast<void*>(const_cast< MapClient1*>(this));
    return IMapClient::qt_metacast(_clname);
}

int MapClient1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IMapClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalNewBPLA((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: signal_new_coord_bla((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 2: signal_new_coord_evil((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 3: signalUpdateTumeOut(); break;
        case 4: signalTimeoutSimulator(); break;
        case 5: signal_new_ais_data((*reinterpret_cast< QMap<int,QVector<QString> >(*)>(_a[1]))); break;
        case 6: signalUpdateCicle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 7: signalUpdateSector((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4]))); break;
        case 8: signal_peleng((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 9: signal_peleng_evil((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 10: signalAddPerehvat((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: signalRemovePerehvat((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: signalPerehvatPoint((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< float(*)>(_a[7])),(*reinterpret_cast< float(*)>(_a[8]))); break;
        case 13: set_Point(); break;
        case 14: center_map(); break;
        case 15: justify_map(); break;
        case 16: slot_add_BLA((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 17: slot_add_evil((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 18: slot_add_ais((*reinterpret_cast< QMap<int,QVector<QString> >(*)>(_a[1]))); break;
        case 19: slot_niipp_power_cicle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 20: slot_update_sector((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4]))); break;
        case 21: slot_peleng((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 22: _slot_set_point_bla((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 23: _slot_set_point_evil((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 24: _slot_set_point_evil_peleng((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 25: _slot_set_ais_data((*reinterpret_cast< QMap<int,QVector<QString> >(*)>(_a[1]))); break;
        case 26: _slot_set_cur_point((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 27: _slot_update_points(); break;
        case 28: _slot_simulator(); break;
        case 29: _slot_update_peleng((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 30: _slot_update_sector((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4]))); break;
        case 31: _slot_update_cicle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 32: _slot_add_perehvat((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 33: _slot_remove_perehvat((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 34: _slot_perehvat_point((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< float(*)>(_a[7])),(*reinterpret_cast< float(*)>(_a[8]))); break;
        case 35: updateCircle(); break;
        case 36: updateSlice(); break;
        case 37: onMapReady(); break;
        case 38: _slot_mouse_clicked((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void MapClient1::signalNewBPLA(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MapClient1::signal_new_coord_bla(int _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MapClient1::signal_new_coord_evil(int _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MapClient1::signalUpdateTumeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void MapClient1::signalTimeoutSimulator()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MapClient1::signal_new_ais_data(QMap<int,QVector<QString> > _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MapClient1::signalUpdateCicle(int _t1, double _t2, QByteArray _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MapClient1::signalUpdateSector(int _t1, double _t2, double _t3, QByteArray _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MapClient1::signal_peleng(int _t1, int _t2, double _t3, double _t4, double _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MapClient1::signal_peleng_evil(int _t1, double _t2, double _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MapClient1::signalAddPerehvat(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void MapClient1::signalRemovePerehvat(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void MapClient1::signalPerehvatPoint(int _t1, int _t2, QPointF _t3, float _t4, float _t5, int _t6, float _t7, float _t8)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_END_MOC_NAMESPACE
