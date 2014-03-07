#ifndef PATH_H
#define PATH_H
#include <QMutex>
#include <QObject>
#include "PrimitiveMapObjectAbstract.h"

class PwGisPointList;

class GISCLIENT_EXPORT Path : public PrimitiveMapObjectAbstract
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{336ad991-28fe-4736-bf40-01603a78f1c8}")
	Q_CLASSINFO("InterfaceID", "{625d91fe-5de2-4a9e-bba2-cafa5e85b5c6}")

	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)

private:
	QString _toolTip;
	PwGisPointList* _path;
	QMutex _mutex;

	friend class MapObjectsFactory;

protected:
	explicit Path(IMapFunctions *map, QString id, QString name = "", QObject *parent = NULL);

public:

	explicit Path(QString id, QString name = "", QObject* parent = NULL);
	explicit Path(QObject* parent);

	virtual ~Path();
    virtual void updateMap();
	QString toolTip() const;

public slots:
	PwGisPointList* points() const;

	void setToolTip(QString arg);
};

#endif // PATH_H
