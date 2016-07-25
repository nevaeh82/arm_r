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
#include "ControlPanel/ControlPanelController.h"

#include "Tabs/Rpc/RpcFlakonClientWrapper.h"

#include "DBStation/DBStationController.h"
#include "DBStation/ListsDialog.h"
#include "DBStation/ListsDialogController.h"

#include "Tabs/RPC/RpcConfigClient.h"

#include <Sleeper.h>

#include "Tabs/SolverResultWidgetController.h"
#include "Tabs/SolverErrorsWidgetController.h"
#include "Tabs/SolverSetupWidgetController.h"
#include "Tabs/LocationSetupWidgetController.h"

class MainWindowController : public QObject, public IController<MainWindow>, public IRpcListener
{
	Q_OBJECT

private:
	MainWindow* m_view;

	SkyHobbit::Common::ServiceControl::ServiceHandler* m_serverHandler;
	TabManager*				m_tabManager;
	IDbManager*				m_dbManager;
	ControlPanelController* m_controlPanelController;
	DBStationController*	m_dbStationController;

	RpcConfigClient*		m_rpcConfigClient;
	IRpcSettingsManager*	m_rpcSettingsManager;

	RpcFlakonClientWrapper *m_rpcFlakonClient;
	QString m_rpcHost;
	uint m_rpcPort;

	SolverResultWidgetController* m_solverWidgetController;
	SolverErrorsWidgetController* m_solverErrorsWidgetController;
	SolverSetupWidgetController* m_solverSetupWidgetController;

	LocationSetupWidgetController* m_locationSetupController;

	QThread *m_rpcClientThread;

public:
	explicit MainWindowController(QObject *parent = 0);
	virtual ~MainWindowController();

	void appendView(MainWindow *view);

signals:
    void signalMessageConfirm(QString mesage);
    void signalMessageError(QString message);

private slots:
	void serverFailedToStartSlot();
	void serverStartedSlot();
	void slotShowLists();
	void slotShowSolverLog();
	void slotShowSolverSetup();

	void rpcConnectionEstablished();
	void startTabManger();
	void resetServer();

	void slotShowSolverErrors();

	void slotSendSolverSetupCommand( QByteArray data );
	void slotSendRdsData(QByteArray data);
	void slotShowLocationSetup();
private:

	void init();

	// IRpcListener interface
	void readProto(const QByteArray &data);
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
};

#endif // MAINWINDOWCONTROLLER_H
