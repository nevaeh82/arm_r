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

	LocSystemConfiguration()
	{
		chanNum = 0;
		mode = 0;
		baseIndex = 0;
		convolution = false;
		spectrumMean = 0;
		tuningMode = 0;
		duration = 0;
		centralFreq = 0;
	}
};

inline QDataStream& operator<<(QDataStream& out, const LocSystemConfiguration& object)
{
	return out << object.chanNum
			   << object.mode
			   << object.baseIndex
			   << object.convolution
			   << object.spectrumMean
			   << object.tuningMode
			   << object.duration
			   << object.centralFreq;
}

inline QDataStream& operator>>(QDataStream& in, LocSystemConfiguration& object)
{
	return in  >> object.chanNum
			   >> object.mode
			   >> object.baseIndex
			   >> object.convolution
			   >> object.spectrumMean
			   >> object.tuningMode
			   >> object.duration
			   >> object.centralFreq;
}

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
