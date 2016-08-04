#include "MainWindowController.h"

#include "RdsPacket.pb.h"

#define SERVER_NAME "ARM_R_Server"

#define DEFAULT_RPC_PORT		24500


MainWindowController::MainWindowController(QObject *parent)
	: QObject(parent)
	, m_dbManager(0)
	, m_dbStationController(0)
//	, m_rpcPort(24500)
//	, m_rpcHost("127.0.0.1")
	, m_serverHandler(0)
{
	m_view = NULL;
//	m_tabManager = NULL;
//	m_controlPanelController = NULL;

	m_solverWidgetController = NULL;
	m_solverSetupWidgetController = NULL;
	m_solverErrorsWidgetController = NULL;


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

    foreach( int key, m_mapTabManager.keys() )
    {
        TabManager *tab = m_mapTabManager.value(key);
        tab->onClose();
        tab->clearAllInformation();
    }
    m_mapTabManager.clear();
}

void MainWindowController::appendView(MainWindow *view)
{
	m_view = view;

    init();

    m_listForm = new ListsDialog(m_view);

    connectToServers();
}

void MainWindowController::connectToServers()
{

    QString settingsFile = QCoreApplication::applicationDirPath();
    settingsFile.append("/TCP/servers.ini");
    QSettings settings(settingsFile, QSettings::IniFormat, this);

    QStringList childKeys = settings.childGroups();

    foreach (const QString &childKey, childKeys)
    {
        settings.beginGroup(childKey);

        int id = settings.value("id", 0).toInt();
        QString ip = settings.value("ip", "127.0.0.1").toString();
        quint16 port = settings.value("port", 0).toInt();

        emit signalAddedNewConnectionFromFile(id, ip, port);

        settings.endGroup();
    }
    m_serverConnectionController->init();
}

void MainWindowController::init()
{

    m_dbStationController = new DBStationController(this);

    m_dbManager = new DbManager(this);


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

//	m_locationSetupController = new LocationSetupWidgetController(this);
//	LocationSetupWidget* locationSetupWgt = new LocationSetupWidget(m_view);
//	m_locationSetupController->appendView(locationSetupWgt);

	SolverErrorsWidget* solverErrorsWidget = new SolverErrorsWidget(m_view);
	m_solverErrorsWidgetController = new SolverErrorsWidgetController(this);
	m_solverErrorsWidgetController->appendView(solverErrorsWidget);

    ServerConnectionsWidget* serverConnectionsWidget = new ServerConnectionsWidget(m_view);
    m_serverConnectionController = new ServerConnectionWidgetController(this);
    m_serverConnectionController->appendView(serverConnectionsWidget);
    connect(m_serverConnectionController, SIGNAL(signalAddedNewConnection(int,QString,quint16)), this, SLOT(addedNewConnectionSlot(int, QString, quint16)));
    connect(m_serverConnectionController, SIGNAL(signalRemoveConnection(int)), this, SLOT(removeConnectionSlot(int)));
    connect(this, SIGNAL(signalAddedNewConnectionFromFile(int,QString,quint16)), m_serverConnectionController, SLOT(addedNewConnectionExtSlot(int,QString,quint16)));


	connect(m_view, SIGNAL(signalShowSolverLog()), this, SLOT(slotShowSolverLog()));
	connect(m_view, SIGNAL(signalShowSolverSetup()), this, SLOT(slotShowSolverSetup()));
	connect(m_view, SIGNAL(signalShowSolverErrors()), this, SLOT(slotShowSolverErrors()));
	connect(m_view, SIGNAL(signalResetSerevr()), this, SLOT(resetServer()));
    connect(m_view, SIGNAL(signalServerConnections()), this, SLOT(slotServerConnections()));

    //сделать динамически добавляемую менюшку. Для каждого сервера отдельный вызов
//    connect( m_locationSetupController, SIGNAL(sendRdsData(QByteArray)), this, SLOT( slotSendRdsData(QByteArray)) );

//    connect(m_controlPanelController, SIGNAL(signalSetComonFreq(int)),
//            m_locationSetupController, SLOT(slotOnSetCommonFreq(int)));

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

//	m_rpcSettingsManager->setIniFile("./Tabs/RPC.ini");
//	QString host = m_rpcSettingsManager->getRpcHost();
//	quint16 port = m_rpcSettingsManager->getRpcPort().toUShort();

//	m_tabManager->setRpcConfig(port, host);

}

