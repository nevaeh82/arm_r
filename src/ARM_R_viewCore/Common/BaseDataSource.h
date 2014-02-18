#ifndef PLAYERDATASOURCE_H
#define PLAYERDATASOURCE_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QVector>
#include <QPointF>

#include "Interfaces/IDataSource.h"
#include "Interfaces/IDataSourceListener.h"

class BaseDataSource : public QObject, public IDataSource
{
	Q_OBJECT
protected:
	QList<IDataSourceListener*> m_listeners;

public:
	explicit BaseDataSource(QObject *parent = 0);

	void registerListener(IDataSourceListener*);
	void deregisterListener(IDataSourceListener*);

	void sendCommand(int) = 0;

protected:
	void onDataReceived(const QString& method, const QVariant& arg);
	void onDataReceived(float* spectrum, float* spectrumPeakHold, int pointCount, double bandwidth, bool isComplex);
	void onDataReceived(float* spectrum, float* spectrumPeakHold);

signals:
	
public slots:
	
};

#endif // PLAYERDATASOURCE_H
