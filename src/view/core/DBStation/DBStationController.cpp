#include <QDebug>
#include <Logger/Logger.h>
#include <QSplashScreen>

#include "DBStationController.h"

//log_error( QString( "SQL is wrong! Error = %1").arg( query.lastError().text() ) );

#define __VALIDATE_QUERY( query, result ) \
	if( query.lastError().type() != QSqlError::NoError ) { \
		return result; \
	}

#define VALIDATE_QUERY( query )			__VALIDATE_QUERY( query, INVALID_INDEX );
#define VALIDATE_QUERY_VOID( query )	__VALIDATE_QUERY( query, );

DBStationController::DBStationController(QObject *parent) :
	QObject(parent)
{
	if( QSqlDatabase::contains("DATACONNECTION") ) {
		m_db =  QSqlDatabase::database( "DATACONNECTION" );
	}
	else {
		m_db = QSqlDatabase::addDatabase( "QMYSQL", "DATACONNECTION" );
		m_db.setConnectOptions( "MYSQL_OPT_RECONNECT=1" );
	}
}

DBStationController::~DBStationController()
{
	if(m_db.isOpen()){
		m_db.close();
	}
}

QSqlDatabase DBStationController::getDataBase()
{
	return m_db;
}

bool DBStationController::connectToDB(const DBConnectionStruct& parameters)
{
	if( m_db.isOpen() ){
		m_db.close();
	}

	m_db.setHostName(parameters.host);
	m_db.setPort(parameters.port);
	m_db.setPassword(parameters.password);
	m_db.setUserName(parameters.login);
	m_db.setDatabaseName(parameters.dbName);

	if( !m_db.open() ) {
		QString resErrorString = m_db.lastError().databaseText() + "\n" + m_db.lastError().driverText();
		log_error( resErrorString );
		return false;
	}

	return true;
}

StationsDataTableModel* DBStationController::createQueryModel(QObject* parent)
{
	StationsDataTableModel* model = new StationsDataTableModel( m_db, parent ? parent : this );
	return model;
}

int DBStationController::addStation(const QString& name, const QString& ip)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare("INSERT INTO station VALUES(NULL, :objectName, :objectIP);");

	VALIDATE_QUERY( query );

	query.bindValue(":objectName", name);
	query.bindValue(":objectIP", ip);
	query.exec();

	VALIDATE_QUERY( query );

	return query.lastInsertId().toUInt();
}

int DBStationController::addStationDevice(const QString& name, const unsigned short& port)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare("INSERT INTO stationDevices VALUES(0, :port, (SELECT id FROM station WHERE name=:station));");

	VALIDATE_QUERY( query );

	query.bindValue(":port", port);
	query.bindValue(":station", name);
	query.exec();

	VALIDATE_QUERY( query );

	return query.lastInsertId().toUInt();
}

int DBStationController::addSignalType(const QString& name)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare("INSERT INTO signalType VALUES(NULL, :objectName);");

	VALIDATE_QUERY( query );

	query.bindValue(":objectName", name);
//	query.bindValue(":objectCode", code);
	query.exec();

	VALIDATE_QUERY( query );

	return query.lastInsertId().toUInt();
}

int DBStationController::addCategory(const QString& name)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare("INSERT INTO category VALUES(NULL, :objectName);");

	VALIDATE_QUERY( query );

	query.bindValue(":objectName", name);
	query.exec();

	VALIDATE_QUERY( query );

	return query.lastInsertId().toUInt();
}