void MainWindowController::slotShowLists()
{	
	ListsDialogController* listController = new ListsDialogController(m_dbStationController, this);
	bool isOpen = m_dbStationController->getDataBase().isOpen();

	m_dbStationController->registerReceiver( listController );

	if(!isOpen) {
		QMessageBox msgBox;
		msgBox.setText(tr("DataBase is not opened!"));
		msgBox.exec();
		return;
	}

    listController->appendView(m_listForm);
    m_listForm->show();
}

void MainWindowController::slotShowSolverLog()
{
	m_solverWidgetController->slotShowWidget();
}

void MainWindowController::slotShowSolverSetup()
{
    m_solverSetupWidgetController->slotShowWidget();
}

void MainWindowController::slotServerConnections()
{
    m_serverConnectionController->slotShowWidget();
}

void MainWindowController::slotShowSolverErrors()
{
	m_solverErrorsWidgetController->slotShowWidget();
}

void MainWindowController::addedNewConnectionSlot(int id, QString Ip, quint16 port)
{
//    QString rpcSettingsFile = QCoreApplication::applicationDirPath();
//    rpcSettingsFile.append("./Tabs/RPC.ini");
//    QSettings rpcSettings( rpcSettingsFile, QSettings::IniFormat );
//    rpcSettings.setIniCodec( QTextCodec::codecForName("UTF-8") );

//    m_rpcHost = rpcSettings.value("RPC_UI/IP", "127.0.0.1").toString();
//    m_rpcPort = rpcSettings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toUInt();

//    QString serverName = "./" + QString(SERVER_NAME);
//    #ifdef QT_DEBUG
//        serverName += "d";
//    #endif
//    m_serverHandler = new SkyHobbit::Common::ServiceControl::ServiceHandler(serverName, QStringList(), NULL, this);


    log_debug("addedNewConnectionSlot");
    TabManager* tabManager = new TabManager(id, m_view->getWorkTabsWidget(), this);
    tabManager->setDbStationController(m_dbStationController);
    tabManager->setDbManager(m_dbManager);
    tabManager->addControlPanelWidget(m_view->getControlPanelWidget());

    tabManager->initRpcConfig();
    tabManager->setcpController();
    tabManager->setRpcFlakon(port, Ip);


    connect(tabManager, SIGNAL(readyToStart(int)), this, SLOT(startTabManger(int)));
    connect(m_listForm, SIGNAL(onClosing()), tabManager, SLOT(slotUpdateDBStationsLists()));
    connect(m_view, SIGNAL(signalShowLocationSetup()), tabManager, SLOT(slotShowLocationSetup()));

    //tabManager->setFlakonRpc(ip, port);
    tabManager->setRpcConfig(port, Ip);
    m_mapTabManager.insert(id, tabManager);
}

void MainWindowController::removeConnectionSlot(int id)
{
    TabManager* tabManager = m_mapTabManager.take(id);
    if(tabManager == NULL) {
        return;
    }

    tabManager->clearAllInformation();

    int tabID = tabManager->getTabId();
    if(tabID != -1)
    {
        m_view->getWorkTabsWidget()->removeTab(tabManager->getTabId());
        int count = m_view->getWorkTabsWidget()->count();

        if(count < 1)
        {
    //        m_view->getWorkTabsWidget()->close();
            m_view->getStackedWidget()->setCurrentIndex(1);
        }
    }

    delete tabManager;
}

void MainWindowController::startTabManger(int id)
{
    log_debug("startTabManger");
    m_mapTabManager.value(id)->startTab(m_solverWidgetController, m_solverErrorsWidgetController, m_solverSetupWidgetController);
    m_view->getStackedWidget()->setCurrentIndex(0);
    m_mapTabManager.value(id)->start();
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
