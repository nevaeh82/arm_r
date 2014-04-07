#ifndef DBSTATIONCONTROLLER_H
#define DBSTATIONCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include <QDateTime>
#include <QList>

#include "IDBStation.h"
#include "DBConnectionParameters.h"
#include "DBFillTables.h"

#define INVALID_INDEX 0

class DBStationController : public QObject, public IDBStation
{
	Q_OBJECT
public:
	explicit DBStationController(QObject *parent = 0);
	virtual ~DBStationController();

public:
	virtual QSqlDatabase getDataBase();
	virtual bool connectToDB(const DBConnectionStruct& parameters);
	virtual int addStation(const QString& name, const QString& ip);
	virtual int addStationDevice(const QString& name, const unsigned short& port);
	virtual int addSignalType(const QString& name);
	virtual int addCategory(const QString& name);
	virtual int addStationData(const StationData& data);
	virtual int getLastIndex(const QString& table);
	virtual int getStationID(const QString& name);
//	virtual QString getStationIP(const QString& name);
	virtual bool getStationInfo(const QString& name, QList<StationDataFull>& stationRecords);
	virtual bool getFrequencyAndBandwidthByCategory(const QString& category, QList<StationsFrequencyAndBandwith>& list);

private:
	QSqlDatabase m_db;

signals:

public slots:

};

#endif // DBSTATIONCONTROLLER_H
