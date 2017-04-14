#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ComReceiver/ComReceiverController.h"
#include "ComReceiver/ComReceiverView.h"


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

	ui->niippLabel->setVisible(false);
	m_niippTimer = new QTimer(this);

	connect(m_niippTimer, SIGNAL(timeout()), this, SLOT(slotHideNiippLabel()));
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

	QAction* action = ui->actionLocationSetup->menu()->addAction(tr("Location settings System #%1").arg(id));
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

	ComReceiverController* controller = new ComReceiverController(this);
	connect(ui->actionPrm_via_com, SIGNAL(triggered(bool)), controller->getView(), SLOT(show()));
}

void MainWindow::updateLocationSetupAction(int id, QString text)
{
	if( !ui->actionLocationSetup->menu() ) {
		return;
	}

	QAction* act = m_actionMap.value(id, NULL);

	if(act) {
		act->setText( tr("Location settings System #%1 %2").arg(id).arg(text) );
	}
}

void MainWindow::slotHideNiippLabel()
{
	ui->niippLabel->setVisible(false);
	m_niippTimer->stop();
}

void MainWindow::closeEvent(QCloseEvent *)
{
	emit onCloseSignal();
}

void MainWindow::slotNIIPPStatus(QString title, bool status)
{
	if(status) {
		ui->niippLabel->setText(tr("Punkt ") + "\"" + title + "\" " + tr(" flushing") );
		ui->niippLabel->setVisible(true);

		m_niippTimer->start(30000);
	} else {
		ui->niippLabel->setVisible(false);
	}
}
