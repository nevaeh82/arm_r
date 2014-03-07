#ifndef IMAPFACTORY_H
#define IMAPFACTORY_H

class Circle;
class Marker;
class Path;
class Polygon;
class Rectangle;
class Sector;
class PwGisLonLat;

/// This interface provides map objects factory functionality
class IObjectsFactory
{
public:
	virtual ~IObjectsFactory(){}


	/// Method performs Circle map object creation
	/// @return Circle map object pointer
	virtual Circle* createCircle() = 0;

	/// Method performs Marker map object creation
	/// @return Marker map object pointer
	virtual Marker* createMarker() = 0;

	/// Method performs Polygon map object creation
	/// @return Path map object pointer
	virtual Path* createPath() = 0;

	/// Method performs Polygon map object creation
	/// @return Polygon map object pointer
	virtual Polygon* createPolygon() = 0;

	/// Method performs Rectangle map object creation
	/// @return Rectangle map object pointer
	virtual Rectangle* createRectangle() = 0;

	/// Method performs Sector map object creation
	/// @return Sector map object pointer
	virtual Sector* createSector() = 0;

	/**
	 * @brief Create a LonLat point.
	 * @param lon Point longitude.
	 * @param lat Point latitude.
	 * @return Created point.
	 */
	virtual PwGisLonLat* createGeoPoint(double lon, double lat)=0;
};

#endif // IMAPFACTORY_H
