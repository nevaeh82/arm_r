#ifndef DBFILLTABLES_H
#define DBFILLTABLES_H

#include <QString>
#include <QDateTime>

/// fill stationData table
typedef struct stationData
{
	QString stationName;
	unsigned short port;
	QString category;
	double frequency;
	double bandwidth;
	QString signalType;
}stationData;

/// full info
typedef struct StationDataFull
{
	stationData stationShortInfo;
	QString		stationIP;
	QDateTime	date;
}StationDataFull;

#endif // DBFILLTABLES_H
