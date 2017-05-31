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
	, m_externalCorrelationControllers(NULL)
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
	, m_tabCount(0)
	, m_listForm(NULL)
	, m_solverSetup(NULL)
	, m_externalCorrelationWidget(NULL)
	, m_dsController(NULL)
{
	m_tabWidget = new QTabWidget(tabWidget);

    m_isSystemMerge = false;

	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
	connect(m_tabWidgetZone, SIGNAL(currentChanged(int)), this, SLOT(changeCommonTabSlot(int)));

	m_locationSetupController = new LocationSetupWidgetController(m_id, this);
	LocationSetupWidget* locationSetupWgt = new LocationSetupWidget(m_id, tabWidget);
	m_locationSetupController->appendView(locationSetupWgt);

	m_panoramaFreqControl = new PanoramaFreqControl(this);

	connect( m_locationSetupController, SIGNAL(sendRdsData(QByteArray)), this, SLOT( slotSendRdsData(QByteArray)) );
    connect( m_locationSetupController, SIGNAL(sendCPPacketData(QByteArray)), this, SLOT( slotSendCPPacketData(QByteArray)) );

	connect(this, SIGNAL(signalMethodCalled(QString,QVariant)), this, SLOT(slotMethodCalled(QString,QVariant)));

	connect(m_locationSetupController, SIGNAL(signalNIIPPWorkStatus(QString, bool)),
			this, SIGNAL(signalNIIPPWorkStatus(QString,bool)));
}

TabManager::~TabManager()
{
	m_rpcFlakonClient->stop();
	m_rpcConfigClient->stop();

	m_rpcFlakonClient->deregisterReceiver(m_locationSetupController);

	disconnect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
	m_rpcClientThread->exit();
	m_rpcClientThread->deleteLater();

	m_rpcClientConfigThread->exit();
	m_rpcClientConfigThread->deleteLater();

	m_locationSetupController->deleteLater();

	clearAllInformation();

	if(m_tabWidget != NULL)
	{
		delete m_tabWidget;
	}
}

void TabManager::slotSendRdsData(QByteArray data)
{
    m_rpcFlakonClient->sendRdsProto(data);
}

void TabManager::slotSendCPPacketData(QByteArray data)
{
    m_rpcFlakonClient->sendCPPacketProto(data);
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

	m_rpcFlakonClient->registerReceiver( m_locationSetupController );

	connect( setupSolver, SIGNAL(onSendSolverCommandSettings(QByteArray)), this, SLOT( slotSendSolverSetupCommand(QByteArray)) );
	//connect( resultSolver, SIGNAL(solverResult(QByteArray)), this, SLOT(slotSolverResult(QByteArray)) );
	connect( resultSolver, SIGNAL(solverResult(QByteArray)), m_panelController, SLOT(slotSolverResult(QByteArray)) );

	m_solverSetup = setupSolver;

    connect(m_panelController, SIGNAL(signalDopplerDetect(QString)),
            resultSolver, SLOT(slotDopplerMail(QString)));
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
	m_cpView = new ControlPanelWidget();

    ControlPanelController* controlPanelController = new ControlPanelController(m_id, m_mainTitle, this);
	controlPanelController->setDbStationController(m_dbStationController);
	controlPanelController->setRpcFlakonClient(m_rpcFlakonClient);
	controlPanelController->appendView(m_cpView);
	controlPanelController->registerReceiver(this);
	controlPanelController->setDbManager(m_dbManager);
	this->setControlPanelController((ICorrelationListener* )controlPanelController);
	setControlPanelController(controlPanelController);
	controlPanelController->setLocationSetupController(m_locationSetupController);

	m_dsController = new DataSourceController(this, m_dbManager, m_rpcFlakonClient, m_panoramaFreqControl, this );
	m_dsController->setLocationController(m_locationSetupController);
	m_dsController->setControlPanelController(m_panelController);

	connect(m_panelController, SIGNAL(signalSetComonFreq(double)),
			m_locationSetupController, SLOT(slotOnSetCommonFreq(double)));

	connect(m_panoramaFreqControl, SIGNAL(setCommonFreq(double)),
			m_locationSetupController, SLOT(slotOnSetCommonFreq(double)));

	controlPanelController->setSpectrumReveive(true);

	connect(this, SIGNAL(signalMessageError(QString)), m_cpView, SLOT(showError(QString)));
	connect(this, SIGNAL(signalMessageConfirm(QString)), m_cpView, SLOT(showConfirm(QString)));
	connect(this, SIGNAL(signalLocationError(QString)), m_cpView, SLOT(showLocationError(QString)));

	connect(this, SIGNAL(signalMessageError(QString)), m_locationSetupController->getView(), SLOT(showError(QString)));
	connect(this, SIGNAL(signalMessageConfirm(QString)), m_locationSetupController->getView(), SLOT(showConfirm(QString)));
	connect(this, SIGNAL(signalLocationError(QString)), m_locationSetupController->getView(), SLOT(showLocationError(QString)));

	connect(m_panelController, SIGNAL(signalSystemMerge(bool)), this, SLOT(onSystemMerge(bool)));
}

