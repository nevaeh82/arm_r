#ifndef DBSTATIONCONTROLLER_H
#define DBSTATIONCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include <QDateTime>

#include "DBConnectionParameters.h"

#define INVALID_INDEX 0

class DBStationController : public QObject
{
	Q_OBJECT
public:
	explicit DBStationController(QObject *parent = 0);
	virtual ~DBStationController();

public:
	virtual bool setDatabases(DBConnectionStruct parameters);
	virtual int addStation(QString name, QString ip);
	virtual int addStationDevice(QString name, unsigned short port);
	virtual int addSignalType(QString name);
	virtual int addCategory(QString name);
	virtual int addStationData(stationData data);
	virtual int getLastIndex(QString table);
	virtual int getStationID(QString name);

private:
	bool m_openDB();

private:
	QSqlDatabase m_db;

signals:

public slots:

};

#endif // DBSTATIONCONTROLLER_H
