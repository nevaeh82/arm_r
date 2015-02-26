#ifndef IDBSTATIONLISTENER_H
#define IDBSTATIONLISTENER_H

#include "DBStation/DBFillTables.h"

class IDBStationListener
{
public:
	virtual ~IDBStationListener() {}

	virtual void onStationDataInserted(const StationData& data) = 0;
	virtual void onStationDataUpdated(const StationData& data) = 0;
};

#endif // IDBSTATIONLISTENER_H
