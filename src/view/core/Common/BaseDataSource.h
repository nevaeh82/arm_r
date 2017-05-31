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
#include "ControlPanel/ControlPanelController.h"


class BaseDataSource : public QObject, public IDataSource
{
	Q_OBJECT
public:
	explicit BaseDataSource(QObject *parent = 0);
	virtual ~BaseDataSource();

	void sendCommand(int) = 0;
	void setLocationController(LocationSetupWidgetController* controller);
	void setControlPanelController(ControlPanelController* controller);

protected:
	void onDataReceived(const QString& method, const QVariant& arg);
	LocationSetupWidgetController* m_locationController;
	ControlPanelController* m_panelController;

signals:
	void onDrawComplete();
    void dopplerStatus(QString name, double doppler);
	
public slots:
	
};

#endif // PLAYERDATASOURCE_H
