#ifndef ISTATION_H
#define ISTATION_H

#include <QString>

class IStation
{
public:
	virtual ~IStation(){}

	virtual int  getId() const = 0;
	virtual QString getName() const = 0;
	virtual double getLatitude() const = 0;
	virtual double getLongitude() const = 0;
	virtual QString getPrm300Ip() const = 0;
	virtual QString getAdcIp() const = 0;
	virtual quint16 getAdcPort() const = 0;
};

#endif // ISTATION_H
