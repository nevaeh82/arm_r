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

	deviceID = query.value(0).toInt();
}
