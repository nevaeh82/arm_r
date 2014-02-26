#include "dbstationcontroller.h"
#include <QDebug>

DBStationController::DBStationController(QObject *parent) :
	QObject(parent)
{
	m_db = QSqlDatabase::addDatabase("QMYSQL");
//	m_db.setHostName("localhost");
//	m_db.setPort(3306);
//	m_db.setPassword("qwerty12345");
//	m_db.setUserName("root");
//	m_db.setDatabaseName("stations");

//	if (!(m_db.open())) {
//		QString resErrorString=m_db.lastError().databaseText()+"\n"+m_db.lastError().driverText();
//		qDebug()<<resErrorString;
//		return;
//	}
}

DBStationController::~DBStationController()
{
	if(m_openDB())
	{
		m_db.close();
	}
}

bool DBStationController::setDatabases(DBConnectionStruct parameters)
{
	m_db.setHostName(parameters.host);
	m_db.setPort(parameters.port);
	m_db.setPassword(parameters.password);
	m_db.setUserName(parameters.login);
	m_db.setDatabaseName(parameters.dbName);

	if (!(m_db.open())) {
		QString resErrorString=m_db.lastError().databaseText()+"\n"+m_db.lastError().driverText();
		qDebug()<<resErrorString;
		return false;
	}

	return true;

}

int DBStationController::addStation(QString name, QString ip)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
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

int DBStationController::addStationDevice(QString name, unsigned short port)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
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

int DBStationController::addSignalType(QString name)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
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

int DBStationController::addCategory(QString name)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
	bool succeeded = query.prepare("INSERT INTO category VALUES(NULL, :objectName);");

	if (!succeeded) {
		qDebug() << "SQL is wrong!";
		return INVALID_INDEX;
	}

	query.bindValue(":objectName", name);

	succeeded = query.exec();

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::addStationData(stationData data)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
	bool succeeded = query.prepare("INSERT INTO stationData VALUES(NULL, "\
								   "(SELECT id FROM stationDevices WHERE stationID=(SELECT id from station WHERE name=:objectStationName) AND port=:objectPort), "\
								   "(SELECT id FROM category WHERE name=:objectCategory), :objectFrequency, :objectBandwidth, " \
								   "(SELECT id FROM signalType WHERE name=:objectSignalType), :objectDateTime);");

	if (!succeeded) {
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

	if (succeeded){
		return query.lastInsertId().toUInt();
	}

	return INVALID_INDEX;
}

int DBStationController::getLastIndex(QString table)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
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

int DBStationController::getStationID(QString name)
{
	if(!m_openDB())
	{
		return INVALID_INDEX;
	}

	QSqlQuery query;
	bool succeeded = query.prepare(tr("SELECT %1 FROM station WHERE name=%1").arg(name));

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

bool DBStationController::m_openDB()
{
	if(!m_db.isOpen())
	{
		if(!m_db.open())
		{
			return false;
		}
	}
	return true;
}
