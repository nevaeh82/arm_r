#include <Logger.h>

#include "DBStation/StationHelper.h"

#include "AddStationDataDialogController.h"

AddStationDataDialogController::AddStationDataDialogController(IDBStation* stationDb, QObject* parent)
	: QObject( parent )
	, m_view( 0 )
	, m_stationDb( stationDb )
{
}

AddStationDataDialogController::~AddStationDataDialogController()
{
}

void AddStationDataDialogController::appendView(AddStationDataDialog *widget)
{
	m_view = widget;

	m_view->fillStation( m_stationDb->getStationsCatalog() );
	m_view->fillCategory( m_stationDb->getCategoriesCatalog() );
	m_view->fillSignalType( m_stationDb->getSignalTypesCatalog() );

	connect( m_view, SIGNAL(accepted(StationData)), this, SLOT(insert(StationData) ) );
}

void AddStationDataDialogController::insert(const StationData& data)
{
	int result = 1;

	// looking for segments that included into the new one
	StationData dataExist = m_stationDb->
			getStationData( data.stationName, data.port, data.frequency, data.bandwidth );

	if( !dataExist.stationName.isNull() ) {
		if( dataExist.frequency == data.frequency && dataExist.bandwidth < data.bandwidth ) {
			// if it really exists and new bandwidth is bigger than current, update it
			result = m_stationDb->updateStationData( data );
		} else {
			// otherwice show message about duplicate
			StationHelper::showStationExistsWarning( dataExist, m_view );
			return;
		}
	} else {
		// if it is new data, insert it into DB
		result = m_stationDb->addStationData(data);
	}

	// show error message, if result is not good
	if( result < 1 ) {
		StationHelper::showStationWarning( result, m_view );
		return;
	}

	m_view->close();
}
