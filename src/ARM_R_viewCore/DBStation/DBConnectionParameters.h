#ifndef DBCONNECTIONPARAMETERS_H
#define DBCONNECTIONPARAMETERS_H

#include <QString>

typedef struct DBConnectionStruct
{
	QString host;
	unsigned short port;
	QString login;
	QString password;
	QString dbName;
}DBConnectionStruct;

typedef struct stationData
{
	QString stationName;
	unsigned short port;
	QString category;
	double frequency;
	double bandwidth;
	QString signalType;
}stationData;

#endif // DBCONNECTIONPARAMETERS_H
