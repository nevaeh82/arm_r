#ifndef POLYGON_H
#define POLYGON_H
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"
#include "../PwGisClient.h"

class PwGisPointList;
class GISCLIENT_EXPORT Polygon : public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{9476cda7-2d09-45b3-99bb-aab672fef476}")
	Q_CLASSINFO("InterfaceID", "{add086f7-c4c8-4a3b-97e0-63f789100186}")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)

	friend class MapObjectsFactory;

private:
	QString _toolTip;
	PwGisPointList* _corners;

protected:
	explicit Polygon(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Polygon(QString id, QString name = "", QObject* parent = NULL);
	explicit Polygon(QObject* parent = NULL);

	virtual ~Polygon();
    virtual void updateMap();
	QString toolTip() const;
	void setToolTip(QString arg);

public slots:
	PwGisPointList* corners() const;

};

#endif // POLYGON_H
