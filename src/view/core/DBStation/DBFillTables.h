#ifndef DBFILLTABLES_H
#define DBFILLTABLES_H

#include <QString>
#include <QDateTime>

/// fill stationData table
typedef struct StationData
{
	QString stationName;
	unsigned short port;
	QString category;
	double frequency;
	double bandwidth;
	QString signalType;
    unsigned short checked;
} StationData;

/// full log_info
typedef struct StationDataFull
{
	StationData stationShortInfo;
	QString		stationIP;
	QDateTime	date;
}StationDataFull;


/// info about frequency and bandwidth
typedef struct StationsFrequencyAndBandwith
{
	QString stationName;
	double frequency;
	double bandwidth;
}StationsFrequencyAndBandwith;

/// bandwidth of overthreshold in station
typedef struct OverthresholdBand
{
    double startFreq;
    double finishFreq;
    bool operator==(OverthresholdBand &val)
    {
        if(finishFreq == val.finishFreq && startFreq == val.finishFreq)
            return true;
        return false;
    }
}OverthresholdBand;



///info for report
typedef struct AllStationsReport
{
    StationsFrequencyAndBandwith sfab;
    QString category;
    QDateTime date;
}AllStationsReport;

#endif // DBFILLTABLES_H
