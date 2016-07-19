#include "MainWindowController.h"

#include "RdsPacket.pb.h"

#define SERVER_NAME "ARM_R_Server"

#define DEFAULT_RPC_PORT		24500


MainWindowController::MainWindowController(QObject *parent)
	: QObject(parent)
	, m_dbManager(0)
	, m_dbStationController(0)
	, m_rpcPort(24500)
	, m_rpcHost("127.0.0.1")
	, m_serverHandler(0)
{
	m_view = NULL;
	m_tabManager = NULL;
	m_controlPanelController = NULL;
	m_rpcConfigClient = NULL;
	m_rpcSettingsManager = NULL;
	m_solverWidgetController = NULL;
	m_solverSetupWidgetController = NULL;
	m_solverErrorsWidgetController = NULL;

	QString rpcSettingsFile = QCoreApplication::applicationDirPath();
	rpcSettingsFile.append("./Tabs/RPC.ini");
	QSettings rpcSettings( rpcSettingsFile, QSettings::IniFormat );
	rpcSettings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	m_rpcHost = rpcSettings.value("RPC_UI/IP", "192.168.0.1").toString();
	m_rpcPort = rpcSettings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toUInt();

	QString serverName = "./" + QString(SERVER_NAME);
	#ifdef QT_DEBUG
		serverName += "d";
	#endif
	m_serverHandler = new SkyHobbit::Common::ServiceControl::ServiceHandler(serverName, QStringList(), NULL, this);

	m_rpcFlakonClient = new RpcFlakonClientWrapper;
    m_rpcClientThread = new QThread;
    connect(m_rpcFlakonClient, SIGNAL(destroyed()), m_rpcClientThread, SLOT(terminate()));
    connect(m_rpcClientThread, SIGNAL(finished()), m_rpcFlakonClient, SLOT(deleteLater()), Qt::DirectConnection);
    connect(m_rpcClientThread, SIGNAL(destroyed()), m_rpcFlakonClient, SLOT(deleteLater()));
    //connect(this, SIGNAL(signalFinishRPC()), m_rpcClientThread, SLOT(quit()));
	//connect(this, SIGNAL(signalFinishRPC()), m_rpcClient, SLOT(deleteLater()));
	//connect(this, SIGNAL(signalFinishRPC()), rpcClientThread, SLOT(deleteLater()));

    m_rpcFlakonClient->moveToThread(m_rpcClientThread);
    m_rpcClientThread->start();
}

MainWindowController::~MainWindowController()
{
	SolverResultWidget* resultWidget = m_solverWidgetController->getView();
	if( resultWidget ) {
		m_solverWidgetController->appendView(NULL);
		resultWidget->close();
		delete resultWidget;
	}

	SolverSetupWidget* setupWidget = m_solverSetupWidgetController->getView();
	if( setupWidget ) {
		m_solverSetupWidgetController->appendView(NULL);
		setupWidget->close();
		delete setupWidget;
	}

	SolverErrorsWidget* errorsWidget = m_solverErrorsWidgetController->getView();
	if( errorsWidget ) {
		m_solverErrorsWidgetController->appendView(NULL);
		errorsWidget->close();
		delete errorsWidget;
	}

	m_tabManager->onClose();
	m_tabManager->clearAllInformation();
    m_rpcClientThread->exit();
    m_rpcClientThread->deleteLater();
}

void MainWindowController::appendView(MainWindow *view)
{
	m_view = view;

	init();
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

	m_controlPanelController = new ControlPanelController(this);
	m_controlPanelController->appendView(m_view->getControlPanelWidget());
	m_controlPanelController->setDbStationController(m_dbStationController);
	m_controlPanelController->setRpcFlakonClient(m_rpcFlakonClient);
	m_controlPanelController->registerReceiver(m_tabManager);
	m_tabManager->setControlPanelController((ICorrelationListener* )m_controlPanelController);

	m_dbManager = new DbManager(this);
	m_tabManager->setDbManager(m_dbManager);
	m_controlPanelController->setDbManager(m_dbManager);
	m_tabManager->setControlPanelController(m_controlPanelController);

	m_view->getStackedWidget()->setCurrentIndex(1);

	/// Problem here:

	connect(m_view, SIGNAL(signalShowLists()), this, SLOT(slotShowLists()));

	///

	SolverResultWidget* solverWidget = new SolverResultWidget(m_view);
	m_solverWidgetController = new SolverResultWidgetController(this);
	m_solverWidgetController->appendView(solverWidget);

	SolverSetupWidget* solverSetupWidget = new SolverSetupWidget(m_view);
	m_solverSetupWidgetController = new SolverSetupWidgetController(this);
	m_solverSetupWidgetController->appendView(solverSetupWidget);

	m_locationSetupController = new LocationSetupWidgetController(this);
	LocationSetupWidget* locationSetupWgt = new LocationSetupWidget(m_view);
	m_locationSetupController->appendView(locationSetupWgt);

	SolverErrorsWidget* solverErrorsWidget = new SolverErrorsWidget(m_view);
	m_solverErrorsWidgetController = new SolverErrorsWidgetController(this);
	m_solverErrorsWidgetController->appendView(solverErrorsWidget);

	connect(m_view, SIGNAL(signalShowSolverLog()), this, SLOT(slotShowSolverLog()));
	connect(m_view, SIGNAL(signalShowSolverSetup()), this, SLOT(slotShowSolverSetup()));
	connect(m_view, SIGNAL(signalShowSolverErrors()), this, SLOT(slotShowSolverErrors()));
	connect(m_view, SIGNAL(signalResetSerevr()), this, SLOT(resetServer()));

	connect(m_view, SIGNAL(signalShowLocationSetup()), this, SLOT(slotShowLocationSetup()));

	connect( m_solverSetupWidgetController, SIGNAL(onSendSolverCommandSettings(QByteArray)), this, SLOT( slotSendSolverSetupCommand(QByteArray)) );
	connect( m_locationSetupController, SIGNAL(sendRdsData(QByteArray)), this, SLOT( slotSendRdsData(QByteArray)) );

	serverStartedSlot();
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
	ListsDialogController* listController = new ListsDialogController(m_dbStationController, this);
	bool isOpen = m_dbStationController->getDataBase().isOpen();

	connect(listForm, SIGNAL(onClosing()), m_tabManager, SLOT(slotUpdateDBStationsLists()));

	m_dbStationController->registerReceiver( listController );

	if(!isOpen) {
		QMessageBox msgBox;
		msgBox.setText(tr("DataBase is not opened!"));
		msgBox.exec();
		return;
	}

	listController->appendView(listForm);
	listForm->show();
}

