#include "TabManager.h"

#include <QDebug>

#include <math.h>

#include "CommonSpectrumTabWidget.h"
#include "TabSpectrumWidgetController.h"

#define DEFAULT_RPC_PORT		24500



/////////////// C A L C U L A T E D E L A Y S C O U N T ///////////////


int factorial(int n){
	return ( n <= 1 ) ? 1 : factorial(n - 1) * n;
}

///////// C A L C U L A T E C O M B I N A T I O N S C O U N T /////////

int CalculateCombinationsCount( const int N_elems, const int M_comb ) {
	return factorial( N_elems ) / factorial( N_elems - M_comb )
			/ factorial( M_comb );
}

int CalculateDelaysCount( const int siz_det ) {
	return CalculateCombinationsCount( siz_det, 2);
}


TabManager::TabManager(int id, QTabWidget *tabWidget, QObject *parent)
	: QObject( parent )
	//    , m_tabWidget(tabWidget)
	, m_tabWidgetZone( tabWidget )
	, m_correlationControllers( NULL )
	, m_currentTabWidget( NULL )
	, m_dbManager( NULL )
	, m_dbStationController( NULL )
	, m_rpcHost( "127.0.0.1" )
	, m_rpcPort( 0 )
	, m_controlPanelController( NULL )
	, m_panelController(NULL)
	, m_rpcFlakonClient(NULL)
	, m_rpcConfigClient(NULL)
	, m_tabWidget(NULL)
	, m_id(id)
	, m_tabId(-1)
{
	m_tabWidget = new QTabWidget(tabWidget);

	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));


	m_locationSetupController = new LocationSetupWidgetController(this);
	LocationSetupWidget* locationSetupWgt = new LocationSetupWidget();
	m_locationSetupController->appendView(locationSetupWgt);

	m_panoramaFreqControl = new PanoramaFreqControl(this);

	connect( m_locationSetupController, SIGNAL(sendRdsData(QByteArray)), this, SLOT( slotSendRdsData(QByteArray)) );

	connect(this, SIGNAL(signalMessageError(QString)), this, SLOT(slotShowError(QString)));
	connect(this, SIGNAL(signalMessageConfirm(QString)), this, SLOT(slotShowConfirm(QString)));
	connect(this, SIGNAL(signalMethodCalled(QString,QVariant)), this, SLOT(slotMethodCalled(QString,QVariant)));
}

TabManager::~TabManager()
{
	m_rpcFlakonClient->stop();
	//  m_rpcConfigClient->stop();

	QMapIterator<QString, ITabWidget*> i(m_tabWidgetsMap);
	while (i.hasNext())
	{
		i.next();

		ITabWidget* st = i.value();
		delete st;
	}

	disconnect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
	m_rpcClientThread->exit();
	m_rpcClientThread->deleteLater();

	m_rpcClientConfigThread->exit();
	m_rpcClientConfigThread->deleteLater();

	m_locationSetupController->deleteLater();

	if(m_tabWidget != NULL)
	{
		delete m_tabWidget;
	}
}

void TabManager::slotSendRdsData(QByteArray data)
{
	m_rpcFlakonClient->sendRdsProto(data);
}

void TabManager::slotShowLocationSetup()
{
	m_locationSetupController->slotShowWidget();
}

void TabManager::startTab(SolverResultWidgetController *resultSolver, SolverErrorsWidgetController *errorSolver, SolverSetupWidgetController *setupSolver)
{
	m_rpcFlakonClient->deregisterReceiver(resultSolver);
	m_rpcFlakonClient->registerReceiver(resultSolver);

	m_rpcFlakonClient->deregisterReceiver(errorSolver);
	m_rpcFlakonClient->registerReceiver(errorSolver);

	m_rpcFlakonClient->deregisterReceiver(setupSolver);
	m_rpcFlakonClient->registerReceiver(setupSolver);

	connect( setupSolver, SIGNAL(onSendSolverCommandSettings(QByteArray)), this, SLOT( slotSendSolverSetupCommand(QByteArray)) );
}

