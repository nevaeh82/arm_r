#ifndef STATIONCONFIGURATION_H
#define STATIONCONFIGURATION_H

#include <QtGlobal>

struct Prm
{
	int att1;
	int att2;
	int filter;

	Prm()
	{
		att1 = 0; // invalid id
		att2 = 0;
		filter = 0;
	}
};

inline QDataStream& operator<<(QDataStream& out, const Prm& object)
{
	return out << object.att1
			   << object.att2
			   << object.filter;
}

inline QDataStream& operator>>(QDataStream& out, Prm& object)
{
	return out >> object.att1
			   >> object.att2
			   >> object.filter;
}

struct DevState
{
	int id;
	bool state;

	DevState()
	{
		id = -2; // invalid id
		state = false;
	}
};

inline QDataStream& operator<<(QDataStream& out, const DevState& object)
{
	return out << object.id
			   << object.state;
}

inline QDataStream& operator>>(QDataStream& out, DevState& object)
{
	return out >> object.id
			   >> object.state;
}

struct LocSystemConfiguration
{
	int chanNum;
	int mode;
	int baseIndex;
	bool convolution;
	int spectrumMean;
	int tuningMode;
	int duration;
	int centralFreq;
	bool doppler;
	int freqBand;

	//Filter for correl
	int range;
	int shift;

	LocSystemConfiguration()
	{
		duration = 10;
		chanNum = 0;
		mode = 0;
		baseIndex = 0;
		convolution = false;
		spectrumMean = 0;
		tuningMode = 2;
		centralFreq = 0;
		doppler = false;
		freqBand = 1;

		//Filter for correl
		range = 0;
		shift = 0;
	}
};

inline QDataStream& operator<<(QDataStream& out, const LocSystemConfiguration& object)
{
	return out	<< object.duration
				<< object.chanNum
				<< object.mode
				<< object.baseIndex
				<< object.convolution
				<< object.spectrumMean
				<< object.tuningMode
				<< object.centralFreq
				<< object.doppler
				<< object.freqBand
				<< object.range
				<< object.shift;
}

inline QDataStream& operator>>(QDataStream& in, LocSystemConfiguration& object)
{
	return in	>> object.duration
				>> object.chanNum
				>> object.mode
				>> object.baseIndex
				>> object.convolution
				>> object.spectrumMean
				>> object.tuningMode
				>> object.centralFreq
				>> object.doppler
				>> object.freqBand
				>> object.range
				>> object.shift;
}

struct StationConfiguration
{
	StationConfiguration()
	{
		id			= 0;
		name		= "Unknown";
		latitude	= 0.0;
		longitude	= 0.0;
		altitude	= 0.0;
		hostPrm300	= "127.0.0.1";
		hostADC		= "127.0.0.1";
		namePrm = "";
		nameChannel = "";
		statusPrm = "";
		freqPrm = 0;
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
		namePrm		= object.namePrm;
		nameChannel	= object.nameChannel;
		statusPrm	= object.statusPrm;
		freqPrm		= object.freqPrm;
		statusAdc	= object.statusAdc;
	}

	quint32		id;
	QString		name;
	QString		namePrm;
	QString		nameChannel;
	double		latitude;
	double		longitude;
	double		altitude;
	QString		hostPrm300;
	bool		statusPrm;
	int			freqPrm;

	bool		statusAdc;
	QString		hostADC;
};

inline QDataStream& operator<<(QDataStream& out, const StationConfiguration& object)
{
	return out << object.id
			   << object.name
			   << object.latitude
			   << object.longitude
			   << object.altitude
			   << object.hostPrm300
			   << object.hostADC
			   << object.namePrm
			   << object.nameChannel
			   << object.statusPrm
			   << object.freqPrm
			   << object.statusAdc;
}

inline QDataStream& operator>>(QDataStream& in, StationConfiguration& object)
{
	in >> object.id
	   >> object.name
	   >> object.latitude
	   >> object.longitude
	   >> object.altitude
	   >> object.hostPrm300
	   >> object.hostADC
	   >> object.namePrm
	   >> object.nameChannel
	   >> object.statusPrm
	   >> object.freqPrm
	   >> object.statusAdc;

	return in;
}

#endif // STATIONCONFIGURATION_H
