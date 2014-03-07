#ifndef SOURCEBEHAVIOURSTYLE_H
#define SOURCEBEHAVIOURSTYLE_H

#include <QObject>

class BearingDynamicsStyle;
class TrackingStyle;
class MapObjectBehaviourStyle;

class SourceBehaviourStyle : public QObject
{
	Q_OBJECT
	Q_PROPERTY (BearingDynamicsStyle* dynamicStyle READ dynamicStyle)
	Q_PROPERTY (TrackingStyle* trackingStyle READ trackingStyle)

protected:
	MapObjectBehaviourStyle* _behaviourStyle;

public:
	explicit SourceBehaviourStyle(QObject *parent = 0);
	BearingDynamicsStyle* dynamicStyle() const;
	TrackingStyle* trackingStyle() const;
};

#endif // SOURCEBEHAVIOURSTYLE_H
