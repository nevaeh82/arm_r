#ifndef DBCONNECTIONPARAMETERS_H
#define DBCONNECTIONPARAMETERS_H

#include <QString>

/// parameters for connect to mysql
typedef struct DBConnectionStruct
{
	QString host;
	unsigned short port;
	QString login;
	QString password;
	QString dbName;
}DBConnectionStruct;

#endif // DBCONNECTIONPARAMETERS_H
