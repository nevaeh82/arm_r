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

#include "Rpc/RpcFlakonClientWrapper.h"

#include "TabSpectrumWidgetController.h"
#include "TabSpectrumWidget.h"

#include "AtlantTabWidget.h"

#include "DBStation/DBStationController.h"

#include "Info/StationConfiguration.h"
#include "Info/AtlantConfiguraton.h"

#include "../ControlPanel/ControlPanelController.h"

class TabManager: public QObject, public ITabManager, public IControlPanelListener
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

	RpcFlakonClientWrapper *m_rpcFlakonClient;

	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;

	DBStationController* m_dbStationController;
	ICorrelationListener* m_controlPanelController;
	ControlPanelController*	m_panelController;

public:
	TabManager(QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();

	void setRpcConfig(const quint16& port, const QString& host);

	void start();
	void setDbManager(IDbManager* dbManager);
	void setDbStationController(DBStationController* controller);
	void setFlakonRpc(RpcFlakonClientWrapper *rpcFlakonClient, QString rpcHost, uint rpcPort);

	virtual QString getStationName(const int id);
	virtual void setActiveTab(const int id);

	virtual void onGlobalAutoSearchEnabled(const bool isEnabled);
	virtual void onGlobalPanoramaEnabled(const bool isEnabled);

	QStringList createStationNamesList();

	void setStationsConfiguration(const QList<StationConfiguration>& stationList);
	void setAtlantConfiguration(const AtlantConfiguration& atlantConfig);

	QMap<int, Station *>& getStations();

	void addStationTabs();
	void clearAllInformation();

	void setControlPanelController(ICorrelationListener* controller);
	void setControlPanelController(ControlPanelController *controller);

	void setResponseCommonFreq(quint32 freq);private:
	int readStationSettings(const QString &settingsFile);
	void readRpcSettings();	

private slots:
	void changeTabSlot(int index);
    void slotUpdateDBStationsLists();

signals:
	void readyToStart();
};

#endif // TABMANAGER_H
