#ifndef STATIONHELPER_H
#define STATIONHELPER_H

#include <QWidget>

#include "DBStation/DBFillTables.h"

namespace StationHelper
{
	void showStationExistsWarning( StationData data, QWidget* parent = 0 );
	void showStationWarning( int result, QWidget* parent = 0 );
	QString translateCategory( const QString& category );
}

#endif // STATIONHELPER_H
