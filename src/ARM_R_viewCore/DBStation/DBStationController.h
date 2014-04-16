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

#include "Info/DBConnectionParameters.h"

#include "DBStation/IDBStation.h"
#include "DBStation/DBFillTables.h"

#define INVALID_INDEX 0

class dbStationTest;

class DBStationController : public QObject, public IDBStation
{
	Q_OBJECT

	friend class dbStationTest;

public:
	explicit DBStationController(QObject *parent = 0);
	virtual ~DBStationController();

	virtual QSqlDatabase getDataBase();

	// interface IDBStation
	virtual bool connectToDB(const DBConnectionStruct& parameters);

	virtual StationsDataTableModel* createQueryModel(QObject* parent = 0);

	virtual int addStation(const QString& name, const QString& ip);
	virtual int addStationDevice(const QString& name, const unsigned short& port);
	virtual int addSignalType(const QString& name);
	virtual int addCategory(const QString& name);
	virtual int addStationData(const StationData& data);

	virtual int updateStationData(const StationData& data);

	virtual bool deleteStationData(int id);

	virtual StationData getStationData(const QString& stationName, int port, double frequency, double bandwidth);
	virtual bool getStationInfo(const QString& name, QList<StationDataFull>& stationRecords);
	virtual bool getFrequencyAndBandwidthByCategory(const QString& category, QList<StationsFrequencyAndBandwith>& list);

	virtual inline QStringList getStationsCatalog() { return getCatalog( "station" ); }
	virtual inline QStringList getCategoriesCatalog() { return getCatalog( "category" ); }
	virtual inline QStringList getSignalTypesCatalog() { return getCatalog( "signalType" ); }

private:
	QSqlDatabase m_db;

protected:
	QStringList getCatalog(const QString& name);

	int getLastIndex(const QString& table);
	int getStationID(const QString& name);
	int getDeviceID(const QString& stationName, int port);
	int getCategoryID(const QString& name);
	int getSignalTypeID(const QString& name);

signals:

public slots:

};

#endif // DBSTATIONCONTROLLER_H