void TabManager::setRpcFlakon(const quint16& port, const QString& host)
{

}

void TabManager::setRpcConfig(const quint16& port, const QString& host)
{

	m_rpcHost = host;
	m_rpcPort = port;

	m_rpcConfigClient->start(port, QHostAddress(host));


	//if (m_rpcFlakonClient != NULL) {
	// m_rpcFlakonClient->init(port, QHostAddress(host));
	// }
}

void TabManager::setupController()
{
	if( !m_panelController ) {
		ControlPanelController* controlPanelController = new ControlPanelController(this);
		controlPanelController->appendView(m_cpWidget);
		controlPanelController->setDbStationController(m_dbStationController);
		controlPanelController->setRpcFlakonClient(m_rpcFlakonClient);
		controlPanelController->registerReceiver(this);
		controlPanelController->setDbManager(m_dbManager);
		this->setControlPanelController((ICorrelationListener* )controlPanelController);
		setControlPanelController(controlPanelController);
	}

	connect(m_panelController, SIGNAL(signalSetComonFreq(int)),
			m_locationSetupController, SLOT(slotOnSetCommonFreq(int)));

	connect(m_panoramaFreqControl, SIGNAL(setCommonFreq(int)),
			m_locationSetupController, SLOT(slotOnSetCommonFreq(int)));
}

void TabManager::addControlPanelWidget(ControlPanelWidget *cpWidget)
{
	m_cpWidget = cpWidget;
}

void TabManager::start()
{
	changeTabSlot(m_tabWidget->currentIndex());
	m_tabWidget->setEnabled(true);
}

QStringList TabManager::createStationNamesList()
{
	QStringList stationNamesList;

	foreach (Station* station, m_stationsMap) {
		stationNamesList.append(station->getName());
	}
	stationNamesList.append(tr("Auto"));

	return stationNamesList;
}

QMap<int, Station *> &TabManager::getStations()
{
	return m_stationsMap;
}

void TabManager::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void TabManager::removeDbManager()
{
	m_dbManager = NULL;

	foreach (ITabWidget* wgt, m_tabWidgetList) {
		wgt->resetDbManager();
	}
}

void TabManager::setDbStationController(DBStationController *controller)
{
	m_dbStationController = controller;
}

//void TabManager::setFlakonRpc(QString rpcHost, uint rpcPort)
//{
//	m_rpcHost = rpcHost;
//	m_rpcPort = rpcPort;
//}

QString TabManager::getStationName(const int id)
{
	Station *t = m_stationsMap.value(id);
	if(!t) {
		return "";
	}
	return t->getName();
}

void TabManager::setActiveTab(const int id)
{
	QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

	if (tabBar != NULL) {
		tabBar->setCurrentIndex(id);
	}
}

/// slot tab change
void TabManager::changeTabSlot(int index)
{
	if (NULL != m_currentTabWidget) {
		m_currentTabWidget->deactivate();
	}

	QString tabName = m_tabWidget->tabText(index);
	m_currentTabWidget = m_tabWidgetsMap.value(tabName, NULL);

	if (m_currentTabWidget == NULL) {
		return;
	}

	m_currentTabWidget->activate();
}

void TabManager::slotUpdateDBStationsLists()
{
	foreach (ITabWidget* tabWidget, m_tabWidgetsMap) {
		tabWidget->updateListsSelections();
	}
}

void TabManager::slotSendSolverSetupCommand(QByteArray data)
{
	m_rpcFlakonClient->sendSolverSetupSettings( data );
}

void TabManager::setStationsConfiguration(const QList<StationConfiguration>& stationList)
{
	foreach (StationConfiguration stationConf, stationList) {
		Station *station = new Station( m_dbManager, m_rpcFlakonClient, this );

		station->setId(stationConf.id);
		station->setName(stationConf.name);
		station->setLatitude(stationConf.latitude);
		station->setLongitude(stationConf.longitude);
		station->setPrm300Ip(stationConf.hostPrm300);
		station->setAdcIp(stationConf.hostADC);
		station->setCenter( stationConf.freqPrm );

		m_stationsMap.insert(station->getId(), station);
	}
}