void MainWindowController::slotShowSolverLog()
{
	m_solverWidgetController->slotShowWidget();
}

void MainWindowController::slotShowSolverSetup()
{
	m_solverSetupWidgetController->slotShowWidget();
}

void MainWindowController::slotShowSolverErrors()
{
	m_solverErrorsWidgetController->slotShowWidget();
}

void MainWindowController::slotShowLocationSetup()
{
	m_locationSetupController->slotShowWidget();
}

void MainWindowController::slotSendSolverSetupCommand(QByteArray data)
{
	m_rpcFlakonClient->sendSolverSetupSettings( data );
}

void MainWindowController::slotSendRdsData(QByteArray data)
{
	m_rpcFlakonClient->sendRdsProto(data);
}

void MainWindowController::rpcConnectionEstablished()
{
	m_rpcConfigClient->requestGetDbConfiguration("./Tabs/Db.ini");
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");
	m_rpcConfigClient->requestGetAtlantConfiguration("./Tabs/RPC.ini");
}

void MainWindowController::startTabManger()
{
	m_rpcFlakonClient->deregisterReceiver(m_solverWidgetController);
	m_rpcFlakonClient->registerReceiver(m_solverWidgetController);

	m_rpcFlakonClient->deregisterReceiver(m_solverSetupWidgetController);
	m_rpcFlakonClient->registerReceiver(m_solverSetupWidgetController);

	m_rpcFlakonClient->deregisterReceiver(m_solverErrorsWidgetController);
	m_rpcFlakonClient->registerReceiver(m_solverErrorsWidgetController);

	m_view->getStackedWidget()->setCurrentIndex(0);
	m_tabManager->start();
}

void MainWindowController::resetServer()
{
	/// todo: Is that logic need in future?
//	QStringList serverPIDList;
//	bool searchResult;
//	searchResult = m_serverHandler->isProcessExist( m_serverHandler->getServicePath(), serverPIDList );

//	if( searchResult ) {
//		m_serverHandler->killProcessExist( serverPIDList );
//	}

	QStringList param;
	param.append("/F");
	param.append("/IM");
	param.append("ZaviruhaRServer.exe");
	QProcess::execute("taskkill", param);
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
		m_controlPanelController->setMapStations(m_tabManager->getStations());

	} else if(method == RPC_METHOD_CONFIG_RDS_ANSWER) {
		//data
		readProto( data );
	}
	else if (method == RPC_METHOD_CONFIG_ANSWER_ATLANT_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		AtlantConfiguration atlantConfiguration;
		dataStream >> atlantConfiguration;

		m_tabManager->setAtlantConfiguration(atlantConfiguration);
	} else if (method == RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		DBConnectionStruct dbConfig;
		dataStream >> dbConfig;

		m_dbStationController->connectToDB(dbConfig);
	}
}

void MainWindowController::readProto(const QByteArray& data)
{
	RdsProtobuf::Packet pkt;
	pkt.ParseFromArray( data.data(), data.size() );

	if( !pkt.has_from_server() ) {
		return;
	}

	if( pkt.from_server().has_current() ) {
		if( pkt.from_server().current().has_location() ) {
			m_locationSetupController->setLocationSetup( pkt.from_server().current().location() );
		}

		if( pkt.from_server().current().has_detector() ) {
			m_locationSetupController->setDetectorSetup( pkt.from_server().current().detector() );
		}

		if( pkt.from_server().current().has_correction() ) {
			m_locationSetupController->setCorrectionSetup( pkt.from_server().current().correction() );
		}
	}
}
