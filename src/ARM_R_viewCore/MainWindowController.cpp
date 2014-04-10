#include "MainWindowController.h"

#define SERVER_NAME "ARM_R_Server"

#define DEFAULT_RPC_PORT		24500


MainWindowController::MainWindowController(QObject *parent) 
	: QObject(parent)
	, m_serverHandler(0)
	, m_dbManager(0)
	, m_dbStationController(0)
	, m_rpcPort(24500)
	, m_rpcHost("127.0.0.1")
{
	m_view = NULL;
	m_tabManager = NULL;
	m_controlPanelController = NULL;
	m_rpcConfigClient = NULL;
	m_rpcSettingsManager = NULL;

	QString rpcSettingsFile = QCoreApplication::applicationDirPath();
	rpcSettingsFile.append("./Tabs/RPC.ini");
	QSettings rpcSettings( rpcSettingsFile, QSettings::IniFormat );
	rpcSettings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	m_rpcHost = rpcSettings.value("RPC_UI/IP", "192.168.0.1").toString();
	m_rpcPort = rpcSettings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toUInt();


	m_rpcFlakonClient = new RpcFlakonClient(this);
	m_rpcFlakonClient->start( m_rpcPort, QHostAddress( m_rpcHost ) );
}

MainWindowController::~MainWindowController()
{
	stop();
	m_tabManager->clearAllInformation();
}

void MainWindowController::appendView(MainWindow *view)
{
	m_view = view;

	init();
}

void MainWindowController::startServer()
{
//	serverStartedSlot();
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
	m_dbStationController = new DBStationController(this);

	m_rpcSettingsManager = RpcSettingsManager::instance();

	m_rpcConfigClient = new RpcConfigClient(this);
	m_rpcConfigClient->registerReceiver(this);

	m_tabManager = new TabManager(m_view->getWorkTabsWidget(), this);
	connect(m_tabManager, SIGNAL(readyToStart()), this, SLOT(startTabManger()));
	m_tabManager->setDbStationController(m_dbStationController);

	m_tabManager->setFlakonRpc(m_rpcFlakonClient, m_rpcHost, m_rpcPort);

	//m_tabManager->createSubModules(tabsSettingsFile);


	m_controlPanelController = new ControlPanelController(this);
	m_controlPanelController->appendView(m_view->getControlPanelWidget());
	m_controlPanelController->setDbStationController(m_dbStationController);
	m_controlPanelController->setRpcFlakonClient(m_rpcFlakonClient);
	m_controlPanelController->setMapStations(m_tabManager->getStations());
	m_controlPanelController->registerReceiver(m_tabManager);

	m_dbManager = new DbManager(this);
	m_tabManager->setDbManager(m_dbManager);
	m_controlPanelController->setDbManager(m_dbManager);

	m_view->getStackedWidget()->setCurrentIndex(1);

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
	m_view->getStackedWidget()->setCurrentIndex(1);

	log_debug("go to sleep");
	QEventLoop loop;
	QTimer timer;
	connect(&timer, SIGNAL(timeout()),&loop, SLOT(quit()));
	timer.start(1000);
	loop.exec();
	timer.stop();
	log_debug("Sleeper is off");

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
	m_rpcConfigClient->requestGetDbConfiguration("./Tabs/Db.ini");
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");
	m_rpcConfigClient->requestGetAtlantConfiguration("./Tabs/RPC.ini");
}

void MainWindowController::startTabManger()
{
	m_view->getStackedWidget()->setCurrentIndex(0);
	m_tabManager->start();
}

void MainWindowController::onMethodCalled(const QString& method, const QVariant& argument)
{
	QByteArray data = argument.toByteArray();
	if (method == RPC_METHOD_CONFIG_ANSWER_STATION_LIST) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		QList<StationConfiguration> stationList;
		dataStream >> stationList;

		m_tabManager->clearAllInformation();
		m_tabManager->setStationsConfiguration(stationList);
		m_tabManager->addStationTabs();

	} else if (method == RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		AtlantConfiguration atlantConfiguration;
		dataStream >> atlantConfiguration;

		m_tabManager->setAtlantConfiguration(atlantConfiguration);
	} else if (method == RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		DBConnectionStruct dbConfig;;
		dataStream >> dbConfig;

		m_dbStationController->connectToDB(dbConfig);
	}
}
