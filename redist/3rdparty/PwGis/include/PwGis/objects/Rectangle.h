#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "PrimitiveMapObjectAbstract.h"

class PwGisLonLat;
class GISCLIENT_EXPORT Rectangle: public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{91c6fc67-da60-4fe9-a739-a7f64631aed7}")
	Q_CLASSINFO("InterfaceID", "{80574021-7350-4f2e-a541-035f2a8bd4a9}")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(PwGisLonLat* bottomLeft READ bottomLeft WRITE setBottomLeft)
	Q_PROPERTY(PwGisLonLat* topRight READ topRight WRITE setTopRight)

	friend class MapObjectsFactory;

private:
	QString _toolTip;
	PwGisLonLat* _bottomLeft;
	PwGisLonLat* _topRight;

protected:
	explicit Rectangle(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:
	explicit Rectangle(QString id, QString name = "", QObject* parent = NULL);
	explicit Rectangle(QObject* parent);

	virtual ~Rectangle();
    virtual void updateMap();
	QString toolTip() const;
	PwGisLonLat* topRight() const;
	PwGisLonLat* bottomLeft() const;

	void setToolTip(QString arg);
	void setTopRight(PwGisLonLat* topRight);
	void setBottomLeft(PwGisLonLat* bottomLeft);

public slots:

};

#endif // RECTANGLE_H
