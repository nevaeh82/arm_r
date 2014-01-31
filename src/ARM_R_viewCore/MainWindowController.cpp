#include "MainWindowController.h"

#define SERVER_NAME "ARM_R_Server"

MainWindowController::MainWindowController(QObject *parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(MainWindowController));


	m_view = NULL;
}

MainWindowController::~MainWindowController()
{
	stop();
}

void MainWindowController::appendView(MainWindow *view)
{
	m_view = view;

	init();
}

void MainWindowController::startServer()
{
	start();
}

void MainWindowController::start()
{
	QString serverName = "./" + QString(SERVER_NAME);

#ifdef QT_DEBUG
	serverName += "d";
#endif

	m_serverHandler = new Pw::Common::ServiceControl::ServiceHandler(serverName, QStringList(), NULL, this);
	connect(m_serverHandler, SIGNAL(processStartedSignal()), this, SLOT(serverStartedSlot()));
	connect(m_serverHandler, SIGNAL(processStartFailedSignal()), this, SLOT(serverFailedToStartSlot()));

	m_serverHandler->start();
}

void MainWindowController::stop()
{
	//m_serverHandler->terminate();
	m_serverHandler->kill();
}

void MainWindowController::init()
{

}

void MainWindowController::serverFailedToStartSlot()
{

	QMessageBox::critical(m_view, tr("Unable to connect to server"), tr("Please try to restart application"));
}

void MainWindowController::serverStartedSlot()
{
	//startRpc();
}

