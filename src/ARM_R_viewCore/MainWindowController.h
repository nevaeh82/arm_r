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

class MainWindowController : public QObject, public IController<MainWindow>
{
	Q_OBJECT

private:
	MainWindow* m_view;

	Pw::Common::ServiceControl::ServiceHandler* m_serverHandler;

	TabManager*				m_tabManager;
	IDbManager*				m_dbManager;
	ControlPanelController* m_controlPanelController;
	DBStationController*	m_dbStationController;

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

private:

	void start();
	void stop();

	void init();
};

#endif // MAINWINDOWCONTROLLER_H
