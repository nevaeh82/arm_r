#ifndef STATIONPROPERTY_H
#define STATIONPROPERTY_H

#include <QString>
#include <QMap>
#include <QStringList>
#include <QVector>
#include <QTextCodec>
#include <QTextStream>

class StationProperty
{
public:
    StationProperty();
    ~StationProperty();

private:
    /// punkt Id
    int                 _id;
    /// name
    QString             _name;
    /// ip
    QString             _ip;
    /// port
    quint16             _port;
    /// latitude
    double              _latitude;
    /// longitude
    double              _longitude;
    /// geo_altitude
    int                 _geo_altitude;
    /// altitude
    int                 _altitude;
    /// signal trace
    QString             _trace;
    /// wire length
    int                 _wire_length;
    /// type
    int                 _type;

    /// distance to center
    double              _distance;

public:
    void                destroy();
    int                 get_id();
    QString             get_name();
    QString             get_ip();
    quint16             get_port();
    double              get_latitude();
    double              get_longitude();
    int                 get_geo_altitude();
    int                 get_altitude();
    QString             get_trace();
    int                 get_wire_length();
    int                 get_type();

    double              get_distance();

    void set_id(int id);
    void set_name(QString name);
    void set_ip(QString ip);
    void set_port(quint16 port);
    void set_latitude(double lat);
    void set_longitude(double lon);
    void set_geo_altitude(int geo_altitude);
    void set_altitude(int altitude);
    void set_trace(QString trace);
    void set_wire_length(int length);
    void set_type(int type);



    void set_distance_to_center(QMap<int, StationProperty* >* map_stations);
    double get_direction_distance(StationProperty* st1, StationProperty* st2);

private:
    void _convertGeogrToGeocentric(double lat, double lon, double alt, double &lX, double &lY, double &lZ);
    double _getPointsDistance(StationProperty* st1, StationProperty* st2);
    void _countDistanceOfPoints(double aX1, double aY1, double aZ1, double aX2, double aY2, double aZ2, double &lDistance);





};

#endif // STATIONPROPERTY_H