void TabManager::addStationTabs(unsigned int zone, unsigned int typeRds)
{

	m_tabId = m_tabWidgetZone->addTab(m_tabWidget, QString::number(zone) + " - " + QString::number(typeRds));

	m_correlationControllers = new CorrelationControllersContainer(this);

	m_correlationControllers->init( CalculateDelaysCount(m_stationsMap.count()) );
	m_panoramaFreqControl->initChannelCount( m_stationsMap.count() );

	QStringList stationNamesList = createStationNamesList();

	CommonSpectrumTabWidget* commonTabSpectrumWidget = new CommonSpectrumTabWidget(m_tabWidget);
	commonTabSpectrumWidget->setDbManager(m_dbManager);
	commonTabSpectrumWidget->setStationNamesList(stationNamesList);
	commonTabSpectrumWidget->setCorrelationComponent(m_correlationControllers);
	commonTabSpectrumWidget->setFlakonRpcClient(m_rpcFlakonClient);

	m_tabWidgetList.append(commonTabSpectrumWidget);

	m_rpcFlakonClient->registerReceiver(commonTabSpectrumWidget);

	QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

	foreach (Station* station, m_stationsMap) {
		if(m_tabWidgetsMap.contains(station->getName()))
		{
			continue;
		}
		TabSpectrumWidget* tabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);

		TabSpectrumWidgetController* tabController = new TabSpectrumWidgetController(
					station, m_correlationControllers, this, tabSpectrumWidget,
					m_rpcHost, m_rpcPort, this);

		tabController->setDbManager(m_dbManager);
		tabController->setDbStationController(m_dbStationController);
		tabController->setStationNamesList(stationNamesList);
		tabController->setRpcFlakonClient(m_rpcFlakonClient);
		tabController->setPanoramaFreqControl(m_panoramaFreqControl);
		tabSpectrumWidget->setRpcFlakonClient(m_rpcFlakonClient);

		tabController->appendView(tabSpectrumWidget);
		tabController->setControlPanelController(m_controlPanelController);
		tabController->setControlPanelControllerTrue(m_panelController);

		m_tabWidgetList.append(tabController);

		tabController->getSpectrumWidget()->setZeroFrequency(station->getCenterVal());

		m_dbManager->registerReceiver(tabController);
		m_rpcFlakonClient->registerReceiver(tabController);

		int index = m_tabWidget->addTab(tabSpectrumWidget, station->getName());

		//QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tabSpectrumWidget->getIndicator());
		}

		connect(tabController, SIGNAL(signalUpdateDBStationsLists()), this, SLOT(slotUpdateDBStationsLists()));

		m_tabWidgetsMap.insert(station->getName(), tabController);
		commonTabSpectrumWidget->insertSpectrumWidget(tabController->getSpectrumWidget());
	}

	commonTabSpectrumWidget->deactivate();
	int index = m_tabWidget->addTab(commonTabSpectrumWidget, tr("Common"));

	if (tabBar != NULL) {
		tabBar->setTabButton(index, QTabBar::LeftSide, commonTabSpectrumWidget->getIndicator());
	}

	QString tabName = m_tabWidget->tabText(index);
	m_tabWidgetsMap.insert(tabName, commonTabSpectrumWidget);

	emit readyToStart(m_id);
}

void TabManager::addZoneType(unsigned int zone, unsigned int typeRds)
{
	m_tabWidgetZone->addTab(m_tabWidget, QString(QString::number(zone) + " - " + QString::number(typeRds)));
	//    int index = m_tabWidgetZone->addTab(m_tabWidget, QString::number(zone));
	//    QTabBar* tabBar = m_tabWidgetZone->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));
	//    if (tabBar != NULL) {
	//        tabBar->setTabButton(index, QTabBar::LeftSide, new QLabel(QString::number(zone) + " - " + QString::number(typeRds)));
	//    }

}

int TabManager::getChannelCount()
{
	return m_stationsMap.size();
}

