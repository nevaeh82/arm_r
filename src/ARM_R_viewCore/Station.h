#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QString>

#include "Interfaces/IStation.h"

#define INVALID_STATION_ID		-1

class Station: public QObject, public IStation
{
	Q_OBJECT

private:
	int m_stationId;
	QString m_stationName;

	double m_stationLatitude;
	double m_stationLongitude;

	QString m_prm300Ip;
	QString m_adcIp;
	quint16 m_adcPort;

public:
	Station(QObject* parent = NULL);
	virtual ~Station();

	int  getId() const;
	QString getName() const;
	double getLatitude() const;
	double getLongitude() const;
	QString getPrm300Ip() const;
	QString getAdcIp() const;
	quint16 getAdcPort() const;

	void setId(int id);
	void setName(const QString& name);
	void setLatitude(const double lat);
	void setLongitude(const double lon);
	void setPrm300Ip(const QString& ip);
	void setAdcIp(const QString& ip);
	void setAdcPort(const quint16 port);
};

#endif // STATION_H
