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
		namePrm = "";
		nameChannel = "";
		portPrm = 0;
		typePrm = 0;
		statusPrm = "";
		freqPrm = 0;
		inversionPrm = "";
		statusAdc = "";
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
		namePrm		= object.namePrm;
		nameChannel	= object.nameChannel;
		portPrm		= object.portPrm;
		typePrm		= object.typePrm;
		statusPrm	= object.statusPrm;
		freqPrm = object.freqPrm;
		inversionPrm= object.inversionPrm;
		statusAdc	= object.statusAdc;
	}

	quint32		id;
	QString		name;
	QString		namePrm;
	QString		nameChannel;
	double		latitude;
	double		longitude;
	QString		hostPrm300;
	quint32		portPrm;
	quint32		typePrm;
	bool		statusPrm;
	bool		inversionPrm;
	int freqPrm;

	bool		statusAdc;

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
			   << object.portADC
			   << object.namePrm
			   << object.nameChannel
			   << object.portPrm
			   << object.typePrm
			   << object.statusPrm
			   << object.freqPrm
			   << object.inversionPrm
			   << object.statusAdc;
}

inline QDataStream& operator>>(QDataStream& in, StationConfiguration& object)
{
	in >> object.id
	   >> object.name
	   >> object.latitude
	   >> object.longitude
	   >> object.hostPrm300
	   >> object.hostADC
	   >> object.portADC
	   >> object.namePrm
	   >> object.nameChannel
	   >> object.portPrm
	   >> object.typePrm
	   >> object.statusPrm
	   >> object.freqPrm
	   >> object.inversionPrm
	   >> object.statusAdc;

	return in;
}

#endif // STATIONCONFIGURATION_H
