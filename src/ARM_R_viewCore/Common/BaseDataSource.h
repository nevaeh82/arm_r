#ifndef PLAYERDATASOURCE_H
#define PLAYERDATASOURCE_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QVector>
#include <QPointF>

#include "Interfaces/IDataSource.h"
#include "Interfaces/IDataSourceListener.h"

#include "BaseSubject.h"

class BaseDataSource : public QObject, public BaseSubject<IDataSourceListener>, public IDataSource
{
	Q_OBJECT
public:
	explicit BaseDataSource(QObject *parent = 0);

	void sendCommand(int) = 0;

protected:
	void onDataReceived(const QString& method, const QVariant& arg);

signals:
	
public slots:
	
};

#endif // PLAYERDATASOURCE_H
