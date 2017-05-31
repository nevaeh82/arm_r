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

#include "DataSourceController.h"
#include "Correlations/CorrelationGroupWidget.h"
#include "Correlations/CorrelationGroupColumnWidget.h"

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
	QMap<int, IAnalysisWidget*> m_analysisControllerMap;

	IDbManager* m_dbManager;
	ITabWidget* m_currentTabWidget;

	QString m_mainTitle;

	QString m_rpcHost;
	uint m_rpcPort;

	RpcFlakonClientWrapper *m_rpcFlakonClient;

	ICorrelationControllersContainer* m_correlationControllers;
	ICorrelationControllersContainer* m_externalCorrelationControllers;

	DBStationController* m_dbStationController;
	ICorrelationListener* m_controlPanelController;
	ControlPanelController* m_panelController;
	int m_id;
	QThread *m_rpcClientThread;
	QThread *m_rpcClientConfigThread;
	ControlPanelWidget* m_cpWidget;

	LocationSetupWidgetController* m_locationSetupController;
	PanoramaFreqControl* m_panoramaFreqControl;
	SolverSetupWidgetController* m_solverSetup;

	RpcConfigClient* m_rpcConfigClient;

	QList<ITabWidget*> m_tabWidgetList;

	int m_tabId;
	int m_tabCount;

	QMap<int, QWidget*> m_tabWidgetContainer;

	ListsDialog* m_listForm;

	ControlPanelWidget* m_cpView;

	CorrelationGroupWidget* m_externalCorrelationWidget;

	DataSourceController* m_dsController;

	QMap<int, int> m_devChannelMap;

	// IRpcListener interface
	void readProto(const QByteArray &data);
	void initAnalysisControllers(int count);

	Station* getStationByDevChannel(int dev, int channel);

    bool m_isSystemMerge;

public:
	TabManager(int id, QTabWidget* tabWidget, QObject *parent = 0);
	virtual ~TabManager();
	void initRpcConfig();
	void startTab(SolverResultWidgetController* resultSolver, SolverErrorsWidgetController* errorSolver, SolverSetupWidgetController* setupSolver);

	void setNIIPPAlarmMode();

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
	virtual void onGlobalPanoramaEnabled(const bool isEnabled, int start, int end);

	QStringList createStationNamesList();

	void setStationsConfiguration(RdsProtobuf::System_SystemOptions msg);

	QMap<int, Station *>& getStations();

	void addStationTabs(QString platformName);
	void addZoneType(unsigned int zone, unsigned int typeRds);

	void clearAllInformation();
	void onClose();

	void setControlPanelController(ICorrelationListener* controller);
	void setControlPanelController(ControlPanelController *controller);

	void setResponseCommonFreq(quint32 freq);
	int readStationSettings(const QString &settingsFile);
	void readRpcSettings();

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	QTabWidget *getTabWidgetZone();

    bool isSystemMerge() const {return m_isSystemMerge;}

private slots:
	void changeTabSlot(int index);
	void slotUpdateDBStationsLists();
	void slotSendSolverSetupCommand(QByteArray);
	void slotSendRdsData(QByteArray data);
    void slotSendCPPacketData(QByteArray data);
	void rpcConnectionEstablished();
	void rpcConnectionDropped();
	void rpcConnectionEstablishedFlakon();
	void slotShowError(QString);
	void slotShowConfirm(QString);

	void slotMethodCalled(const QString& method, const QVariant& argument);

	void slotExpandCorrelations();

	void slotAnalysisAddToList(double start, double end);
	void slotAnalysisContinue(int id, bool);
	void slotAnalysisSelection(int ind, double start, double end);
	void slotAnalysisReady(int);

	void slotAnalysisClose();

	void onSystemMerge(bool);

	void changeCommonTabSlot(int index);
public slots:
	void slotShowLocationSetup();
	void setListDialog(ListsDialog* dlg);

signals:
	void readyToStart(int);
	void signalMessageConfirm(QString);
	void signalMessageError(QString);

	void signalMethodCalled(const QString& method, const QVariant& argument);

	void signalLocationError(QString);
	void signalLocationChanged();

	void onTitleUp(int, QString);

	void signalNIIPPWorkStatus(QString, bool);

protected slots:
	void slotShowLists(QString station, double freq, double bandwidth);

public:
	int getTabId();
	void setCurrentOptions(const RdsProtobuf::ClientMessage_OneShot_Location& msg,
						   const bool isPanorama,
						   const double& start, const double& end);

	RdsProtobuf::ClientMessage_OneShot_Location getCurrentOptions(bool &isPanorama, double &start, double &end);
};

#endif // TABMANAGER_H
