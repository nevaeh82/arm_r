#ifndef IDBSTATION_H
#define IDBSTATION_H

#include <QString>
#include <QSqlDatabase>
#include "Info/DBConnectionParameters.h"
#include "DBFillTables.h"

#define ERROR_ADD_STATION_DATA_INVALID_DEVICE			-1
#define ERROR_ADD_STATION_DATA_INVALID_CATEGORY			-2
#define ERROR_ADD_STATION_DATA_INVALID_SIGNAL_TYPE		-3

class IDBStation
{
public:
	virtual ~IDBStation() {}
	virtual QSqlDatabase getDataBase()									= 0;
	virtual bool connectToDB(const DBConnectionStruct& parameters)		= 0;
	virtual int addStation(const QString& name, const QString& ip)		= 0;
	virtual int addStationDevice(const QString& name, const unsigned short& port)	= 0;
	virtual int addSignalType(const QString& name)						= 0;
	virtual int addCategory(const QString& name)						= 0;
	virtual int addStationData(const StationData& data)					= 0;

	virtual int updateStationData(const StationData& data)				= 0;

	virtual StationData getStationData(const QString& stationName, int port, double frequency, double bandwidth) = 0;

//	virtual QString getStationIP(const QString& name);
	virtual bool getStationInfo(const QString& name, QList<StationDataFull>& stationRecords)	= 0;
	virtual bool getFrequencyAndBandwidthByCategory(const QString& category, QList<StationsFrequencyAndBandwith>& list) = 0;


};

#endif // IDBSTATION_H
