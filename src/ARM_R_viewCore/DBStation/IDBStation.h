#ifndef IDBSTATION_H
#define IDBSTATION_H

#include <QString>
#include <QSqlDatabase>
#include "Info/DBConnectionParameters.h"
#include "DBFillTables.h"


class IDBStation
{
public:
	virtual ~IDBStation(){};
	virtual QSqlDatabase getDataBase()									= 0;
	virtual bool connectToDB(const DBConnectionStruct& parameters)		= 0;
	virtual int addStation(const QString& name, const QString& ip)		= 0;
	virtual int addStationDevice(const QString& name, const unsigned short& port)	= 0;
	virtual int addSignalType(const QString& name)						= 0;
	virtual int addCategory(const QString& name)						= 0;
	virtual int addStationData(const StationData& data)					= 0;
	virtual int getLastIndex(const QString& table)						= 0;
	virtual int getStationID(const QString& name)						= 0;
//	virtual QString getStationIP(const QString& name);
	virtual bool getStationInfo(const QString& name, QList<StationDataFull>& stationRecords)	= 0;
	virtual bool getFrequencyAndBandwidthByCategory(const QString& category, QList<StationsFrequencyAndBandwith>& list) = 0;


};

#endif // IDBSTATION_H