void TabManager::clearAllInformation()
{
	m_rpcFlakonClient->clearAllReceiversList();

	m_currentTabWidget = NULL;
	//	m_tabWidget->setEnabled(false);

	QString tabName = tr("Common");
	CommonSpectrumTabWidget* commonTabSpectrumWidget = dynamic_cast<CommonSpectrumTabWidget*>(m_tabWidgetsMap.take(tabName));
	if (commonTabSpectrumWidget != NULL) {
		commonTabSpectrumWidget->clearSpectrumWidgetsContainer();
		commonTabSpectrumWidget->deleteLater();
		commonTabSpectrumWidget = NULL;
	}

	foreach (Station* station, m_stationsMap) {
		TabSpectrumWidgetController* tabController = dynamic_cast<TabSpectrumWidgetController*>(m_tabWidgetsMap.take(station->getName()));
		if (tabController != NULL){
			if (m_dbManager != NULL) {
				m_dbManager->deregisterReceiver(tabController);
			}
			delete tabController;
			tabController = NULL;
		}
	}



	for (qint32 index = 0; index < m_tabWidget->count(); ++index) {
		QWidget* tabWidget = m_tabWidget->widget(index);
		if (tabWidget != NULL){
			delete tabWidget;
			tabWidget = NULL;
		}
		m_tabWidget->removeTab(index);
	}

	if (m_correlationControllers != NULL) {
		delete m_correlationControllers;
		m_correlationControllers = NULL;
	}

	foreach (Station* station, m_stationsMap) {
		if (station != NULL) {
			delete station;
			station = NULL;
		}
	}

	m_tabWidgetsMap.clear();
	m_tabWidget->clear();
	m_stationsMap.clear();

	if (m_rpcFlakonClient != NULL) {
		m_rpcFlakonClient->stop();
	}
}

void TabManager::onClose()
{
	disconnect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
}

void TabManager::setControlPanelController(ICorrelationListener* controller)
{
	m_controlPanelController = controller;
}

void TabManager::setControlPanelController(ControlPanelController *controller)
{
	m_panelController = controller;
}

void TabManager::setResponseCommonFreq(quint32 freq)
{
	if(m_panelController == NULL)
	{
		return;
	}

	m_panelController->setResponseFreq(freq);
}

int TabManager::getTabId()
{
	return m_tabId;
}

QTabWidget *TabManager::getTabWidgetZone()
{
	return m_tabWidgetZone;
}

void TabManager::initRpcConfig()
{
	m_rpcConfigClient = new RpcConfigClient;
	m_rpcConfigClient->registerReceiver(this);

	m_rpcClientConfigThread = new QThread;
	connect(m_rpcConfigClient, SIGNAL(destroyed()), m_rpcClientConfigThread, SLOT(terminate()));
	connect(m_rpcClientConfigThread, SIGNAL(finished()), m_rpcConfigClient, SLOT(deleteLater()), Qt::DirectConnection);
	connect(m_rpcClientConfigThread, SIGNAL(destroyed()), m_rpcConfigClient, SLOT(deleteLater()));

	m_rpcConfigClient->moveToThread(m_rpcClientConfigThread);
	m_rpcClientConfigThread->start();

	connect(m_rpcConfigClient, SIGNAL(connectionEstablishedSignal()), this, SLOT(rpcConnectionEstablished()));

	m_rpcFlakonClient = new RpcFlakonClientWrapper;
	m_rpcFlakonClient->registerReceiver(this);
	m_rpcClientThread = new QThread;
	connect(m_rpcFlakonClient, SIGNAL(destroyed()), m_rpcClientThread, SLOT(terminate()));
	connect(m_rpcClientThread, SIGNAL(finished()), m_rpcFlakonClient, SLOT(deleteLater()), Qt::DirectConnection);
	connect(m_rpcClientThread, SIGNAL(destroyed()), m_rpcFlakonClient, SLOT(deleteLater()));

	m_rpcFlakonClient->moveToThread(m_rpcClientThread);
	m_rpcClientThread->start();

	connect(m_rpcFlakonClient, SIGNAL(connectionEstablishedSignal()), this, SLOT(rpcConnectionEstablishedFlakon()));
}