int DBStationController::addStationData(const StationData& data)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	// check for correct names
	int deviceID = getDeviceID( data.stationName, data.port );
	if( deviceID == INVALID_INDEX ) return ERROR_ADD_STATION_DATA_INVALID_DEVICE;

	int categoryID = getCategoryID( data.category );
	if( categoryID == INVALID_INDEX ) return ERROR_ADD_STATION_DATA_INVALID_CATEGORY;

	int signalTypeID = getSignalTypeID( data.signalType );
	if( signalTypeID == INVALID_INDEX ) return ERROR_ADD_STATION_DATA_INVALID_SIGNAL_TYPE;

	QSqlQuery query( m_db );
	query.prepare("INSERT INTO stationData VALUES(NULL, " \
				  "(SELECT id FROM stationDevices WHERE stationID=(SELECT id from station WHERE name=:station) AND port=:port), " \
				  "(SELECT id FROM category WHERE name=:category), :frequency, :bandwidth, " \
				  "(SELECT id FROM signalType WHERE name=:objectSignalType), :dateTime, :comment, :checked);");

	VALIDATE_QUERY( query );

	query.bindValue( ":station", data.stationName );
	query.bindValue( ":port", data.port );
	query.bindValue( ":category", data.category );
	query.bindValue( ":frequency", data.frequency );
	query.bindValue( ":bandwidth", data.bandwidth );
	query.bindValue( ":signalType", data.signalType );
	query.bindValue( ":dateTime", QDateTime::currentDateTime() );
	query.bindValue( ":comment", data.comment );
    query.bindValue( ":checked", 1);
	query.exec();

	VALIDATE_QUERY( query );

	foreach( IDBStationListener* listener, m_receiversList ) {
		listener->onStationDataInserted( data );
	}

    log_debug(query.lastError().text());
	return query.lastInsertId().toUInt();
}

int DBStationController::updateStationData(const StationData& data)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	// check for correct names
	int deviceID = getDeviceID( data.stationName, data.port );
	if( deviceID == INVALID_INDEX ) return ERROR_ADD_STATION_DATA_INVALID_DEVICE;

	int categoryID = getCategoryID( data.category );
	if( categoryID == INVALID_INDEX ) return ERROR_ADD_STATION_DATA_INVALID_CATEGORY;

	int signalTypeID = getSignalTypeID( data.signalType );
	if( signalTypeID == INVALID_INDEX ) return ERROR_ADD_STATION_DATA_INVALID_SIGNAL_TYPE;

	QSqlQuery query( m_db );
	query.prepare( "UPDATE stationData " \
				   "SET bandwidth = :bandwidth, signalTypeID = :signalType, datetime = :datetime, comment = :comment, checked = :checked " \
				   "WHERE deviceID = :device AND categoryID = :category AND frequency = :frequency" );

	VALIDATE_QUERY( query );

	query.bindValue( ":bandwidth", data.bandwidth );
	query.bindValue( ":signalType", signalTypeID );
	query.bindValue( ":datetime", QDateTime::currentDateTime() );
	query.bindValue( ":device", deviceID );
	query.bindValue( ":category", categoryID );
	query.bindValue( ":frequency", data.frequency );
	query.bindValue( ":comment", data.comment );
    query.bindValue( ":checked", data.checked);
	query.exec();

	VALIDATE_QUERY( query );

	foreach( IDBStationListener* listener, m_receiversList ) {
		listener->onStationDataUpdated( data );
	}

	return 1;
}

bool DBStationController::deleteStationData(int id)
{
	if( !m_db.isOpen() ) {
		return false;
	}

	QSqlQuery query( m_db );
	query.prepare( "DELETE FROM StationData WHERE id = :id" );

	__VALIDATE_QUERY( query, false );

	query.bindValue( ":id", id );
	query.exec();

	__VALIDATE_QUERY( query, false );

	return true;
}

int DBStationController::getLastIndex(const QString& table)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare(tr("SELECT id FROM %1 ORDER BY id DESC LIMIT 1").arg(table));

	VALIDATE_QUERY( query );

//	query.bindValue(":table", table);

	query.exec();

//	int i = 0;
//	while(query.next())
//	{
		query.next();
//		qDebug() << 0 << query.record().value(0).toString();

//	}

	VALIDATE_QUERY( query );

	return query.record().value(0).toUInt();
}