void TabManager::addControlPanelWidget(ControlPanelWidget *cpWidget)
{
	m_cpWidget = cpWidget;
}

void TabManager::start()
{
	changeTabSlot(0);

	changeTabSlot(m_tabWidget->count()-1);
	m_tabWidget->setEnabled(true);

	//Request Settings
	for(int i = 0; i < m_devChannelMap.size(); i++) {
		RdsProtobuf::Packet pkt;
		createGetSystemDeviceStates(i, pkt );
		m_rpcFlakonClient->sendRdsProto( pack(pkt) );
	}

	for(int i = 0; i < m_devChannelMap.size(); i++) {
		for(int j = 0; j < m_devChannelMap.value(i); j++) {
			RdsProtobuf::Packet pkt1;
			createGetSystemReceiverOptions(i, j, pkt1 );
			m_rpcFlakonClient->sendRdsProto( pack(pkt1) );

			RdsProtobuf::Packet pkt2;
			createGetSystemReceiverStatus(i, j, pkt2 );
			m_rpcFlakonClient->sendRdsProto( pack(pkt2) );
		}
	}
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

	m_tabWidget->setCurrentIndex(index);
	m_currentTabWidget->activate();
}

void TabManager::changeCommonTabSlot(int index)
{
	if(m_tabId != index) {
		return;
	}

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

void TabManager::setStationsConfiguration(RdsProtobuf::System_SystemOptions msg)
{
	int cnt = 0;
	QStringList platformList;
	m_devChannelMap.clear();

	//const QList<StationConfiguration>& stationList

	//foreach (StationConfiguration stationConf, stationList) {
	for(int i = 0; i<msg.devices_size(); i++) {

		m_devChannelMap.insert(i, msg.devices(i).channels_size());
		for( int j = 0; j<msg.devices(i).channels_size(); j++ ) {

			Station *station = new Station( m_dbManager, m_rpcFlakonClient, this );
			RdsProtobuf::ChannelOptions chOpt = msg.devices(i).channels(j);

			station->setId(cnt);
			station->setPlatformId( i );
			station->setChannelId( j );

			station->setName( QString::fromStdString(chOpt.title()) );
			station->setLatitude(chOpt.coordinates().latitude());
			station->setLongitude(chOpt.coordinates().longitude());
			station->setPrm300Ip( QString::fromStdString(chOpt.receiver().ip()) );
			station->setAdcIp( QString::fromStdString(msg.devices(i).ip()) );

			station->setLocationSetupController(m_locationSetupController);

			m_stationsMap.insert(station->getId(), station);
			m_dbStationController->addStation( station->getName(), station->getPrm300Ip() );
			cnt++;
		}
	}

	//m_locationSetupController->setPlatformList( platformList );
}

void TabManager::addStationTabs(QString platformName)
{

	m_tabId = m_tabWidgetZone->addTab(m_tabWidget, platformName);

	m_externalCorrelationWidget = new CorrelationGroupWidget(0);

	m_correlationControllers = new CorrelationControllersContainer(this);
	m_correlationControllers->setLocationController(m_locationSetupController);
    m_correlationControllers->setControlPanelController(m_panelController);
    m_correlationControllers->init( CalculateDelaysCount(m_stationsMap.count()), 1 ); //1 - is Indicator type
	connect((CorrelationControllersContainer*)m_correlationControllers, SIGNAL(signalExpand()),
			this, SLOT(slotExpandCorrelations()));

	m_externalCorrelationControllers = new CorrelationControllersContainer(this);
	m_externalCorrelationControllers->setLocationController(m_locationSetupController);
	m_externalCorrelationControllers->init( CalculateDelaysCount(m_stationsMap.count()), 0 ); //0 - is Spect type

	for(int i = 0; i < m_externalCorrelationControllers->count(); i++) {
		ICorrelationWidget* correlationWidget = m_externalCorrelationControllers->get(i);
		m_externalCorrelationWidget->insertCorrelationWidget(correlationWidget);
	}

	m_panoramaFreqControl->initChannelCount( m_stationsMap.count() );

	initAnalysisControllers(0);

	QStringList stationNamesList = createStationNamesList();

	CommonSpectrumTabWidget* commonTabSpectrumWidget = new CommonSpectrumTabWidget(m_tabWidget);
	commonTabSpectrumWidget->setDbManager(m_dbManager);
	commonTabSpectrumWidget->setStationNamesList(stationNamesList);
	commonTabSpectrumWidget->setCorrelationComponent(m_correlationControllers);
	commonTabSpectrumWidget->setFlakonRpcClient(m_rpcFlakonClient);
	commonTabSpectrumWidget->setControlPanelWidget(m_cpView);

	m_tabWidgetList.append(commonTabSpectrumWidget);

	//m_rpcFlakonClient->registerReceiver(commonTabSpectrumWidget);

	connect(commonTabSpectrumWidget, SIGNAL(signalFreqChanged(int)),
			m_locationSetupController, SLOT(slotOnSetCommonFreq(int)));

	QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

	foreach (Station* station, m_stationsMap) {
		if(m_tabWidgetsMap.contains(station->getName()))
		{
			continue;
		}
		TabSpectrumWidget* tabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);

		TabSpectrumWidgetController* tabController = new TabSpectrumWidgetController(
					station, m_correlationControllers, m_analysisControllerMap, this, tabSpectrumWidget,
					m_rpcHost, m_rpcPort, this);

		tabController->setControlPanelWidget(m_cpView);

		tabController->setDbManager(m_dbManager);
		tabController->setDbStationController(m_dbStationController);
		tabController->setStationNamesList(stationNamesList);
		tabController->setRpcFlakonClient(m_rpcFlakonClient);
		tabController->setPanoramaFreqControl(m_panoramaFreqControl);
		tabSpectrumWidget->setRpcFlakonClient(m_rpcFlakonClient);

		tabController->setExtCorrelController(m_externalCorrelationControllers);
		tabController->appendView(tabSpectrumWidget);
		tabController->setControlPanelController(m_controlPanelController);
		tabController->setControlPanelControllerTrue(m_panelController);

		m_tabWidgetList.append(tabController);

		tabController->getSpectrumWidget()->setZeroFrequency(station->getCenterVal());

		m_dbManager->registerReceiver(tabController);
		//m_rpcFlakonClient->registerReceiver(tabController);

		int index = m_tabWidget->addTab(tabSpectrumWidget, station->getName());
		m_tabWidgetContainer.insert(index, tabSpectrumWidget);

		//QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tabSpectrumWidget->getIndicator());
		}

		tabSpectrumWidget->getSpectrumController()->setLocationSetupWidgetController( m_locationSetupController );

		connect(tabController, SIGNAL(signalUpdateDBStationsLists()), this, SLOT(slotUpdateDBStationsLists()));

		connect(tabController, SIGNAL(signalFreqChanged(int)), m_locationSetupController, SLOT(slotOnSetCommonFreq(int)));
		//connect(m_locationSetupController, SIGNAL(analysisChannelChanged(int)), tabController, SIGNAL(analysisChannelChanged(int)));
		//connect(m_locationSetupController, SIGNAL(analysisChannelChanged(int)), tabSpectrumWidget, SIGNAL(onChangeAnalysisChannel(int)));

		connect(tabSpectrumWidget->getSpectrumController(), SIGNAL(signalAddToList(QString,double,double)),
				this, SLOT(slotShowLists(QString,double,double)));

		m_tabWidgetsMap.insert(station->getName(), tabController);
		commonTabSpectrumWidget->insertSpectrumWidget(tabController->getSpectrumWidget());
	}

	commonTabSpectrumWidget->deactivate();
	int index = m_tabWidget->addTab(commonTabSpectrumWidget, tr("Common"));
	m_tabWidgetContainer.insert(index, commonTabSpectrumWidget);

	m_dsController->initSpectrumDataSource( m_tabWidgetsMap );
	m_dsController->initCorrelationDataSource( m_correlationControllers, m_externalCorrelationControllers );
	m_dsController->initAnalysisDataSource( m_analysisControllerMap );

	if (tabBar != NULL) {
		tabBar->setTabButton(index, QTabBar::LeftSide, commonTabSpectrumWidget->getIndicator());
	}

	QString tabName = m_tabWidget->tabText(index);
	m_tabWidgetsMap.insert(tabName, commonTabSpectrumWidget);

