#include "ListsAddController.h"

#include <QDebug>

ListsAddController::ListsAddController(const QSqlDatabase& db, QObject* parent):
	QObject(parent)
{
	m_db = db;
}

ListsAddController::~ListsAddController()
{
}

void ListsAddController::appendView(ListsAdd *widget)
{
	m_view = widget;

	m_view->fillStation(getNames("Station"));
	m_view->fillCategory(getNames("Category"));
	m_view->fillSignalType(getNames("SignalType"));

	connect(m_view, SIGNAL(signalAccept(QStringList)), this, SLOT(slotAccept(QStringList)));

}

QStringList ListsAddController::getNames(QString table)
{
	QSqlQuery query;

	QString str = QString("SELECT name FROM %1 GROUP BY name").arg(table);

			qDebug() << str;

	bool succeeded = query.prepare(str);

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return QStringList();
	}

//	query.bindValue(":objectTableName", table);

	succeeded = query.exec();

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return QStringList();
	}

	QStringList list;
	while (query.next())
	{
		list.append(query.value(0).toString());
	}

	return list;
}

void ListsAddController::slotAccept(const QStringList &list)
{
	qDebug() << list.count();

	QSqlQuery query;
//	QString str1 = QString("SELECT id FROM Station WHERE name=%1")
	bool succeeded = query.prepare("SELECT sd.id FROM stationDevices AS sd "\
								   "WHERE sd.port=:objectPort " \
								   "AND sd.stationID=(SELECT id FROM station WHERE name=:objectStationName)");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	query.bindValue(":objectStationName", list.at(0));
	query.bindValue(":objectPort", list.at(1));


	succeeded = query.exec();
	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	int deviceID = -1;
	if(!query.next())
	{
		QSqlQuery queryInsert;

		bool succeeded = queryInsert.prepare("INSERT INTO stationDevices VALUES(NULL, :objectPort, (SELECT id FROM station WHERE name=:objectStationName))");

		if (!succeeded) {
			qDebug() << "SQL is wrong!" <<  queryInsert.lastError();
			return;
		}
		queryInsert.bindValue(":objectPort", list.at(1));
		queryInsert.bindValue(":objectStationName", list.at(0));


		succeeded = queryInsert.exec();
		if (!succeeded) {
			qDebug() << "SQL is wrong!" <<  query.lastError();
			return;
		}

		deviceID = queryInsert.lastInsertId().toInt();
	}
	else
	{
		deviceID = query.value(0).toInt();
	}

//	QSqlQuery query;
//	QString str1 = QString("SELECT id FROM Station WHERE name=%1")
	succeeded = query.prepare("SELECT id FROM Category WHERE name=:objectName");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	query.bindValue(":objectName", list.at(2));

	succeeded = query.exec();
	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	if(!query.next())
	{
		return;
	}

	int categoryID = query.value(0).toInt();


	///signal type
	succeeded = query.prepare("SELECT id FROM signalType WHERE name=:objectName");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	query.bindValue(":objectName", list.at(5));

	succeeded = query.exec();
	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	if(!query.next())
	{
		return;
	}

	int signalTypeID = query.value(0).toInt();

	/// check for the same record
	succeeded = query.prepare("SELECT bandwidth FROM stationData WHERE (deviceID=:objectDeviceID AND categoryID=:objectCategoryID AND frequency=:objectFrequency)");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	query.bindValue(":objectDeviceID", deviceID);
	query.bindValue(":objectCategoryID", categoryID);
	query.bindValue(":objectFrequency", list.at(3).toDouble());
//	query.bindValue(":objectBandwidth", list.at(4).toDouble());

	succeeded = query.exec();
	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	if(query.next())
	{
		if(query.value(0).toDouble() < list.at(4).toDouble())
		{
			succeeded = query.prepare("SELECT id FROM stationData WHERE (deviceID=:objectDeviceID AND categoryID=:objectCategoryID AND frequency=:objectFrequency)");

			if (!succeeded) {
				qDebug() << "SQL is wrong!" <<  query.lastError();
				return;
			}

			query.bindValue(":objectDeviceID", deviceID);
			query.bindValue(":objectCategoryID", categoryID);
			query.bindValue(":objectFrequency", list.at(3).toDouble());
		//	query.bindValue(":objectBandwidth", list.at(4).toDouble());

			succeeded = query.exec();
			if (!succeeded) {
				qDebug() << "SQL is wrong!" <<  query.lastError();
				return;
			}

			query.next();
			int sid = query.value(0).toInt();

			succeeded = query.prepare("UPDATE stationData SET bandwidth=:objectBandwidth1 WHERE id=:objectID");

			if (!succeeded) {
				qDebug() << "SQL is wrong!" <<  query.lastError();
				return;
			}

			query.bindValue(":objectBandwidth1", list.at(4).toDouble());
			query.bindValue(":objectID", sid);

			succeeded = query.exec();
			if (!succeeded) {
				qDebug() << "SQL is wrong!" <<  query.lastError();
				return;
			}
		}
		return;
	}

	/// insert new record

	succeeded = query.prepare("INSERT INTO stationData VALUES(NULL, :objectDeviceID, :objectCategoryID, :objectFrequency, :objectBandwidth, :objectSignalType, :objectDate)");

	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}

	query.bindValue(":objectDeviceID", deviceID);
	query.bindValue(":objectCategoryID", categoryID);
	query.bindValue(":objectFrequency", list.at(3).toDouble());
	query.bindValue(":objectBandwidth", list.at(4).toDouble());
	query.bindValue(":objectSignalType", signalTypeID);
	query.bindValue(":objectDate", QDateTime::currentDateTime());

	succeeded = query.exec();
	if (!succeeded) {
		qDebug() << "SQL is wrong!" <<  query.lastError();
		return;
	}
}
