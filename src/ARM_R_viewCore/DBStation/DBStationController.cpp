#include "DBStationController.h"
#include <QDebug>

DBStationController::DBStationController(QObject *parent) :
	QObject(parent)
{
	if(QSqlDatabase::contains("DATACONNECTION"))
	{
		m_db =  QSqlDatabase::database("DATACONNECTION");
	}
	else
	{
		m_db = QSqlDatabase::addDatabase("QMYSQL", "DATACONNECTION");
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
	if(m_db.isOpen()){
		m_db.close();
	}

	m_db.setHostName(parameters.host);
	m_db.setPort(parameters.port);
	m_db.setPassword(parameters.password);
	m_db.setUserName(parameters.login);
	m_db.setDatabaseName(parameters.dbName);

	if (!m_db.open()) {
		QString resErrorString = m_db.lastError().databaseText() + "\n" + m_db.lastError().driverText();
		qDebug() << resErrorString;
		return false;
	}

	return true;

}

int DBStationController::addStation(const QString& name, const QString& ip)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO station VALUES(NULL, :objectName, :objectIP);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectName", name);
	query.bindValue(":objectIP", ip);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::addStationDevice(const QString& name, const unsigned short& port)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO stationDevices VALUES(0, :objectPort, (SELECT id FROM station WHERE name=:objectStationName));");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectPort", port);
	query.bindValue(":objectStationName", name);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::addSignalType(const QString& name)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO signalType VALUES(NULL, :objectName);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectName", name);
//	query.bindValue(":objectCode", code);


	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::addCategory(const QString& name)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO category VALUES(NULL, :objectName);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectName", name);

	succeeded = query.exec();

	if (succeeded)
	{
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::addStationData(const StationData& data)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("INSERT INTO stationData VALUES(NULL, "\
								   "(SELECT id FROM stationDevices WHERE stationID=(SELECT id from station WHERE name=:objectStationName) AND port=:objectPort), "\
								   "(SELECT id FROM category WHERE name=:objectCategory), :objectFrequency, :objectBandwidth, " \
								   "(SELECT id FROM signalType WHERE name=:objectSignalType), :objectDateTime);");

	if (!succeeded)
	{
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectStationName", data.stationName);
	query.bindValue(":objectPort", data.port);
	query.bindValue(":objectCategory", data.category);
	query.bindValue(":objectFrequency", data.frequency);
	query.bindValue(":objectBandwidth", data.bandwidth);
	query.bindValue(":objectSignalType", data.signalType);
	query.bindValue(":objectDateTime", QDateTime::currentDateTime());

	succeeded = query.exec();

	if (succeeded)
	{
//		qDebug() << "last " << query.lastInsertId().toUInt();
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::getLastIndex(const QString& table)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(tr("SELECT id FROM %1 ORDER BY id DESC LIMIT 1").arg(table));

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

//	query.bindValue(":table", table);

	succeeded = query.exec();

//	int i = 0;
//	while(query.next())
//	{
		query.next();
		qDebug() << 0 << query.record().value(0).toString();

//	}

	if (succeeded){
		return query.record().value(0).toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::getStationID(const QString& name)
{
	if(!m_db.isOpen())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare(tr("SELECT %1 FROM station WHERE name=%1").arg(name));

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}


	succeeded = query.exec();

//	int i = 0;
//	while(query.next())
//	{
		query.next();
		qDebug() << 0 << query.record().value(0).toString();

//	}

	if (succeeded)
	{
		return query.record().value(0).toUInt();
	}

	return INVALID_INDEX;
}

bool DBStationController::getStationInfo(const QString& name, QList<StationDataFull>& stationRecords)
{
	if(!m_db.isOpen())
	{
		return false;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT sdi.id, st.name AS stationName, " \
					"st.ip AS stationIP, sd.port, cat.name AS CategoryName, " \
					"sdi.frequency, sdi.bandwidth, sigType.name AS signalType, " \
					"sdi.datetime FROM stationData AS sdi " \
					"INNER JOIN stationDevices as sd ON sdi.deviceID=sd.id " \
					"INNER JOIN station AS st ON st.name=:objectName and st.id=sd.stationID " \
					"INNER JOIN category AS cat ON sdi.categoryID=cat.id " \
					"INNER JOIN signalType AS sigType ON sdi.signalTypeID=sigType.id");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return false;
	}

	query.bindValue(":objectName", name);


	succeeded = query.exec();
	if (!succeeded)
	{
		qDebug() << "SQL is wrong!";
		return false;
	}

	while(query.next())
	{
		qDebug() << 0 << query.value(0).toUInt();
		qDebug() << 1 << query.value(1).toString();
		qDebug() << 2 << query.value(2).toString();
		qDebug() << 3 << query.value(3).toUInt();
		qDebug() << 4 << query.value(4).toString();
		qDebug() << 5 << query.value(5).toDouble();
		qDebug() << 6 << query.value(6).toDouble();
		qDebug() << 7 << query.value(7).toString();
		qDebug() << 8 << query.value(8).toDateTime();
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

	if (succeeded)
	{
		return true;
	}
	return false;
}

void DBStationController::getDataByCategory(const QString& category, QList<FrequencyAndBandwidth> &frequencyRecords)
{
	if(!m_db.isOpen())
	{
		return;
	}

	QSqlQuery query(m_db);
	bool succeeded = query.prepare("SELECT s.frequency, s.bandwidth FROM stationData AS s " \
					"INNER JOIN category AS cat ON s.categoryID=cat.id " \
					"WHERE cat.name=:objectName");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	query.bindValue(":objectName", category);


	succeeded = query.exec();
	if (!succeeded)
	{
		qDebug() << "SQL is wrong!";
		return;
	}

	while(query.next())
	{
		qDebug() << 0 << query.value(0).toDouble();
		qDebug() << 1 << query.value(1).toDouble();

		FrequencyAndBandwidth data;
		data.frequency = query.value(0).toDouble();
		data.bandwidth = query.value(1).toDouble();

		frequencyRecords.append(data);
	}
}

