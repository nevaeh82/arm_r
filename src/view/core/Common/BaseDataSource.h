#ifndef PLAYERDATASOURCE_H
#define PLAYERDATASOURCE_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QVector>
#include <QPointF>

#include "Interfaces/IDataSource.h"
#include "Interfaces/IDataSourceListener.h"

#include "Logger/Logger.h"

#include "Tabs/LocationSetupWidgetController.h"


class BaseDataSource : public QObject, public IDataSource
{
	Q_OBJECT
public:
	explicit BaseDataSource(QObject *parent = 0);
	virtual ~BaseDataSource();

	void sendCommand(int) = 0;
	void setLocationController(LocationSetupWidgetController* controller);

protected:
	void onDataReceived(const QString& method, const QVariant& arg);
	LocationSetupWidgetController* m_locationController;

signals:
	void onDrawComplete();
	
public slots:
	
};

#endif // PLAYERDATASOURCE_H