int DBStationController::getStationID(const QString& name)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

    QSqlQuery queryAdd( m_db );
    bool erradd = queryAdd.prepare( "INSERT into station (name, ip) VALUES (:name,:ip)" );
    VALIDATE_QUERY( queryAdd );

    queryAdd.bindValue( "name", name );
    queryAdd.bindValue( "ip", "ip" );
    erradd = queryAdd.exec();

	QSqlQuery query( m_db );
	bool err = query.prepare( "SELECT id FROM station WHERE name=:name" );

	query.bindValue( "name", name );
	err = query.exec();

	VALIDATE_QUERY( query );

	if( query.next() ) {
		return query.record().value(0).toUInt();
	}
	return INVALID_INDEX;
}

int DBStationController::getDeviceID(const QString& stationName, int port)
{
	int stationID = getStationID( stationName );
	if( stationID == INVALID_INDEX ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare( "SELECT sd.id FROM stationDevices AS sd "\
								   "WHERE sd.port = :port AND sd.stationID = :station" );

	VALIDATE_QUERY( query );

	query.bindValue( ":station", stationID );
	query.bindValue( ":port", port );
	query.exec();

	VALIDATE_QUERY( query );

	if( query.next() ) {
		return query.value(0).toInt();
	}

	query.prepare( "INSERT INTO stationDevices VALUES(NULL, :port, :station)" );

	VALIDATE_QUERY( query );

	query.bindValue( ":port", port );
	query.bindValue( ":station", stationID );
	query.exec();

	VALIDATE_QUERY( query );

	return query.lastInsertId().toInt();
}

int DBStationController::getCategoryID(const QString& name)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare( "SELECT id FROM Category WHERE name = :name" );

	VALIDATE_QUERY( query );

	query.bindValue( ":name", name );
	query.exec();

	VALIDATE_QUERY( query );

	if( !query.next() ) {
		return INVALID_INDEX;
	}

	return query.value(0).toInt();
}

int DBStationController::getSignalTypeID(const QString& name)
{
	if( !m_db.isOpen() ) {
		return INVALID_INDEX;
	}

	QSqlQuery query( m_db );
	query.prepare( "SELECT id FROM signalType WHERE name = :name" );

	VALIDATE_QUERY( query );

	query.bindValue( ":name", name );
	query.exec();

	VALIDATE_QUERY( query );

	if( !query.next() ) {
		return INVALID_INDEX;
	}

	return query.value(0).toInt();
}

StationData DBStationController::getStationData(const QString& stationName, int port, double frequency, double bandwidth)
{
	StationData data;

	if( !m_db.isOpen() ) {
		return data;
	}

	QSqlQuery query( m_db );

	QString select = "SELECT cat.name, st.name, sd.frequency, sd.bandwidth " \
			"FROM stationData AS sd " \
			"INNER JOIN stationDevices as sdi on sd.deviceID = sdi.id " \
			"INNER JOIN station as st on st.id = sdi.stationID " \
			"INNER JOIN category AS cat on cat.id = sd.categoryID " \
			"INNER JOIN signalType AS stype on sd.signalTypeID = stype.id " \
			"WHERE st.name = :station AND sdi.port = :port";

	// first try to get records with equals frequency
	query.prepare( select + " AND sd.frequency = :frequency LIMIT 1" );

	__VALIDATE_QUERY( query, data );

	query.bindValue( ":station", stationName );
	query.bindValue( ":port", port );
	query.bindValue( ":frequency", frequency );
	query.exec();

	__VALIDATE_QUERY( query, data );

	// if there is no same records, try to get nearest
	if( query.size() == 0 ) {
		query.prepare( select + " AND sd.frequency > :lowFreq AND sd.frequency < :highFreq LIMIT 1" );

		__VALIDATE_QUERY( query, data );

		query.bindValue( ":station", stationName );
		query.bindValue( ":port", port );
		query.bindValue( ":lowFreq", frequency - bandwidth / 2 );
		query.bindValue( ":highFreq", frequency + bandwidth / 2 );
		query.exec();
	}

	__VALIDATE_QUERY( query, data );

	if( query.next() ) {
		data.stationName = stationName;
		data.port = port;
		data.category = query.value(0).toString();
		data.signalType = query.value(1).toString();
		data.frequency = query.value(2).toDouble();
		data.bandwidth = query.value(3).toDouble();
	}

	return data;
}

