#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#define SERVER_NAME "ARM_R_Server"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(MainWindow));

	m_logger->info("Started");

	this->setWindowTitle(tr("Zaviruha"));

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


void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionLists, SIGNAL(triggered()), this, SIGNAL(signalShowLists()));
	//connect(ui->actionAtlantSetup, SIGNAL(triggered()), _diag_txt, SLOT(slotShow()));

}