//	m_rpcServer->call(RPC_METHOD_CONFIG_RDS_ANSWER, data);
	m_rpcFlakonClient->sendServerRequestSettings(m_id);
	emit readyToStart(m_id);
}

void TabManager::addZoneType(unsigned int zone, unsigned int typeRds)
{
	m_tabWidgetZone->addTab(m_tabWidget, QString(QString::number(zone) + " - " + QString::number(typeRds)));
}

int TabManager::getChannelCount()
{
	return m_stationsMap.size();
}

void TabManager::clearAllInformation()
{
	m_dsController->deregisterAll();


	m_tabWidgetZone->removeTab(m_tabId);
	m_tabId = -1;

	m_currentTabWidget = NULL;

	QString tabName = tr("Common");
	CommonSpectrumTabWidget* commonTabSpectrumWidget = dynamic_cast<CommonSpectrumTabWidget*>(m_tabWidgetsMap.take(tabName));
	if (commonTabSpectrumWidget != NULL) {

		m_tabWidgetContainer.remove( m_tabWidgetContainer.key(commonTabSpectrumWidget) );

		commonTabSpectrumWidget->clearSpectrumWidgetsContainer();
		commonTabSpectrumWidget->deleteLater();
		commonTabSpectrumWidget = NULL;
	}

	foreach (Station* station, m_stationsMap) {
		TabSpectrumWidgetController* tabController = dynamic_cast<TabSpectrumWidgetController*>(m_tabWidgetsMap.take(station->getName()));
		if (tabController != NULL){

			ITabWidget* iwgt = dynamic_cast<ITabWidget*>(tabController);
			if(iwgt) {
				m_tabWidgetList.removeAll( iwgt );
			}

			if (m_dbManager != NULL) {
				m_dbManager->deregisterReceiver(tabController);
			}
			delete tabController;
			tabController = NULL;
		}
	}

	for (qint32 index = 0; index < m_tabWidget->count(); ++index) {
		QWidget* tabWidget = m_tabWidget->widget(index);

		m_tabWidgetContainer.remove( m_tabWidgetContainer.key(tabWidget) );

		ITabWidget* iwgt = dynamic_cast<ITabWidget*>(tabWidget);
		if(iwgt) {
			m_tabWidgetList.removeAll( iwgt );
		}
		if (tabWidget != NULL) {
			delete tabWidget;
		}
		m_tabWidget->removeTab(index);
	}

	if (m_correlationControllers != NULL) {
		delete m_correlationControllers;
		m_correlationControllers = NULL;
	}

	if (m_externalCorrelationControllers != NULL) {
		delete m_externalCorrelationControllers;
		m_externalCorrelationControllers = NULL;
	}

	foreach (Station* station, m_stationsMap) {
		if (station != NULL) {
			delete station;
			station = NULL;
		}
	}

	for (int i=0; i<m_tabWidgetList.size(); i++) {
		delete (m_tabWidgetList.takeAt(0));
	}

	foreach (QWidget* wgt, m_tabWidgetContainer.values()) {
		delete wgt;
	}

	m_tabWidgetList.clear();
	m_tabWidgetsMap.clear();
	m_tabWidgetContainer.clear();
	m_tabWidget->clear();
	m_stationsMap.clear();

	if(m_externalCorrelationWidget) {
		m_externalCorrelationWidget->close();
		delete m_externalCorrelationWidget;
		m_externalCorrelationWidget = NULL;
	}

	foreach (IAnalysisWidget* wgt, m_analysisControllerMap) {
		delete wgt;
	}

	m_analysisControllerMap.clear();
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

	connect(m_locationSetupController, SIGNAL(solverConnectionState(bool)), m_panelController, SLOT(setSolverConnectState(bool)));
}

