#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

#include <QObject>
#include <MapManager.h>
#include <pwgiswidget.h>
#include <IMapBounds.h>
#include <GeoRectangle.h>
#include <PwGis/objects/PwGisStyle.h>
#include <PwGisPointList.h>
#include <QPointF>
#include <QByteArray>

#include "../NIIPP/INiippController.h"

struct DataFly {
    QString height;
    QString latitute;
    QString longinude;
    QString speed;
    QString namePlane;
    QString course;
};

class IMapClient: public QObject
{
public:
    ~IMapClient() {};

public slots:
    virtual void set_Point()    = 0;
    virtual void center_map()   = 0;
    virtual void justify_map()  = 0;
    virtual void slot_add_BLA(int id, QByteArray data)    = 0;
    virtual void slot_add_ais(QMap<int, QVector<QString> > vec)                                                         = 0;

//    int state;
//    QPointF point;
//    double speed;
//    double alt;
//    double course;
    virtual void slot_add_evil(int id, QByteArray data)   = 0;
    virtual void slot_niipp_power_cicle(int id, double radius, QByteArray ba)         = 0;
//    virtual void slot_create_niipp_sector(int id, QString name, double lon, double lat, double angel)   = 0;
    virtual void slot_update_sector(int id, double radius, double bis, QByteArray ba)                   = 0;
    virtual void set_niipp_controller(INiiPPController *niipp_controller)                               = 0;

    virtual void slot_peleng(int id, int id_post, double lat, double lon, double direction)             = 0;
//    virtual void slot_peleng_evil(int id, double lat, double lon)                                       = 0;
    virtual void show_layer(int index, bool state)                                                      = 0;

    virtual void add_perehvat(int bla_id, int bpla_id)      = 0;
    virtual void remove_perehvat(int bla_id, int bpla_id)   = 0;

    virtual void perehvat_point(int bla_id, int bpla_id, QPointF aCoord, float aHgt, float aRadius, int aTime, float aIntcCourse, float aIntcSpeed) = 0;

    virtual void slot_remove_point_uvoda() = 0;
};

#endif // IMAPCLIENT_H
