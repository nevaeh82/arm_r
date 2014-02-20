#include "TabsProperty.h"

Station::Station(QObject *parent):
	QObject(parent)
{
	m_stationId = INVALID_STATION_ID;
	m_stationName = tr("Unknown");
	m_stationLatitude = 0;
	m_stationLongitude = 0;
	m_prm300Ip = "127.0.0.1";
	m_adcIp = "127.0.0.1";
	m_adcPort = 0;
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