void TabManager::onGlobalAutoSearchEnabled(const bool isEnabled)
{
	foreach (ITabWidget* tabWidget, m_tabWidgetsMap) {
		ISpectrumWidget* spectrumWidget = tabWidget->getSpectrumWidget();

		if (NULL == spectrumWidget) {
			continue;
		}

		spectrumWidget->setAutoSearch(isEnabled);
	}
}

void TabManager::onGlobalPanoramaEnabled(const bool isEnabled)
{
	foreach (ITabWidget* tabWidget, m_tabWidgetsMap) {
		ISpectrumWidget* spectrumWidget = tabWidget->getSpectrumWidget();

		if (NULL == spectrumWidget) {
			continue;
		}

		spectrumWidget->setPanorama(isEnabled);
	}
}


void TabManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	emit signalMethodCalled(method, argument);
}

void TabManager::readProto(const QByteArray& data)
{
	RdsProtobuf::Packet pkt;
	pkt.ParseFromArray( data.data(), data.size() );

	if( !pkt.has_from_server() ) {
		return;
	}

	if( pkt.from_server().has_current() ) {
		if( pkt.from_server().current().has_location() ) {
			m_locationSetupController->setLocationSetup( pkt.from_server().current().location() );
			m_panelController->setResponseFreq(
						pkt.from_server().current().location().options().central_frequency()
						);
		}

		if( pkt.from_server().current().has_detector() ) {
			m_locationSetupController->setDetectorSetup( pkt.from_server().current().detector() );
		}

		if( pkt.from_server().current().has_correction() ) {
			m_locationSetupController->setCorrectionSetup( pkt.from_server().current().correction() );
		}

		if(pkt.from_server().current().has_mode()) {
			m_panelController->onSetSystem( pkt.from_server().current().mode().index(),
											pkt.from_server().current().mode().status() );
		}
	}

	if(pkt.from_server().has_answer()) {
		if(pkt.from_server().answer().has_confirmation()) {
			emit signalMessageConfirm(
						QString::fromStdString(pkt.from_server().answer().confirmation().str()));
		}
		if(pkt.from_server().answer().has_error()) {
			emit signalMessageError(
						QString::fromStdString(pkt.from_server().answer().error().str()));
		}
	}
}


void TabManager::rpcConnectionEstablished()
{
	m_rpcConfigClient->requestGetDbConfiguration("./Tabs/Db.ini");
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");

	if (m_rpcFlakonClient != NULL) {
		m_rpcFlakonClient->init(m_rpcPort, QHostAddress(m_rpcHost));
	}
}

void TabManager::rpcConnectionEstablishedFlakon()
{
	int y  = 0;
	y = 10;
}

void TabManager::slotShowError(QString msg)
{
	//QMessageBox::critical(0, "Error", msg, QMessageBox::Ok);
}

void TabManager::slotShowConfirm(QString msg)
{
	//QMessageBox::information(0, "Confirm", msg, QMessageBox::Ok);
}

void TabManager::slotMethodCalled(const QString &method, const QVariant &argument)
{
	QByteArray data = argument.toByteArray();
	if (method == RPC_METHOD_CONFIG_ANSWER_STATION_LIST) {

		//TODO reinit, now it is failing when server restart
		if(!m_stationsMap.isEmpty()) {
			return;
		}

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		QList<StationConfiguration> stationList;
		unsigned int zone;
		unsigned int typeRds;
		dataStream >> zone;
		dataStream >> typeRds;
		dataStream >> stationList;

		// m_tabManager->addZoneType(zone, typeRds);

		//TODO понять как очистить конкретный таб
		//clearAllInformation();
		setStationsConfiguration(stationList);
		addStationTabs(zone, typeRds);
		m_panelController->setMapStations(getStations());

	} else if(method == RPC_METHOD_CONFIG_RDS_ANSWER) {
		//data
		readProto( data );
	} else if (method == RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		DBConnectionStruct dbConfig;
		dataStream >> dbConfig;

		m_dbStationController->connectToDB(dbConfig);
	}
}
