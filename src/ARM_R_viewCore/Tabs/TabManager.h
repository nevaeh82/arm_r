#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include <QString>
#include <QSettings>
#include <QMap>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QMutex>
#include <QList>

#include "Correlations/CorrelationControllersContainer.h"

#include "Station.h"

#include "Interfaces/IDBManager.h"
#include "Interfaces/ITabManager.h"
#include "Interfaces/IControlPanelListener.h"

#include "Rpc/RpcFlakonClient.h"

#include "TabSpectrumWidgetController.h"
#include "TabSpectrumWidget.h"

#include "AtlantTabWidget.h"

#include "DBStation/DBStationController.h"

#include "Rpc/RpcSettingsManager.h"

#include "Info/StationConfiguration.h"
#include "Info/AtlantConfiguraton.h"

class TabManager: public QObject, public ITabManager, public IControlPanelListener, public IRpcListener
{
	Q_OBJECT

private:
	QTabWidget* m_tabWidget;

	QMap<int, Station *>   m_stationsMap;
	QMap<QString, ITabWidget* >    m_tabWidgetsMap;
	ICorrelationControllersContainer*	m_correlationControllers;

	IDbManager* m_dbManager;
	ITabWidget* m_currentTabWidget;

	QString m_rpcHost;
	uint m_rpcPort;

	RpcFlakonClient *m_rpcFlakonClient;

	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;

	DBStationController* m_dbStationController;

	IRpcSettingsManager* m_settingsManager;

public:
	TabManager(QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();

	void setRpcConfig(const quint16& port, const QString& host);

	void start();
	int createSubModules(const QString& settingsFile);
	void setDbManager(IDbManager* dbManager);
	void setDbStationController(DBStationController* controller);

	virtual QString getStationName(const int id);
	virtual void setActiveTab(const int id);

	virtual void onGlobalAutoSearchEnabled(const bool isEnabled);
	virtual void onGlobalPanoramaEnabled(const bool isEnabled);

	QStringList createStationNamesList();

private:
	int readStationSettings(const QString &settingsFile);
	void readRpcSettings();

	void setStationsConfiguration(const QList<StationConfiguration>& stationList);
	void setAtlantConfiguration(const AtlantConfiguration& atlantConfig);
	void addStationTabs();

private slots:
	void changeTabSlot(int index);

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
};

#endif // TABMANAGER_H
