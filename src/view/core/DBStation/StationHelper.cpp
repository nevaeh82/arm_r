#include <QIcon>
#include <QMessageBox>

#include "DBStation/IDBStation.h"
#include "DBStation/StationHelper.h"

namespace StationHelper
{

QString translateCategory(const QString& category)
{
	if( category == "White" ) {
		return QObject::tr( "White list" );
	}

	if( category == "Black" ) {
		return QObject::tr( "Black list" );
	}

	return category;
}

void showStationExistsWarning( StationData data, QWidget* parent )
	{
		QMessageBox msg( parent );
		msg.setIcon( QMessageBox::Warning );
		msg.setWindowTitle( QObject::tr("Error") );
		msg.setWindowIcon( QIcon( ":/images/icons/ExistInDB.png" ) );
		msg.setText( QObject::tr("Frequency and bandwidth is exist!\n %1\n %2\n %3\n %4")
						.arg( QObject::tr( "Station = %1" ).arg( data.stationName ))
						.arg( QObject::tr( "Frequency = %1 MHz" ).arg( data.frequency ) )
						.arg( QObject::tr( "Bandwidth = %1 MHz" ).arg( data.bandwidth ) )
						.arg( QObject::tr( "Category = %1" ).arg( translateCategory( data.category ) ) ) );
		msg.exec();
	}

	void showStationWarning(int result, QWidget* parent)
	{
		QString message;
		switch( result ) {
			case ERROR_ADD_STATION_DATA_INVALID_DEVICE:
				message = QObject::tr( "Invalid station name or port number" );
				break;

			case ERROR_ADD_STATION_DATA_INVALID_CATEGORY:
				message = QObject::tr( "Invalid category name" );
				break;

			case ERROR_ADD_STATION_DATA_INVALID_SIGNAL_TYPE:
				message = QObject::tr( "Invalid signal type name" );
				break;

			case 0:
				message = QObject::tr( "An unexpected error occurred" );
				break;

			default:
				return;
		}

		QMessageBox::warning( parent, QObject::tr( "Error" ), message );
	}
}

#include "DBStation/StationHelper.h"
