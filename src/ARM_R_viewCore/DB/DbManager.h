#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QThread>

#include <PwLogger/PwLogger.h>

#include "Interfaces/IDbManager.h"

#include "Interfaces/IDbController.h"
#include "DbController.h"
#include "UiDefines.h"

#include "Interfaces/ISettingsChangedListener.h"


class DbManager : public QObject, public IDbManager
{
	Q_OBJECT

private:
	Pw::Logger::ILogger* m_logger;

	DbController*       m_dbController;
	//QList<ISettingsChangedListener*> m_receiversList;

public:

	DbManager(QObject* parent  = NULL);
	virtual ~DbManager();

	//void registerDbChangedReceiver(ISettingsChangedListener *receiver);
	//void deregisterDbChangedReceiver(ISettingsChangedListener *receiver);

	SettingsNode getSettingsNode(const QString& objectName);
	bool updateProperty(const Property& property);

private:
	Object createSettingsNode(const QString& objectName);

	Object createObject(const QString& objectName);

//	Object createKTPSettingsNode(const QString& objectName);
//	Object createPhoenixSettingsNode(const QString& objectName);
//	Object createRoatingAntennaSettingsNode(const QString& objectName);

	Object createStation(const QString& stationName);


	void notifyDbChanged(const SettingsNode &category);
	void notifyDbChanged(const Property& property);


signals:
	void signalSetDataDBController(QMap<QString, QVariant> *);
};

#endif // DBMANAGER_H
