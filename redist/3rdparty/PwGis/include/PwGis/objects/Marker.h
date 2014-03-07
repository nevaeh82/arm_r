#ifndef MARKER_H
#define MARKER_H
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisLonLat;
class GISCLIENT_EXPORT Marker: public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{0f13d736-f108-4c47-9ba7-15f36fd002a3}")
	Q_CLASSINFO("InterfaceID", "{c9c932e5-442a-4872-8e07-274c389b92cc}")

	friend class MapObjectsFactory;

private:
	QString _toolTip;
	PwGisLonLat* _position;
	double _rotate;

protected:
	explicit Marker(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:
	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(double rotate READ rotate WRITE setRotate)
	Q_PROPERTY(PwGisLonLat* position READ position WRITE setPosition)

	explicit Marker(QString id, QString name = "", QObject* parent = NULL);
	explicit Marker(QObject* parent = NULL);

	virtual ~Marker();
    virtual void updateMap();
	QString toolTip() const;
	PwGisLonLat* position() const;
	void setPosition(const PwGisLonLat* position);
	double rotate() const;

public slots:

	void setToolTip(QString arg);
	void setRotate(double arg);
};

#endif // MARKER_H
