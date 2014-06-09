#ifndef STRUCTS_H
#define STRUCTS_H

#include <QTime>
#include <QPointF>

typedef struct UAVPositionDataEnemy
{
	UAVPositionDataEnemy()
	{
		altitude	= 0.0;
		altitudeStdDev = 0.0;
		speed		= 0.0;
		course		= 0.0;
		state		= 0;
		frequency	= -1;
		time		= QTime::currentTime();
		latLonStdDev	= QPointF(0.0, 0.0);
		sourceType  = 0;
	}

	UAVPositionDataEnemy(const UAVPositionDataEnemy& object)
	{
		altitude	= object.altitude;
		altitudeStdDev = object.altitudeStdDev;
		speed		= object.speed;
		course		= object.course;
		state		= object.state;
		frequency	= object.frequency;
		time		= object.time;
		latLonStdDev	= object.latLonStdDev;
		latLon		= object.latLon;
		sourceType  = object.sourceType;
	}
	double		altitude;
	double		altitudeStdDev;
	double		speed;
	double		course;
	int			state;
	double		frequency;
	QTime		time;
	QPointF		latLonStdDev;
	QPointF		latLon;
	uint		sourceType;

} UAVPositionDataEnemy;

#endif // STRUCTS_H
