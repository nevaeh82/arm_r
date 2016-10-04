#ifndef ISTATION_H
#define ISTATION_H

#include <QString>
#include <QPoint>

typedef struct SpectrumSelection
{
	QPointF start;
	QPointF end;
} SpectrumSelection;

class IStation
{
public:
	virtual ~IStation(){}

	virtual int  getId() const = 0;
	virtual quint32  getPlatformId() const = 0;
	virtual quint32  getChannelId() const = 0;

	virtual QString getName() const = 0;
	virtual double getLatitude() const = 0;
	virtual double getLongitude() const = 0;
	virtual QString getPrm300Ip() const = 0;
	virtual QString getAdcIp() const = 0;
	virtual quint16 getAdcPort() const = 0;

	virtual float getBandwidth() const = 0;
	virtual float getShift() const = 0;
	virtual float getCenter() const = 0;


	virtual void setSelectedArea(const SpectrumSelection& selection) = 0;
	virtual void recognize() = 0;
};

#endif // ISTATION_H
