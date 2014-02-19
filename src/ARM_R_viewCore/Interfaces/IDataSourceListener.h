#ifndef IPLAYERDATASOURCELISTENER_H
#define IPLAYERDATASOURCELISTENER_H

class QString;
class QVariant;
class QPointF;

#include <QVector>
#include <QList>

class IDataSourceListener
{
	public:
		virtual ~IDataSourceListener(){}

		virtual void onDataArrived(const QString& method, const QVariant& arg) = 0;
		virtual void onDataArrived(float* spectrum, float* spectrumPeakHold, int pointCount, double bandwidth, bool isComplex) = 0;
		virtual void onDataArrived(float* spectrum, float* spectrumPeakHold) = 0;
};

#endif // IPLAYERDATASOURCELISTENER_H