void TabManager::setResponseCommonFreq(quint32 freq)
{
	if(m_panelController == NULL)
	{
		return;
	}

	m_panelController->setResponseFreq(freq);
}

RdsProtobuf::ClientMessage_OneShot_Location TabManager::getCurrentOptions(bool &isPanorama, double& start, double& end)
{
	if( !m_locationSetupController || !m_panelController ) {
		return RdsProtobuf::ClientMessage_OneShot_Location();
	}

	isPanorama = m_panelController->isPanorama(start, end);
	return m_locationSetupController->getCurrentLocation();
}

void TabManager::setCurrentOptions(const RdsProtobuf::ClientMessage_OneShot_Location& msg,
								   const bool isPanorama,
								   const double& start, const double& end)
{
	if( !m_locationSetupController || !m_panelController ) {
		return;
	}

	m_locationSetupController->updateLocation(msg);
	m_panelController->updatePanorama(isPanorama, start, end);
}

void TabManager::setListDialog(ListsDialog *dlg)
{
	m_listForm = dlg;
	m_panelController->setListDialog(m_listForm);
}

int TabManager::getTabId()
{
	return m_tabId;
}

QTabWidget *TabManager::getTabWidgetZone()
{
	return m_tabWidgetZone;
}

void TabManager::setNIIPPAlarmMode()
{
	if(m_panelController) {
		m_panelController->changeFrequency(6000);
		m_panelController->setManualMode();
	}
}

