#include "MainWindowController.h"

#define SERVER_NAME "ARM_R_Server"

MainWindowController::MainWindowController(QObject *parent) 
	: QObject(parent)
	, m_serverHandler(0)
	, m_dbManager(0)
	, m_dbStationController(0)
{
	m_view = NULL;
	m_tabManager = NULL;
	m_controlPanelController = NULL;
	m_rpcConfigClient = NULL;
	m_rpcSettingsManager = NULL;
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
	m_dbManager = new DbManager(this);

	m_dbStationController = new DBStationController(this);

	m_rpcSettingsManager = RpcSettingsManager::instance();

	m_rpcConfigClient = new RpcConfigClient(this);


	m_tabManager = new TabManager(m_view->getWorkTabsWidget(), this);
	connect(m_tabManager, SIGNAL(readyToStart()), this, SLOT(startTabManger()));

	m_tabManager->setDbStationController(m_dbStationController);

	m_tabManager->setDbManager(m_dbManager);

	m_rpcConfigClient->registerReceiver(m_tabManager);


	m_controlPanelController = new ControlPanelController(this);
	m_controlPanelController->appendView(m_view->getControlPanelWidget());
	m_controlPanelController->setDbManager(m_dbManager);
	m_controlPanelController->registerReceiver(m_tabManager);

	/// Problem here:



	connect(m_view, SIGNAL(signalShowLists()), this, SLOT(slotShowLists()));

//	(m_view, SIGNAL(signalShowLists()), this, SLOT(slotShowLists()));
}

void MainWindowController::serverFailedToStartSlot()
{
	QMessageBox::critical(m_view, tr("Unable to connect to server"), tr("Please try to restart application"));
}

void MainWindowController::serverStartedSlot()
{
	// We need wait for a second after start server
	Sleeper::msleep(1000);

	DBConnectionStruct param;
	param.dbName = "Stations";
	param.host = "127.0.0.1";
	param.login = "root";
	param.port = 3306;
	param.password = "qwerty12345";
	m_dbStationController->connectToDB(param);


	m_rpcSettingsManager->setIniFile("./Tabs/RPC.ini");
	QString host = m_rpcSettingsManager->getRpcHost();
	quint16 port = m_rpcSettingsManager->getRpcPort().toUShort();

	m_tabManager->setRpcConfig(port, host);
	m_rpcConfigClient->start(port, QHostAddress(host));
	connect(m_rpcConfigClient, SIGNAL(connectionEstablishedSignal()), this, SLOT(rpcConnectionEstablished()));

}

void MainWindowController::slotShowLists()
{

	ListsDialog* listForm = new ListsDialog(m_view);
	ListsDialogController* listController = new ListsDialogController(m_dbStationController->getDataBase(), this);
	bool isOpen = m_dbStationController->getDataBase().isOpen();
	if(!isOpen)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("DataBase is not opened!"));
		msgBox.exec();
		return;
	}
	listController->appendView(listForm);
	listForm->show();
}

void MainWindowController::rpcConnectionEstablished()
{
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");
	m_rpcConfigClient->requestGetAtlantConfiguration("./Tabs/RPC.ini");
}

void MainWindowController::startTabManger()
{
	m_tabManager->start();
}

