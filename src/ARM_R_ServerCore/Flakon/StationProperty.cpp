#include "StationProperty.h"

#include <QDebug>

StationProperty::StationProperty()
{

    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    _id = -1;
    _name = "Default";
    _ip = "127.0.0.1";
    _port = 1024;
    _latitude = 0;
    _longitude = 0;
    _geo_altitude = 0;
    _altitude = 0;
    _trace = "0";
    _type = -1;

    _distance = -1;
}

StationProperty::~StationProperty()
{
}

void StationProperty::destroy()
{
    delete this;
}

void StationProperty::set_id(int id)
{
    _id = id;
}

void StationProperty::set_name(QString name)
{
    _name = name;
}

void StationProperty::set_ip(QString ip)
{
    _ip = ip;
}

void StationProperty::set_port(quint16 port)
{
    _port = port;
}

void StationProperty::set_latitude(double lat)
{
    _latitude = lat;
}

void StationProperty::set_longitude(double lon)
{
    _longitude = lon;
}

void StationProperty::set_geo_altitude(int geo_altitude)
{
    _geo_altitude = geo_altitude;
}

void StationProperty::set_altitude(int altitude)
{
    _altitude = altitude;
}

void StationProperty::set_trace(QString trace)
{
    _trace = trace;
}

void StationProperty::set_wire_length(int length)
{
    _wire_length = length;
}

void StationProperty::set_type(int type)
{
    _type = type;
}

void StationProperty::set_distance_to_center(QMap<int, StationProperty* >* map_stations)
{
//    QVector<int> v_id;
    _distance = 0;
    int id = -1;
    int id_prev = _id;
    QString str_id;
    QStringList list = _trace.split("-");
    QStringList::Iterator it;
    for(it = list.begin(); it != list.end(); ++it)
    {
        str_id = *it;
        id = str_id.toInt();
        _distance += _getPointsDistance(map_stations->value(id_prev), map_stations->value(id));
        _distance += map_stations->value(id)->get_wire_length();
        if((id_prev == _id) && (id_prev != id))
        {
            _distance += map_stations->value(id_prev)->get_wire_length();
        }
        id_prev = id;
    }
}

double StationProperty::get_direction_distance(StationProperty *st1, StationProperty *st2)
{
    return _getPointsDistance(st1, st2);
}

int StationProperty::get_id()
{
    return _id;
}

QString StationProperty::get_name()
{
    return _name;
}

QString StationProperty::get_ip()
{
    return _ip;
}

quint16 StationProperty::get_port()
{
    return _port;
}

double StationProperty::get_latitude()
{
    return _latitude;
}

double StationProperty::get_longitude()
{
    return _longitude;
}

int StationProperty::get_geo_altitude()
{
    return _geo_altitude;
}

int StationProperty::get_altitude()
{
    return _altitude;
}

QString StationProperty::get_trace()
{
    return _trace;
}

int StationProperty::get_wire_length()
{
    return _wire_length;
}

int StationProperty::get_type()
{
    return _type;
}

double StationProperty::get_distance()
{
    return _distance;
}

void StationProperty::_convertGeogrToGeocentric(double lat, double lon, double alt, double &lX, double &lY, double &lZ)
{
    double ERadius=6366197.724;
    double pi=3.14159265359;
    double d = cos(lat*pi/180);
    double RzHeight = ERadius + alt;
    lX = RzHeight * d * cos(lon*pi/180);
    lY = RzHeight * d * sin(lon*pi/180);
    lZ = RzHeight * sin(lat*pi/180);
}

double StationProperty::_getPointsDistance(StationProperty* st1, StationProperty* st2)
{
    double aXA, aYA, aZA, aXB, aYB, aZB;
    double aDist;
    int alt1 = st1->get_geo_altitude() + st1->get_altitude();
    int alt2 = st2->get_geo_altitude() + st2->get_altitude();
    _convertGeogrToGeocentric(st1->get_latitude(), st1->get_longitude(), alt1, aXA, aYA, aZA);
    _convertGeogrToGeocentric(st2->get_latitude(), st2->get_longitude(), alt2, aXB, aYB, aZB);

    _countDistanceOfPoints(aXA, aYA, aZA, aXB, aYB, aZB, aDist);

    return aDist;
}

void StationProperty::_countDistanceOfPoints(double aX1, double aY1, double aZ1, double aX2, double aY2, double aZ2, double &lDistance)
{
    lDistance = sqrt((aX1 - aX2) * (aX1 - aX2) + (aY1 - aY2) * (aY1 - aY2) + (aZ1 - aZ2) * (aZ1 - aZ2));
}
