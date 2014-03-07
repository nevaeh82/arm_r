#ifndef OBJECTSREGISTRY_H
#define OBJECTSREGISTRY_H
#include <QMap>
#include <QObject>
#include "PwGisClient.h"
#include <IObjectsRegistry.h>

namespace Pw {
namespace Gis {

class GISCLIENT_EXPORT ObjectsRegistry : public QObject, public IObjectsRegistry
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{5aa23cd3-a750-40dd-8233-f770251421ea}")
	Q_CLASSINFO("InterfaceID", "{9078c4af-9103-400a-bcdc-378afb00966d}")
	Q_CLASSINFO("CoClassAlias", "ObjectsRegistry")

	QMap< QString, IMapObject* > _objects;

public:
	ObjectsRegistry(QObject* parent = 0);

public slots:
	/// add object to registry
	/// @param id objects id
	/// @param obj objects pointer
	virtual void addObject(const QString& id, IMapObject* obj);

	/// Method returns object pointer by its id. Returns NULL if object not found
	/// @param id objects id
	virtual IMapObject* getObjectById(const QString& id);

	/// Method shows if object wtih specified id contains in registry
	/// @param id objects id
	virtual bool isObjectExist(const QString& id);

	/// Method removes object with specified id from registry
	/// @param id objects id
	virtual void removeObject(const QString& id);

	/// Method returns Path object by its id. Returns NULL if object not found
	/// @param id objects id
	virtual Path* getPath(const QString& id);

	/// Method returns Circle object by its id. Returns NULL if object not found
	/// @param id objects id
	virtual Circle* getCircle(const QString& id);

	/// Method returns Marker object by its id. Returns NULL if object not found
	/// @param id objects id
	virtual Marker* getMarker(const QString& id);

	/// Method returns Polygon object by its id. Returns NULL if object not found
	/// @param id objects id
	virtual Polygon* getPolygon(const QString& id);

	/// Method returns Rectangle object by its id. Returns NULL if object not found
	/// @param id objects id
	virtual Rectangle* getRectangle(const QString& id);

	/// Method returns Sector object by its id. Returns NULL if object not found
	/// @param id objects id
	virtual Sector* getSector(const QString& id);
};

} // Gis
} // Pw

#endif // OBJECTSREGISTRY_H
