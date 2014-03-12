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
}StationData;

/// full info
typedef struct StationDataFull
{
	StationData stationShortInfo;
	QString		stationIP;
	QDateTime	date;
}StationDataFull;

#endif // DBFILLTABLES_H
