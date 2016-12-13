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

	log_debug(QString("id-%1").arg((int)this->thread()->currentThreadId()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

QTabWidget *MainWindow::getWorkTabsWidget() const
{
	return ui->worlTabsWidget;
}

QStackedWidget* MainWindow::getStackedWidget() const
{
	return ui->stackedWidget;
}

QAction* MainWindow::appendLocationSetupAction(int id)
{
	if( !ui->actionLocationSetup->menu() ) {
		ui->actionLocationSetup->setMenu( new QMenu(this) );
	}
	QAction* action = ui->actionLocationSetup->menu()->addAction(QString("Location settings %1").arg(id));
	m_actionMap.insert(id, action);

	return action;
}

void MainWindow::removeSetupAction(int id)
{
	ui->actionLocationSetup->menu()->removeAction( m_actionMap.take(id) );
}

void MainWindow::init()
{
	//connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionLists, SIGNAL(triggered()), this, SIGNAL(signalShowLists()));
	connect(ui->actionSolverLogs, SIGNAL(triggered()), this, SIGNAL(signalShowSolverLog()));
	connect(ui->actionSolverErrors, SIGNAL(triggered()), this, SIGNAL(signalShowSolverErrors()));
	connect(ui->actionSolverSetup, SIGNAL(triggered()), this, SIGNAL(signalShowSolverSetup()));

	//connect(ui->actionLocationSetup, SIGNAL(triggered()), this, SIGNAL(signalShowLocationSetup()));

	connect(ui->actionRestartServer, SIGNAL(triggered()), this, SIGNAL(signalResetSerevr()));

	connect(ui->actionConnections, SIGNAL(triggered()), this, SIGNAL(signalServerConnections()));

}

void MainWindow::showError(QString msg)
{
   // QMessageBox::critical(this, "Error", msg, QMessageBox::Ok);
}

void MainWindow::showConfirm(QString msg)
{
    //QMessageBox::information(this, "Confirm", msg, QMessageBox::Ok);
}
