#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>

#include <SettingsManager/RpcSettingsManager.h>
#include <ServiceHandler/ServiceHandler.h>

#include "Interfaces/IController.h"
#include "MainWindow.h"

#include "Tabs/TabManager.h"


#include "DBStation/DBStationController.h"
#include "DBStation/ListsDialog.h"
#include "DBStation/ListsDialogController.h"

//#include "Tabs/RPC/RpcConfigClient.h"

#include <Sleeper.h>

#include "Tabs/SolverResultWidgetController.h"
#include "Tabs/SolverErrorsWidgetController.h"
#include "Tabs/SolverSetupWidgetController.h"

#include "Tabs/ServerConnectionWidgetController.h"

#include "Tabs/RecordSignalSettings.h"

class MainWindowController : public QObject, public IController<MainWindow>//, public IRpcListener
{
	Q_OBJECT

private:
	MainWindow* m_view;

	QList<SkyHobbit::Common::ServiceControl::ServiceHandler*> m_serverHandlerList;
	//TabManager*				m_tabManager;
	IDbManager*				m_dbManager;
	//ControlPanelController* m_controlPanelController;
	DBStationController*	m_dbStationController;

	SolverResultWidgetController* m_solverWidgetController;
	SolverErrorsWidgetController* m_solverErrorsWidgetController;
	SolverSetupWidgetController* m_solverSetupWidgetController;


	ServerConnectionWidgetController* m_serverConnectionController;

	//QThread *m_rpcClientThread;

	QMap<int, TabManager*> m_mapTabManager;
	ListsDialog* m_listForm;

	void init();
	void connectToServers();

	ListsDialogController* m_listController;

	RecordSignalSettings* m_recordSignalSettingsDialog;

public:
	explicit MainWindowController(QObject *parent = 0);
	virtual ~MainWindowController();

	void appendView(MainWindow *view);

signals:
	void signalMessageConfirm(QString mesage);
	void signalMessageError(QString message);
	void signalAddedNewConnectionFromFile(int id, QString Ip, quint16 port);

private slots:
	void serverFailedToStartSlot();
	void serverStartedSlot();
	void slotShowLists();
	void slotShowSolverLog();
	void slotShowSolverSetup();
	void slotServerConnections();

	//	void rpcConnectionEstablished();
	void startTabManger(int id);
	void resetServer();

	void slotShowSolverErrors();

	void addedNewConnectionSlot(int id, QString Ip, quint16 port);
	void removeConnectionSlot(int id);

	void slotLocationSetup(int id);

	void onSetupLocationSettings();

	void slotNIIPPStatus(QString, bool);

};

#endif // MAINWINDOWCONTROLLER_H
