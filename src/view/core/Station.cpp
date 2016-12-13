#include "UiDefines.h"

#include "Station.h"

Station::Station(IDbManager* dbManager, RpcFlakonClientWrapper *client, QObject *parent)
	: QObject( parent )
	, m_dbManager( dbManager )
	, m_stationId( INVALID_STATION_ID )
	, m_stationName( tr("Unknown") )
	, m_stationLatitude( 0 )
	, m_stationLongitude( 0 )
	, m_prm300Ip( "127.0.0.1" )
	, m_adcIp( "127.0.0.1" )
	, m_adcPort( 0 )
	, m_rpcFlakonClient( client )
{
}

Station::~Station()
{
	//log_debug("<<<<<<");
}

void Station::setId(int id)
{
	m_stationId = id;
}

void Station::setName(const QString& name)
{
	m_stationName = name;
}

void Station::setLatitude(const double lat)
{
	m_stationLatitude = lat;
}

void Station::setLongitude(const double lon)
{
	m_stationLongitude = lon;
}

void Station::setPrm300Ip(const QString &ip)
{
	m_prm300Ip = ip;
}

void Station::setAdcIp(const QString& ip)
{
	m_adcIp = ip;
}

void Station::setCenter(const int val)
{
	m_center = val;
	m_dbManager->updatePropertyValue( getName(), DB_FREQUENCY_PROPERTY, val );
}

void Station::setAdcPort(const quint16 port)
{
	m_adcPort = port;
}

void Station::setPlatformId(const quint32 id)
{
	m_platformId = id;
}

void Station::setChannelId(const quint32 id)
{
	m_channelId = id;
}

void Station::setSelectedArea(const SpectrumSelection& selection)
{
	/// To HZ
	double x1 = selection.start.x();
	double x2 = selection.end.x();

	double dx = qAbs(x1 - x2);
	double center = (x1 + x2)/ 2;

	m_dbManager->updatePropertyValue( getName(), DB_SELECTED_PROPERTY, QString::number(dx, 'f', 3));
	m_dbManager->updatePropertyValue( getName(), DB_CENTER_PROPERTY, QString::number(center, 'f', 3));
	m_dbManager->updatePropertyValue( getName(), DB_START_PROPERTY, QString::number(x1, 'f', 3));
	m_dbManager->updatePropertyValue( getName(), DB_STOP_PROPERTY, QString::number(x2, 'f', 3));

	if( x1 != 0 && x2 != 0 ) {
		m_locationSetupController->setSpectrumSelection(x1, x2);
	}
}

void Station::recognize()
{
	float bandwidth = getBandwidth();
	float shift = getShift();

	m_rpcFlakonClient->sendBandwidth( m_stationId, bandwidth );
	m_rpcFlakonClient->sendShift( m_stationId, shift );
	m_rpcFlakonClient->recognize( m_stationId, 104 );
}

int Station::getId() const
{
	return m_stationId;
}

QString Station::getName() const
{
	return m_stationName;
}

double Station::getLatitude() const
{
	return m_stationLatitude;
}

double Station::getLongitude() const
{
	return m_stationLongitude;
}

QString Station::getPrm300Ip() const
{
	return m_prm300Ip;
}

QString Station::getAdcIp() const
{
	return m_adcIp;
}

quint16 Station::getAdcPort() const
{
	return m_adcPort;
}

quint32 Station::getPlatformId() const
{
	return m_platformId;
}

quint32 Station::getChannelId() const
{
	return m_channelId;
}

float Station::getBandwidth() const
{
	float bandwidth = 0;

	// try to get correct value from DB
	QVariant value = m_dbManager->getPropertyValue( getName(), DB_SELECTED_PROPERTY );
	if( value.canConvert( QVariant::Double ) ) {
        bandwidth = value.toFloat();
	}

    bandwidth *= 1000;

    return bandwidth;
}

float Station::getShift() const
{
	float shift = 0;

	float frequency = 0;

	float center = 0;

	// try to get correct value from DB
	QVariant freq = m_dbManager->getPropertyValue( getName(), DB_FREQUENCY_PROPERTY );
	if( freq.canConvert( QVariant::Double ) ) {
		frequency = freq.toFloat();
	}

	QVariant value = m_dbManager->getPropertyValue( getName(), DB_CENTER_PROPERTY );
	if( value.canConvert( QVariant::Double ) ) {
		center = value.toFloat();
	}

	shift = center-frequency;

	shift *= 1000;

	return shift;
}

float Station::getCenter() const
{
	float center = 0;

    QVariant value = m_dbManager->getPropertyValue( getName(), DB_FREQUENCY_PROPERTY );
	if( value.canConvert( QVariant::Double ) ) {
		center = value.toFloat();
	}

	return center;
}

void Station::setLocationSetupController(LocationSetupWidgetController *controller)
{
	m_locationSetupController = controller;
}

int Station::getCenterVal() const
{
	return m_center;
}
