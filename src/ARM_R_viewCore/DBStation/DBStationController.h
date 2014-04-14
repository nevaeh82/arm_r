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

#include <QIcon>
#include <QMessageBox>

#include <BaseSubject.h>

#include "IDBStation.h"
#include "IDBStationListener.h"
#include "Info/DBConnectionParameters.h"
#include "DBFillTables.h"

#define INVALID_INDEX 0

class DBStationController : public QObject, public IDBStation, public BaseSubject<IDBStationListener>
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

	virtual int updateStationData(const StationData& data);

	virtual int getLastIndex(const QString& table);
	virtual int getStationID(const QString& name);
	virtual int getDeviceID(const QString& stationName, int port);
	virtual int getCategoryID(const QString& name);
	virtual int getSignalTypeID(const QString& name);

	virtual StationData getStationData(const QString& stationName, int port, double frequency, double bandwidth);

//	virtual QString getStationIP(const QString& name);
	virtual bool getStationInfo(const QString& name, QList<StationDataFull>& stationRecords);
	virtual bool getFrequencyAndBandwidthByCategory(const QString& category, QList<StationsFrequencyAndBandwith>& list);

private:
	QSqlDatabase m_db;

private:
	bool checkExist(double frequency, double bandwidth, StationData& data);

signals:

public slots:

};

#endif // DBSTATIONCONTROLLER_H
