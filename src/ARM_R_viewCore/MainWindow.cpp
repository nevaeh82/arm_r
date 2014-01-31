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

    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaTypeStreamOperators<QByteArray>("QByteArray");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaTypeStreamOperators<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

	this->setWindowTitle(tr("Zaviruha"));


	/// TODO: fix
	//QString tabs_setting_file = QCoreApplication::applicationDirPath();
	//tabs_setting_file.append("/Tabs/Tabs.ini");
	//int count_tabs = _tab_manager->createSubModules(tabs_setting_file);

    this->showMaximized();

	/// TODO: fix
	//_tab_manager->start();

	init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	//connect(ui->actionAtlantSetup, SIGNAL(triggered()), _diag_txt, SLOT(slotShow()));

}
