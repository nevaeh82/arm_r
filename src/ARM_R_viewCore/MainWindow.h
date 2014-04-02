#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ControlPanel/ControlPanelWidget.h"
#include "Tabs/TabManager.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	ControlPanelWidget* getControlPanelWidget() const;
	QTabWidget* getWorkTabsWidget() const;
    
private:
	Ui::MainWindow*     ui;

private:
	void init();

signals:
	void signalShowLists();

};


#endif // MAINWINDOW_H
