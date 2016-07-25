#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "ControlPanel/ControlPanelWidget.h"
#include "Tabs/TabManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	virtual ~MainWindow();

	ControlPanelWidget* getControlPanelWidget() const;
	QTabWidget* getWorkTabsWidget() const;
	QStackedWidget* getStackedWidget() const;

private:
	Ui::MainWindow* ui;

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

	void signalShowLocationSetup();
};


#endif // MAINWINDOW_H
