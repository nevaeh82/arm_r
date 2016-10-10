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

#include "../ControlPanel/ControlPanelController.h"

#include "Rpc/RpcFlakonClientWrapper.h"

#include "Station.h"

#include "Interfaces/IDBManager.h"
#include "Interfaces/ITabManager.h"
#include "Interfaces/IControlPanelListener.h"

#include "Rpc/RpcFlakonClientWrapper.h"

#include "TabSpectrumWidgetController.h"
#include "TabSpectrumWidget.h"

#include "DBStation/DBStationController.h"

#include "Info/StationConfiguration.h"

#include "../ControlPanel/ControlPanelController.h"
#include "SolverResultWidgetController.h"
#include "SolverErrorsWidgetController.h"
#include "SolverSetupWidgetController.h"
#include "RPC/RpcConfigClient.h"

#include "LocationSetupWidgetController.h"
#include "Controls/PanoramaFreqControl.h"

#include "DBStation/ListsDialogController.h"


class TabManager: public QObject, public ITabManager, public IControlPanelListener, public IRpcListener
{
	Q_OBJECT

private:
	QTabWidget* m_tabWidgetZone;
	QTabWidget* m_tabWidget;

	QMap<int, Station *>   m_stationsMap;
	QMap<QString, ITabWidget* >    m_tabWidgetsMap;
	ICorrelationControllersContainer* m_correlationControllers;
	QMap<int, IAnalysisWidget*> m_analysisControllertMap;

	IDbManager* m_dbManager;
	ITabWidget* m_currentTabWidget;

	QString m_rpcHost;
	uint m_rpcPort;

	RpcFlakonClientWrapper *m_rpcFlakonClient;

	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;

	DBStationController* m_dbStationController;
	ICorrelationListener* m_controlPanelController;
	ControlPanelController* m_panelController;
	int m_id;
	QThread *m_rpcClientThread;
	QThread *m_rpcClientConfigThread;
	ControlPanelWidget* m_cpWidget;

	LocationSetupWidgetController* m_locationSetupController;
	PanoramaFreqControl* m_panoramaFreqControl;

	RpcConfigClient* m_rpcConfigClient;

	QList<ITabWidget*> m_tabWidgetList;

	int m_tabId;
	int m_tabCount;

	QMap<int, QWidget*> m_tabWidgetContainer;

    ListsDialog* m_listForm;

private:
	// IRpcListener interface
	void readProto(const QByteArray &data);
	void initAnalysisControllers(int count);

public:
	TabManager(int id, QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();
	void initRpcConfig();
	void startTab(SolverResultWidgetController* resultSolver, SolverErrorsWidgetController* errorSolver, SolverSetupWidgetController* setupSolver);

	int getChannelCount();

	void setRpcFlakon(const quint16& port, const QString& host);
	void setRpcConfig(const quint16& port, const QString& host);
	void setupController();
	void addControlPanelWidget(ControlPanelWidget* cpWidget);
	void start();
	void setDbManager(IDbManager* dbManager);
	void removeDbManager();
	void setDbStationController(DBStationController* controller);
	//void setFlakonRpc(QString rpcHost, uint rpcPort);

	virtual QString getStationName(const int id);
	virtual void setActiveTab(const int id);

	virtual void onGlobalAutoSearchEnabled(const bool isEnabled);
	virtual void onGlobalPanoramaEnabled(const bool isEnabled);

	QStringList createStationNamesList();

	void setStationsConfiguration(const QList<StationConfiguration>& stationList);

	QMap<int, Station *>& getStations();

	void addStationTabs(unsigned int zone, unsigned int typeRds);
	void addZoneType(unsigned int zone, unsigned int typeRds);

	void clearAllInformation();
	void onClose();

	void setControlPanelController(ICorrelationListener* controller);
	void setControlPanelController(ControlPanelController *controller);

	void setResponseCommonFreq(quint32 freq);private:
	int readStationSettings(const QString &settingsFile);
	void readRpcSettings();

private slots:
	void changeTabSlot(int index);
	void slotUpdateDBStationsLists();
	void slotSendSolverSetupCommand(QByteArray);
	void slotSendRdsData(QByteArray data);
	void rpcConnectionEstablished();
	void rpcConnectionEstablishedFlakon();
	void slotShowError(QString);
	void slotShowConfirm(QString);

	void slotMethodCalled(const QString& method, const QVariant& argument);

	void slotOnChangeWorkMode(int mode, bool isOn);
	void slotOnChangeAnalysisTab(int channel);

public slots:
	void slotShowLocationSetup();
    void setListDialog(ListsDialog* dlg);

signals:
	void readyToStart(int);
	void signalMessageConfirm(QString);
	void signalMessageError(QString);

	void signalMethodCalled(const QString& method, const QVariant& argument);


public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	QTabWidget *getTabWidgetZone();

    int getTabId();
protected slots:
    void slotShowLists(QString station, double freq, double bandwidth);
};

#endif // TABMANAGER_H