void TabManager::initRpcConfig()
{
	m_rpcConfigClient = new RpcConfigClient;
	m_rpcConfigClient->registerReceiver(this);

	m_rpcClientConfigThread = new QThread;
	connect(m_rpcConfigClient, SIGNAL(destroyed()), m_rpcClientConfigThread, SLOT(terminate()));
	connect(m_rpcClientConfigThread, SIGNAL(finished()), m_rpcConfigClient, SLOT(deleteLater()), Qt::DirectConnection);
	connect(m_rpcClientConfigThread, SIGNAL(destroyed()), m_rpcConfigClient, SLOT(deleteLater()));

	m_rpcFlakonClient = new RpcFlakonClientWrapper;
	m_rpcFlakonClient->registerReceiver(this);
	m_rpcClientThread = new QThread;
	connect(m_rpcFlakonClient, SIGNAL(destroyed()), m_rpcClientThread, SLOT(terminate()));
	connect(m_rpcClientThread, SIGNAL(finished()), m_rpcFlakonClient, SLOT(deleteLater()), Qt::DirectConnection);
	connect(m_rpcClientThread, SIGNAL(destroyed()), m_rpcFlakonClient, SLOT(deleteLater()));

	connect(m_rpcConfigClient, SIGNAL(onBaseConnected()), this, SLOT(rpcConnectionEstablished()));
	connect(m_rpcConfigClient, SIGNAL(onBaseDisconnected()), this, SLOT(rpcConnectionDropped()));

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

void TabManager::onGlobalPanoramaEnabled(const bool isEnabled, int start, int end)
{
	foreach (ITabWidget* tabWidget, m_tabWidgetsMap) {
		ISpectrumWidget* spectrumWidget = tabWidget->getSpectrumWidget();

		if (NULL == spectrumWidget) {
			continue;
		}

		spectrumWidget->setPanorama(isEnabled);
	}

	if( m_dsController ) {
		m_dsController->enablePanorama( isEnabled, start, end );
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

	RdsProtobuf::ServerMessage sPkt = pkt.from_server();

	if( isSystemSystemOptions(sPkt) ) {
		clearAllInformation();

		RdsProtobuf::System_SystemOptions sOptMsg = getSystemSystemOptions(sPkt);
		m_locationSetupController->setDevicesState( sOptMsg );
		m_mainTitle = QString::fromStdString(sOptMsg.title());
		m_locationSetupController->getView()->setTitle(m_mainTitle);
        m_panelController->setTitle(m_mainTitle);

		emit onTitleUp(m_id, m_mainTitle);

		setStationsConfiguration(sOptMsg);
		addStationTabs(m_mainTitle);
		m_panelController->setMapStations(getStations());
	}

	if( isSystemDeviceOptions(sPkt) ) {
		RdsProtobuf::System_Device sDevPkt = getSystemDeviceOptions(sPkt);

		m_locationSetupController->setDeviceEnableState( sDevPkt.device_index(), sDevPkt.status() );
		QString tabName = tr("Common");
		CommonSpectrumTabWidget* commonTabSpectrumWidget = dynamic_cast<CommonSpectrumTabWidget*>(m_tabWidgetsMap.value(tabName));
		if (commonTabSpectrumWidget != NULL) {
			commonTabSpectrumWidget->setIndicatorState( sDevPkt.device_index(), sDevPkt.status() );
		}
	}

	if( isSystemReceiverOptions(sPkt) ) {
		RdsProtobuf::System_Receiver sRecPkt = getSystemReceiver(sPkt);
		//params

		int dev = sRecPkt.device_index();
		int channel = sRecPkt.channel_index();

		Station* st = getStationByDevChannel(dev, channel);
		if( !st ) {
			return;
		}

		ITabWidget* wgt = m_tabWidgetsMap.value(st->getName(), NULL);

		if(!wgt) {
			return;
		}

		wgt->getSpectrumWidget()->setReceiverSettings( sRecPkt.settings() );
	}

	if( isSystemReceiverStatus(sPkt) ) {

		RdsProtobuf::System_Receiver sRecPkt = getSystemReceiver(sPkt);
		//params

		int dev = sRecPkt.device_index();
		int channel = sRecPkt.channel_index();

		Station* st = getStationByDevChannel(dev, channel);
		if( !st ) {
			return;
		}

		ITabWidget* wgt = m_tabWidgetsMap.value(st->getName(), NULL);

		if(!wgt) {
			return;
		}

		ITabSpectrum* spect = dynamic_cast<ITabSpectrum*>(wgt);

		wgt->getSpectrumWidget()->setReceiverStatus( sRecPkt.status() );
		spect->setIndicator( sRecPkt.status() );

		m_locationSetupController->setChannelEnableState( dev, channel, sRecPkt.status() );
	}

	if(isServerLocationShot(sPkt)) {
		RdsProtobuf::ServerMessage_OneShotData_LocationData locPkt = getServerLocationShot(sPkt);
		QString tsrt = QString::fromStdString(locPkt.error());
		bool tstb = locPkt.success();
		if(!locPkt.success()) {
			emit signalLocationError( QString::fromStdString(locPkt.error()) );
		} else {
			emit signalLocationError( "" );
		}
	}

	if(isServerAnalysisShot(sPkt)) {
	}

	if(isServerMessageError(sPkt)) {
		emit signalMessageError( getServerMessageError(sPkt) );
	}

	if(isServerMessageConfirm(sPkt)) {
		emit signalMessageConfirm( getServerMessageConfirm(sPkt) );
	}

	if(isServerMessageRecordAnswer(sPkt)) {
		RdsProtobuf::ServerMessage_OneShotData_RecordData rPkt = getServerMessageRecordAnswer(sPkt);
		QString status = QString(tr("Status: %1"));

		if (rPkt.success()) {
			status = status.arg(tr("Sucess"));
		} else {
			status = status.arg(tr("Error"));
		}

		status.append( QString("\r\n  %1").arg(QString::fromStdString(rPkt.error())) );

		QMessageBox::warning( 0, tr("Record Signal result"), tr("Record Signal result: \r\n %1").arg( status ), QMessageBox::Ok );

		m_panelController->setSpectrumReveive(true);
		m_locationSetupController->setLocationState(true);
	}
}

void TabManager::initAnalysisControllers(int count)
{
	Q_UNUSED(count)
	AnalysisWidget* wgt = new AnalysisWidget(0);
	AnalysisWidgetController* controller = new AnalysisWidgetController(0, this);
	controller->appendView(wgt);

	m_analysisControllerMap.insert(0, controller);

	connect(wgt, SIGNAL(signalAddToList(double,double)), this, SLOT(slotAnalysisAddToList(double, double)));
	connect(wgt, SIGNAL(signalAnalysisContinue(int, bool)), this, SLOT(slotAnalysisContinue(int, bool)));
	connect(wgt, SIGNAL(signalAnalysisClose()), this, SLOT(slotAnalysisClose()));
	connect(wgt, SIGNAL(signalAnalysisSelection(int,double,double)), this, SLOT(slotAnalysisSelection(int, double, double)));
	connect(wgt, SIGNAL(analysisReady(int)), this, SLOT(slotAnalysisReady(int)));
}

Station *TabManager::getStationByDevChannel(int dev, int channel)
{
	foreach (Station* station, m_stationsMap) {
		if(station->getPlatformId() == dev && station->getChannelId() == channel) {
			return station;
		}
	}

	return nullptr;
}

void TabManager::rpcConnectionEstablished()
{
	log_debug("rpcConnectionEstablished");
	m_rpcConfigClient->requestGetDbConfiguration("./Tabs/Db.ini");
	m_rpcConfigClient->requestGetStationList("./Tabs/Tabs.ini");

	if (m_rpcFlakonClient != NULL) {
		m_rpcFlakonClient->init(m_rpcPort, QHostAddress(m_rpcHost));
	}
}

void TabManager::rpcConnectionDropped()
{
}

void TabManager::rpcConnectionEstablishedFlakon()
{
	//Set Indicator


	//ask all RDS params
	RdsProtobuf::Packet pkt;

	createGetSystemSystemOptions( pkt );
	m_rpcFlakonClient->sendRdsProto( pack(pkt) );
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
	if(method == RPC_METHOD_CONFIG_RDS_ANSWER || method == RPC_METHOD_CONFIG_ANSWER_STATION_LIST) {
		//data
		readProto( data );
	} else if (method == RPC_METHOD_CONFIG_ANSWER_DB_CONFIGURATION) {

		QDataStream dataStream(&data, QIODevice::ReadOnly);
		DBConnectionStruct dbConfig;
		dataStream >> dbConfig;

		m_dbStationController->connectToDB(dbConfig);
	} else if (method == RPC_SLOT_SERVER_SEND_SETTINGS) {
		CommonParams::Parameters msg;
		QByteArray data = argument.toByteArray();
		msg.ParseFromArray( data.data(), data.size() );
		if(msg.serverid() == m_id)
		{
			if(msg.has_mergesysctrl())
			{
				onSystemMerge(msg.mergesysctrl());
			}
		}
	}
}

void TabManager::slotExpandCorrelations()
{
	m_externalCorrelationWidget->show();
}

void TabManager::slotAnalysisAddToList(double start, double end)
{
	slotShowLists(m_currentTabWidget->getSpectrumWidget()->getSpectrumName(), (end+start)/2,  (end-start)*1000 );
}

void TabManager::slotAnalysisContinue(int id, bool)
{
	if(id < 0) {
		return;
	}
	m_analysisControllerMap.value(0)->setVisible(false);
	m_panelController->setSpectrumReveive(true);
	m_locationSetupController->setLocationState(true);

	//m_tabWidgetList.at(id+1)->getSpectrumWidget()->setAnalysisSelection(0, 0);
	//m_tabWidget->setEnabled(true);
}

void TabManager::slotAnalysisClose()
{
	m_analysisControllerMap.value(0)->setVisible(false);
	//m_tabWidgetList.at(id+1)->getSpectrumWidget()->setAnalysisSelection(0, 0);
	//m_tabWidget->setEnabled(true);
}

void TabManager::onSystemMerge(bool val)
{
    m_isSystemMerge = val;

	if(val) {
		connect(m_panelController, SIGNAL(onStateChanged()), this, SIGNAL(signalLocationChanged()));
		connect(m_locationSetupController, SIGNAL(onStateChanged()), this, SIGNAL(signalLocationChanged()));
	} else {
		disconnect(m_panelController, SIGNAL(onStateChanged()), this, SIGNAL(signalLocationChanged()));
		disconnect(m_locationSetupController, SIGNAL(onStateChanged()), this, SIGNAL(signalLocationChanged()));
	}
}

void TabManager::slotAnalysisSelection(int ind, double start, double end)
{
	if( m_tabWidgetList.size() <= ind+1 ) {
		return;
	}

	m_tabWidgetList.at(ind+1)->getSpectrumWidget()->setAnalysisSelection(start, end);
}

void TabManager::slotAnalysisReady(int ind)
{
	m_tabWidget->setCurrentIndex(ind);
	//m_tabWidget->setEnabled(false);
}

void TabManager::slotShowLists(QString station, double freq, double bandwidth)
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

	listController->showAddDialog(station, freq, bandwidth);
}
