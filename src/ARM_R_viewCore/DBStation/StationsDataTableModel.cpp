#include "StationsDataTableModel.h"

#include <QSqlQuery>

StationsDataTableModel::StationsDataTableModel(QSqlDatabase database, QObject *parent)
	: QSqlQueryModel( parent )
	, m_database( database )
{
	update();
}

void StationsDataTableModel::update()
{
	// ping server before update model query
	QSqlQuery ping( "SELECT 1", m_database );
	ping.exec();

	QString sql = "SELECT sdi.id, st.name AS stationName, " \
			"st.ip AS stationIP, sd.port, cat.name AS CategoryName, " \
			"sdi.frequency, sdi.bandwidth, sigType.name AS signalType, " \
			"sdi.datetime FROM stationData AS sdi " \
			"INNER JOIN stationDevices as sd ON sdi.deviceID=sd.id " \
			"INNER JOIN station AS st ON st.id=sd.stationID " \
			"INNER JOIN category AS cat ON sdi.categoryID=cat.id " \
			"INNER JOIN signalType AS sigType ON sdi.signalTypeID=sigType.id " \
			"ORDER BY stationName";

	setQuery( sql, m_database );
}
