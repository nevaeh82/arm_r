#include <Logger.h>

#include "DBStation/StationHelper.h"

#include "AddStationDataDialogController.h"

AddStationDataDialogController::AddStationDataDialogController(DBStationController* stationDb, QObject* parent)
	: QObject( parent )
	, m_view( 0 )
	, m_stationDb( stationDb )
{
	m_db = stationDb->getDataBase();
}

AddStationDataDialogController::~AddStationDataDialogController()
{
}

void AddStationDataDialogController::appendView(AddStationDataDialog *widget)
{
	m_view = widget;

	m_view->fillStation(getNames("Station"));
	m_view->fillCategory(getNames("Category"));
	m_view->fillSignalType(getNames("SignalType"));

	connect( m_view, SIGNAL(accepted(StationData)), this, SLOT(insert(StationData) ) );
}

QStringList AddStationDataDialogController::getNames(QString table)
{
	QSqlQuery query(m_db);

	QString str = QString("SELECT name FROM %1 GROUP BY name").arg(table);

	bool succeeded = query.prepare(str);
	if (!succeeded) {
		log_error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QStringList();
	}

//	query.bindValue(":objectTableName", table);

	succeeded = query.exec();

	if (!succeeded) {
		log_error(QString("SQL is wrong! Error = %1").arg(query.lastError().text()));
		return QStringList();
	}

	QStringList list;
	while (query.next()) {
		list << query.value(0).toString();
	}

	return list;
}

void AddStationDataDialogController::insert(const StationData& data)
{
	int result = 1;

	StationData dataExist = m_stationDb->
			getStationData( data.stationName, data.port, data.frequency, data.bandwidth );

	if( !dataExist.stationName.isNull() ) {
		if( dataExist.bandwidth < data.bandwidth ) {
			result = m_stationDb->updateStationData( data );
		} else {
			StationHelper::showStationExistsWarning( dataExist, m_view );
			return;
		}
	} else {
		result = m_stationDb->addStationData(data);
	}

	if( result < 1 ) {
		StationHelper::showStationWarning( result, m_view );
		return;
	}

	m_view->close();
}