bool DBStationController::getStationInfo(const QString& name, QList<StationDataFull>& stationRecords)
{
	if( !m_db.isOpen() ) {
		return false;
	}

	QSqlQuery query( m_db );
	query.prepare("SELECT sdi.id, st.name AS stationName, " \
					"st.ip AS stationIP, sd.port, cat.name AS CategoryName, " \
					"sdi.frequency, sdi.bandwidth, sigType.name AS signalType, " \
					"sdi.datetime FROM stationData AS sdi " \
					"INNER JOIN stationDevices as sd ON sdi.deviceID=sd.id " \
					"INNER JOIN station AS st ON st.name=:objectName and st.id=sd.stationID " \
					"INNER JOIN category AS cat ON sdi.categoryID=cat.id " \
					"INNER JOIN signalType AS sigType ON sdi.signalTypeID=sigType.id");

	__VALIDATE_QUERY( query, false );

	query.bindValue(":objectName", name);
	query.exec();

	__VALIDATE_QUERY( query, false );

	while( query.next() ) {
		StationDataFull data;
		data.stationShortInfo.stationName = query.value(1).toString();
		data.stationIP = query.value(2).toString();
		data.stationShortInfo.port = query.value(3).toUInt();
		data.stationShortInfo.category = query.value(4).toString();
		data.stationShortInfo.frequency = query.value(5).toDouble();
		data.stationShortInfo.bandwidth = query.value(6).toDouble();
		data.stationShortInfo.signalType = query.value(7).toString();
		data.date = query.value(8).toDateTime();
		data.stationShortInfo.comment = query.value(9).toString();
		data.stationShortInfo.checked = query.value(10).toBool();
		stationRecords.append(data);
	}

	return true;
}

bool DBStationController::getFrequencyAndBandwidthByCategory(const QString &category, QList<StationsFrequencyAndBandwith> &list, bool withChecked)
{
	if( !m_db.isOpen() ) {
		return false;
	}

	QSqlQuery query( m_db );
	query.prepare( "SELECT s.name, st.frequency, st.bandwidth, st.checked " \
					"FROM stationData AS st " \
					"INNER JOIN stationDevices as sdi on st.deviceID=sdi.id " \
					"INNER JOIN station as s on s.id=sdi.stationID " \
					"INNER JOIN category AS cat on st.categoryID=cat.id " \
					"WHERE cat.name=:objectName" );

	VALIDATE_QUERY( query );

	query.bindValue(":objectName", category);
	query.exec();

	VALIDATE_QUERY( query );

	list.clear();

	while(query.next()) {
		StationsFrequencyAndBandwith data;
		data.stationName = query.value(0).toString();
		data.frequency = query.value(1).toDouble();
		data.bandwidth = query.value(2).toDouble();
		int tmp = query.value(3).toInt();
		data.isChecked = query.value(3).toBool();
		if(data.isChecked || withChecked) {
			list.append(data);
		}
	}

	return true;
}

bool DBStationController::getReportCategory(const QString &category, QList<AllStationsReport> &listReport)
{
    if( !m_db.isOpen() ) {
        return false;
    }

    QSqlQuery query( m_db );
    if(category.isEmpty())
    {
        query.prepare( "SELECT st.datetime, s.name, st.frequency, st.bandwidth, cat.name " \
                        "FROM stationData AS st " \
                        "INNER JOIN stationDevices as sdi on st.deviceID=sdi.id " \
                        "INNER JOIN station as s on s.id=sdi.stationID " \
                        "INNER JOIN category AS cat on st.categoryID=cat.id ");

        VALIDATE_QUERY( query );

    } else {
        query.prepare( "SELECT st.datetime, s.name, st.frequency, st.bandwidth, cat.name " \
                        "FROM stationData AS st " \
                        "INNER JOIN stationDevices as sdi on st.deviceID=sdi.id " \
                        "INNER JOIN station as s on s.id=sdi.stationID " \
                        "INNER JOIN category AS cat on st.categoryID=cat.id " \
                        "WHERE cat.name=:objectName" );

        VALIDATE_QUERY( query );

        query.bindValue(":objectName", category);
    }

    query.exec();

    VALIDATE_QUERY( query );

    while(query.next()) {
        AllStationsReport data;
        data.date = query.value(0).toDateTime();
        data.sfab.stationName = query.value(1).toString();
        data.sfab.frequency = query.value(2).toDouble();
        data.sfab.bandwidth = query.value(3).toDouble();
        data.category = query.value(4).toString();
        listReport.append(data);
    }

    return true;
}

