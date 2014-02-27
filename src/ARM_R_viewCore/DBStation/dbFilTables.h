#ifndef DBFILTABLES_H
#define DBFILTABLES_H

#include <QString>

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

#endif // DBFILTABLES_H
