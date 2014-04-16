#ifndef IDBSTATION_H
#define IDBSTATION_H

#include <QString>
#include <QSqlDatabase>

#include "Info/DBConnectionParameters.h"

#include <BaseSubject.h>

#include "DBStation/IDBStationListener.h"
#include "DBStation/StationsDataTableModel.h"
#include "DBStation/DBFillTables.h"

#define ERROR_ADD_STATION_DATA_INVALID_DEVICE			-1
#define ERROR_ADD_STATION_DATA_INVALID_CATEGORY			-2
#define ERROR_ADD_STATION_DATA_INVALID_SIGNAL_TYPE		-3

class IDBStation : public BaseSubject<IDBStationListener>
{
public:
	virtual ~IDBStation() {}

	virtual bool connectToDB(const DBConnectionStruct& parameters)		= 0;

	virtual StationsDataTableModel* createQueryModel(QObject* parent = 0) = 0;

	virtual int addStation(const QString& name, const QString& ip)		= 0;
	virtual int addStationDevice(const QString& name, const unsigned short& port)	= 0;
	virtual int addSignalType(const QString& name)						= 0;
	virtual int addCategory(const QString& name)						= 0;
	virtual int addStationData(const StationData& data)					= 0;

	virtual int updateStationData(const StationData& data)				= 0;

	virtual bool deleteStationData(int id) = 0;

	virtual StationData getStationData(const QString& stationName, int port, double frequency, double bandwidth = 0) = 0;

	virtual bool getStationInfo(const QString& name, QList<StationDataFull>& stationRecords)	= 0;
	virtual bool getFrequencyAndBandwidthByCategory(const QString& category, QList<StationsFrequencyAndBandwith>& list) = 0;

	/// Returns station names (stations catalog)
	virtual QStringList getStationsCatalog() = 0;

	/// Returns catigories names (catigories catalog)
	virtual QStringList getCategoriesCatalog() = 0;

	/// Returns signal types names (signal types catalog)
	virtual QStringList getSignalTypesCatalog() = 0;
};

#endif // IDBSTATION_H
