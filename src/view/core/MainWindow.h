#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "ControlPanel/ControlPanelWidget.h"
#include "Tabs/TabManager.h"

#include "Charts/chart.h"
#include "Charts/chartview.h"

namespace Ui {
class MainWindow;
}

QT_CHARTS_USE_NAMESPACE

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
	//ChartView* getChartWidget();

private:
	void init();

signals:
	void signalShowLists();
	void signalShowSolverLog();
	void signalShowSolverErrors();
	void signalResetSerevr();

};


#endif // MAINWINDOW_H
