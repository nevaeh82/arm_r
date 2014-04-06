#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include <QMessageBox>

#include "Interfaces/IController.h"
#include "MainWindow.h"

#include "ServiceHandler/ServiceHandler.h"

#include "Tabs/TabManager.h"
#include "ControlPanel/ControlPanelController.h"

#include "DBStation/DBStationController.h"
#include "DBStation/ListsDialog.h"
#include "DBStation/ListsDialogController.h"

#include "Tabs/RPC/RpcConfigClient.h"
#include "Rpc/RpcSettingsManager.h"

#include <Sleeper.h>

class MainWindowController : public QObject, public IController<MainWindow>, public IRpcListener
{
	Q_OBJECT

private:
	MainWindow* m_view;

	Pw::Common::ServiceControl::ServiceHandler* m_serverHandler;

	TabManager*				m_tabManager;
	IDbManager*				m_dbManager;
	ControlPanelController* m_controlPanelController;
	DBStationController*	m_dbStationController;

	RpcConfigClient*		m_rpcConfigClient;
	IRpcSettingsManager*	m_rpcSettingsManager;

public:
	explicit MainWindowController(QObject *parent = 0);
	virtual ~MainWindowController();

	void appendView(MainWindow *view);

	void startServer();

signals:

private slots:
	void serverFailedToStartSlot();
	void serverStartedSlot();
	void slotShowLists();

	void rpcConnectionEstablished();
	void startTabManger();

private:

	void start();
	void stop();

	void init();

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
};

#endif // MAINWINDOWCONTROLLER_H
