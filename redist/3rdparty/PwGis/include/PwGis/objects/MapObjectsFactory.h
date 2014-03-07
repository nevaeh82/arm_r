#ifndef MAPOBJECTSFACTORY_H
#define MAPOBJECTSFACTORY_H
#include <QObject>
#include "IObjectsFactory.h"
#include "PwGisClient.h"

class IIdGenerator;
class PwGisLonLat;
class IMapFunctions;

namespace Pw{ namespace Gis{
	class IObjectsRegistry;
}}

class GISCLIENT_EXPORT MapObjectsFactory: public QObject, public IObjectsFactory
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{5f4ee129-6b75-4296-9b94-d97f5ed55331}")
	Q_CLASSINFO("InterfaceID", "{dbece90c-7fb8-4a0b-a4af-3930fdb9bd9d}")

private:
	IIdGenerator* _idGenerator;
	IMapFunctions* _map;
	Pw::Gis::IObjectsRegistry* _registry;

public:
	explicit MapObjectsFactory(QObject* parent = NULL);
	explicit MapObjectsFactory(IIdGenerator* idGenerator, IMapFunctions* map, Pw::Gis::IObjectsRegistry* registry, QObject* parent = NULL);

public slots:
	virtual Circle* createCircle();
	virtual Marker* createMarker();
	virtual Path* createPath();
	virtual Polygon* createPolygon();
	virtual Rectangle* createRectangle();
	virtual Sector* createSector();

	/**
	 * @brief Create a LonLat point.
	 * @param lon Point longitude.
	 * @param lat Point latitude.
	 * @return Created point.
	 */
	virtual PwGisLonLat* createGeoPoint(double lon, double lat);
};

#endif // MAPOBJECTSFACTORY_H