QStringList DBStationController::getCatalog(const QString& name)
{
	if( !m_db.isOpen() ) {
		return QStringList();
	}

	QSqlQuery query( m_db );
	QString sql = QString( "SELECT name FROM %1 GROUP BY name" ).arg( name );

	query.exec( sql );

	__VALIDATE_QUERY( query, QStringList() );

	QStringList list;
	while( query.next() ) {
		list << query.value(0).toString();
	}

	return list;
}

//Thresholds

bool DBStationController::setThresholdByFrequencyAndStation(const QString &station, const int frequency, const int threshold)
{
	if( !m_db.isOpen() ) {
		return false;
	}

	QSqlQuery query( m_db );
	QMap<int, int> list;
	bool isRes = getThresholdByFrequencyAndStation(station, frequency, list);

	if(isRes) {
		bool res = false;
		res = query.prepare( "UPDATE threshold " \
							 "SET threshold=:thresholdVal " \
							 "WHERE frequency=:freqVal AND station=:stationVal" );

		QString err = query.lastError().text();

		if(!res) {
			return false;
		}

		VALIDATE_QUERY( query );

		query.bindValue(":freqVal", frequency);
		query.bindValue(":thresholdVal", threshold);
		query.bindValue(":stationVal", station);
		res = query.exec();
		QString error = query.lastError().text();

		if(!res) {
			return false;
		}
		return true;
	} /*else {

		bool res = false;
		res = query.prepare( "INSERT INTO threshold " \
							 "VALUES (NULL, :freqVal, :thresholdVal, :stationVal);" );

		if(!res) {
			return false;
		}

		VALIDATE_QUERY( query );

		query.bindValue(":freqVal", frequency);
		query.bindValue(":thresholdVal", threshold);
		query.bindValue(":stationVal", station);
		res = query.exec();
		QString error = query.lastError().text();

		if(!res) {
			return false;
		}
		return true;
	}*/

	return true;
}

bool DBStationController::getThresholdByFrequencyAndStation(const QString &station, const int frequency, QMap<int, int>& thresholdList)
{
	if( !m_db.isOpen() ) {
		return false;
	}

	QSqlQuery query( m_db );
	query.prepare( "SELECT frequency, threshold FROM threshold " \
					"WHERE station=:stationVal" );

	VALIDATE_QUERY( query );

	query.bindValue(":freqVal", frequency);
	query.bindValue(":stationVal", station);

	bool result = query.exec();

	QString qqq = query.executedQuery();
	QMap<QString, QVariant> tmpMap = query.boundValues();

	if(!result) {
		return false;
	}

	QString error = query.lastError().text();

	int tmp1 = query.size();

	while( query.next() ) {
		int frequency = query.value(0).toInt();
		int value = query.value(1).toInt();
		thresholdList.insert(frequency, value);
	}


	if(!thresholdList.contains(frequency)){

		bool res = false;
		res = query.prepare( "INSERT INTO threshold " \
							 "VALUES (NULL, :freqVal, :thresholdVal, :stationVal);" );

		if(!res) {
			return false;
		}

		VALIDATE_QUERY( query );

		query.bindValue(":freqVal", frequency);
		query.bindValue(":thresholdVal", 0);
		query.bindValue(":stationVal", station);
		res = query.exec();
		QString error = query.lastError().text();

		if(!res) {
			return false;
		}

		return true;
	}

	return true;
}

