#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "ControlPanel/ControlPanelWidget.h"
#include "Tabs/TabManager.h"
#include "Tabs/ServerConnectionsWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	virtual ~MainWindow();

	QTabWidget* getWorkTabsWidget() const;
	QStackedWidget* getStackedWidget() const;

	QAction *appendLocationSetupAction(int id);
	void removeSetupAction(int id);

private:
	Ui::MainWindow* ui;
	QMap<int, QAction*> m_actionMap;

private:
	void init();
private slots:
	void showError(QString);
	void showConfirm(QString);

signals:
	void signalShowLists();
	void signalShowSolverLog();
	void signalShowSolverSetup();
	void signalShowSolverErrors();
	void signalResetSerevr();
    void signalServerConnections();

	void signalShowLocationSetup(int id);
};


#endif // MAINWINDOW_H
