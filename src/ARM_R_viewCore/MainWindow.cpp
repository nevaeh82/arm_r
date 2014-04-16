#include "MainWindow.h"
#include "ui_MainWindow.h"

#define SERVER_NAME "ARM_R_Server"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	log_info("Started");

	this->setWindowTitle(tr("Zaviruha"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));

	this->showMaximized();

	init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

ControlPanelWidget *MainWindow::getControlPanelWidget() const
{
	return ui->controlWidget;
}

QTabWidget *MainWindow::getWorkTabsWidget() const
{
	return ui->worlTabsWidget;
}

QStackedWidget* MainWindow::getStackedWidget() const
{
	return ui->stackedWidget;
}

void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionLists, SIGNAL(triggered()), this, SIGNAL(signalShowLists()));
	//connect(ui->actionAtlantSetup, SIGNAL(triggered()), _diag_txt, SLOT(slotShow()));

}
