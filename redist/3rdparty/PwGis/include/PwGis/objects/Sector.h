#ifndef SECTOR_H
#define SECTOR_H
#include <QString>
#include <QPainter>

#include "PrimitiveMapObjectAbstract.h"

class IMapProvider;
class IMapObject;
class PwGisLonLat;

/// Sector tactic map object
class GISCLIENT_EXPORT Sector : public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{ce1818cb-06ab-40de-98a7-49d2b2d86641}")
	Q_CLASSINFO("InterfaceID", "{59006342-af7e-4183-b3e7-f9633551e223}")

	Q_PROPERTY (PwGisLonLat* originPoint READ originPoint WRITE setOriginPoint)
	Q_PROPERTY (double radius READ radius WRITE setRadius)
	Q_PROPERTY (double startAngle READ startAngle WRITE setStartAngle)
	Q_PROPERTY (double endAngle READ endAngle WRITE setEndAngle)
	Q_PROPERTY (QString toolTip READ toolTip WRITE setToolTip)

	friend class MapObjectsFactory;

private:
	PwGisLonLat* _originPoint;
	double _radius;
	double _startAngle;
	double _endAngle;
	QString _toolTip;

	inline QRadialGradient parseGradient(QVariant var);

protected:
	explicit Sector(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Sector(QString id, QString name = "", QObject *parent = NULL);
	explicit Sector(QObject *parent);

    virtual void updateMap();

	PwGisLonLat* originPoint() const;

	double radius() const;

	double startAngle() const;

	double endAngle() const;

	QString toolTip() const;

	~Sector();

public slots:

	void setOriginPoint(const PwGisLonLat* originPoint);

	void setRadius(double radius);

	void setStartAngle(double startAngle);

	void setEndAngle(double endAngle);

	void setToolTip(QString toolTip);
};

#endif // SECTOR_H
