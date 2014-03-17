#ifndef PRM300SETTINGS_H
#define PRM300SETTINGS_H

#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct Prm300Settings : public BaseSetting
{
	Prm300Settings() : BaseSetting()
	{
		id = -1;
		latitude = 0;
		longitude = 0;
		geo_altitude = 0;
		altitude = 0;
		trace = QString();
		wire_length = 0;
		type = TypePRM300;
	}

	int id;
	double latitude;
	double longitude;
	int geo_altitude;
	int altitude;
	QString trace;
	quint32 wire_length;

}Prm300Settings;

inline QDataStream& operator<<(QDataStream& out, const Prm300Settings &object)
{
	return out	<< object.name << object.host << object.port
				<< (int)object.id << (double)object.latitude << (double)object.longitude
				<< (int)object.geo_altitude << (int)object.altitude << object.trace
				<< (quint32)object.wire_length;
}

inline QDataStream& operator>>(QDataStream& in, Prm300Settings &object)
{
	quint32 type;
	QString name;
	QString host;
	quint32 port;
	int id;
	double latitude;
	double longitude;
	int geo_altitude;
	int altitude;
	QString trace;
	quint32 wire_length;

	in	>> type >> name >> host >> port >> id >> latitude >> longitude >> geo_altitude
		>> altitude >> trace >> wire_length;
	object.name = name;
	object.host = host;
	object.port = port;

	object.id = id;
	object.latitude = latitude;
	object.longitude = longitude;
	object.geo_altitude = geo_altitude;
	object.altitude = altitude;
	object.trace = trace;
	object.wire_length = wire_length;

	return in;
}

inline QByteArray Prm300SettingsToByteArray(const Prm300Settings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline Prm300Settings Prm300SettingsFromByteArray(QByteArray& inBA)
{
	Prm300Settings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // PRM300SETTINGS_H
