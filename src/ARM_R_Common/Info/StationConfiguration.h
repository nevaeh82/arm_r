#ifndef STATIONCONFIGURATION_H
#define STATIONCONFIGURATION_H

#include <QtGlobal>

struct StationConfiguration
{
	StationConfiguration()
	{
		id			= 0;
		name		= "Unknown";
		latitude	= 0.0;
		longitude	= 0.0;
		hostPrm300	= "127.0.0.1";
		hostADC		= "127.0.0.1";
		portADC		= 0;
	}

	StationConfiguration(const StationConfiguration& object)
	{
		id			= object.id;
		name		= object.name;
		latitude	= object.latitude;
		longitude	= object.longitude;
		hostPrm300	= object.hostPrm300;
		hostADC		= object.hostADC;
		portADC		= object.portADC;
	}

	quint32		id;
	QString		name;
	double		latitude;
	double		longitude;
	QString		hostPrm300;
	QString		hostADC;
	quint32		portADC;
};

inline QDataStream& operator<<(QDataStream& out, const StationConfiguration& object)
{
	return out << object.id
			   << object.name
			   << object.latitude
			   << object.longitude
			   << object.hostPrm300
			   << object.hostADC
			   << object.portADC;
}

inline QDataStream& operator>>(QDataStream& in, StationConfiguration& object)
{
	in >> object.id
	   >> object.name
	   >> object.latitude
	   >> object.longitude
	   >> object.hostPrm300
	   >> object.hostADC
	   >> object.portADC;

	return in;
}

#endif // STATIONCONFIGURATION_H
