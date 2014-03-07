#ifndef CIRCLE_H
#define CIRCLE_H
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisLonLat;
class  GISCLIENT_EXPORT Circle: public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{919f7ab2-00db-4f1a-9458-84724c1e0a6a}")
	Q_CLASSINFO("InterfaceID", "{e38d759e-5353-4524-813a-008db4e6086d}")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(PwGisLonLat* originPoint READ originPoint WRITE setOriginPoint)
	Q_PROPERTY(int radius READ radius WRITE setRadius)

	friend class MapObjectsFactory;

private:
	QString _toolTip;
	PwGisLonLat* _center;
	int _radius;

protected:
	explicit Circle(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Circle(QString id, QString name = "", QObject* parent = NULL);
	explicit Circle(QObject* parent = NULL);

	virtual ~Circle();
    virtual void updateMap();
	QString toolTip() const;
	PwGisLonLat* originPoint() const;
	void setOriginPoint(const PwGisLonLat* point);

	int radius() const;

	void setToolTip(QString arg);
	void setRadius(int radius);
public slots:

};

#endif // CIRCLE_H
