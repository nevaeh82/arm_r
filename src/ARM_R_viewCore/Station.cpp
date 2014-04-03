#include "UiDefines.h"

#include "Station.h"

Station::Station(IDbManager* dbManager, RpcFlakonClient* client, QObject *parent)
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

void Station::setAdcPort(const quint16 port)
{
	m_adcPort = port;
}

void Station::setSelectedArea(const SpectrumSelection& selection)
{
	/// To HZ
	double x1 = selection.start.x() / 1000;
	double x2 = selection.end.x() / 1000;

	double dx = qAbs(x1 - x2);
	double center = (x1 + x2)/ 2;

	m_dbManager->updatePropertyValue( getName(), DB_SELECTED_PROPERTY, QString::number(dx, 'f', 3));
	m_dbManager->updatePropertyValue( getName(), DB_CENTER_PROPERTY, QString::number(center, 'f', 3));
	m_dbManager->updatePropertyValue( getName(), DB_START_PROPERTY, QString::number(x1, 'f', 3));
	m_dbManager->updatePropertyValue( getName(), DB_STOP_PROPERTY, QString::number(x2, 'f', 3));
}

void Station::recognize()
{
	float bandwidth = getBandwidth();
	float shift = getShift();

	m_rpcFlakonClient->sendBandwidth( this, bandwidth );
	m_rpcFlakonClient->sendShift( this, shift );
	m_rpcFlakonClient->recognize( this, 104 );
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

float Station::getBandwidth() const
{
	float bandwidth = 0;

	// try to get correct value from DB
	QVariant value = m_dbManager->getPropertyValue( getName(), DB_SELECTED_PROPERTY );
	if( value.canConvert( QVariant::Double ) ) {
		bandwidth = value.toFloat();
	}

	return bandwidth;
}

float Station::getShift() const
{
	float shift = 0;

	// try to get correct value from DB
	QVariant value = m_dbManager->getPropertyValue( getName(), DB_CENTER_PROPERTY );
	if( value.canConvert( QVariant::Double ) ) {
		shift = value.toFloat();
	}

	return shift;
}
