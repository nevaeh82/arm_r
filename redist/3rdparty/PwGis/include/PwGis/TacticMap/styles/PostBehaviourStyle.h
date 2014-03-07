#ifndef POSTBEHAVIOURSTYLE_H
#define POSTBEHAVIOURSTYLE_H

#include <QObject>

class MapObjectBehaviourStyle;
class TrackingStyle;


class PostBehaviourStyle : public QObject
{
	Q_OBJECT
	Q_PROPERTY (TrackingStyle* trackingStyle READ trackingStyle)

protected:
	MapObjectBehaviourStyle* _behaviourStyle;

public:
	explicit PostBehaviourStyle(QObject *parent = 0);
	TrackingStyle* trackingStyle() const;
};

#endif // POSTBEHAVIOURSTYLE_H
