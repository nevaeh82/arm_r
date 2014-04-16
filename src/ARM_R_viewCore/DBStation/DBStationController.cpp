#include <QDebug>
#include <Logger.h>

#include "DBStationController.h"

#define __VALIDATE_QUERY( query, result ) \
	if( query.lastError().type() != QSqlError::NoError ) { \
		log_error( QString( "SQL is wrong! Error = %1").arg( query.lastError().text() ) ); \
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

	QSqlQuery query(m_db);
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

	QSqlQuery query(m_db);
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

	QSqlQuery query(m_db);
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

	QSqlQuery query(m_db);
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
				  "(SELECT id FROM signalType WHERE name=:objectSignalType), :dateTime);");

	VALIDATE_QUERY( query );

	query.bindValue( ":station", data.stationName );
	query.bindValue( ":port", data.port );
	query.bindValue( ":category", data.category );
	query.bindValue( ":frequency", data.frequency );
	query.bindValue( ":bandwidth", data.bandwidth );
	query.bindValue( ":signalType", data.signalType );
	query.bindValue( ":dateTime", QDateTime::currentDateTime() );
	query.exec();

	VALIDATE_QUERY( query );

	foreach( IDBStationListener* listener, m_receiversList ) {
		listener->onStationDataInserted( data );
	}

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

	QSqlQuery query;
	query.prepare( "UPDATE stationData " \
				   "SET bandwidth = :bandwidth AND signalTypeID = :signalType AND datetime = :datetime" \
				   "WHERE deviceID = :device AND categoryID = :category AND frequency = :frequency" );

	VALIDATE_QUERY( query );

	query.bindValue( ":bandwidth", data.bandwidth );
	query.bindValue( ":signalType", signalTypeID );
	query.bindValue( ":datetime", QDateTime::currentDateTime() );
	query.bindValue( ":device", deviceID );
	query.bindValue( ":category", categoryID );
	query.bindValue( ":frequency", data.frequency );
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

	QSqlQuery query(m_db);
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

	QSqlQuery query( m_db );
	query.prepare( "SELECT id FROM station WHERE name = :name" );

	VALIDATE_QUERY( query );

	query.bindValue( "name", name );
	query.exec();

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

	QSqlQuery query(m_db);

	query.prepare( "SELECT cat.name, st.name, sd.frequency, sd.bandwidth " \
					"FROM stationData AS sd " \
					"INNER JOIN stationDevices as sdi on sd.deviceID = sdi.id " \
					"INNER JOIN station as st on st.id = sdi.stationID " \
					"INNER JOIN category AS cat on cat.id = sd.categoryID " \
					"INNER JOIN signalType AS stype on sd.signalTypeID = stype.id " \
					"WHERE st.name = :station AND sdi.port = :port " \
					"AND sd.frequency >= :lowFreq AND sd.frequency <= :highFreq" );

	__VALIDATE_QUERY( query, data );

	query.bindValue( ":station", stationName );
	query.bindValue( ":port", port );
	query.bindValue( ":lowFreq", frequency - bandwidth / 2 );
	query.bindValue( ":highFreq", frequency + bandwidth / 2 );
	query.exec();

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

	QSqlQuery query(m_db);
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
		stationRecords.append(data);
	}

	return true;
}

bool DBStationController::getFrequencyAndBandwidthByCategory(const QString &category, QList<StationsFrequencyAndBandwith> &list)
{
	if( !m_db.isOpen() ) {
		return false;
	}

	QSqlQuery query(m_db);
	query.prepare("SELECT s.name, st.frequency, st.bandwidth " \
					"FROM stationData AS st " \
					"INNER JOIN stationDevices as sdi on st.deviceID=sdi.id " \
					"INNER JOIN station as s on s.id=sdi.stationID " \
					"INNER JOIN category AS cat on st.categoryID=cat.id " \
					"WHERE cat.name=:objectName");

	VALIDATE_QUERY( query );

	query.bindValue(":objectName", category);
	query.exec();

	VALIDATE_QUERY( query );

	while(query.next()) {
		StationsFrequencyAndBandwith data;
		data.stationName = query.value(0).toString();
		data.frequency = query.value(1).toDouble();
		data.bandwidth = query.value(2).toDouble();
		list.append(data);
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

