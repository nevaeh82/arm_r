#include "TabManager.h"

#include <QDebug>

#include "CommonSpectrumTabWidget.h"
#include "TabSpectrumWidgetController.h"

#define DEFAULT_RPC_PORT		24500

TabManager::TabManager(QTabWidget *tabWidget, QObject *parent)
	: QObject( parent )
	, m_tabWidget( tabWidget )
	, m_correlationControllers( NULL )
	, m_currentTabWidget( NULL )
	, m_dbManager( NULL )
	, m_dbStationController( NULL )
	, m_rpcFlakonClient( NULL )
{
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
}

TabManager::~TabManager()
{
}

void TabManager::start()
{
	changeTabSlot(m_tabWidget->currentIndex());
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

int TabManager::createSubModules(const QString& settingsFile)
{
	m_correlationControllers = new CorrelationControllersContainer(this);

	readRpcSettings();

	m_rpcFlakonClient = new RpcFlakonClient(this);
	m_rpcFlakonClient->start( m_rpcPort, QHostAddress( m_rpcHost ) );

	int submodulesCount = readStationSettings(settingsFile);

	m_correlationControllers->init(m_stationsMap.count());

	QStringList stationNamesList = createStationNamesList();

	CommonSpectrumTabWidget* commonTabSpectrumWidget = new CommonSpectrumTabWidget(m_tabWidget);
	commonTabSpectrumWidget->setDbManager(m_dbManager);
	commonTabSpectrumWidget->setStationNamesList(stationNamesList);
	commonTabSpectrumWidget->setCorrelationComponent(m_correlationControllers);


	foreach (Station* station, m_stationsMap) {
		TabSpectrumWidget* tabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);

		TabSpectrumWidgetController* tabController = new TabSpectrumWidgetController(
					station, m_correlationControllers, this, tabSpectrumWidget,
					m_rpcHost, m_rpcPort, this);

		tabController->setDbManager(m_dbManager);
		tabController->setDbStationController(m_dbStationController);
		tabController->setStationNamesList(stationNamesList);
		tabController->setRpcFlakonClient(m_rpcFlakonClient);
		tabController->appendView(tabSpectrumWidget);

		m_dbManager->registerReceiver(tabController);

		int index = m_tabWidget->addTab(tabSpectrumWidget, station->getName());

		QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tabSpectrumWidget->getIndicator());
		}

		m_tabWidgetsMap.insert(station->getName(), tabController);
		commonTabSpectrumWidget->insertSpectrumWidget(tabController->getSpectrumWidget());
	}

	commonTabSpectrumWidget->deactivate();
	int index = m_tabWidget->addTab(commonTabSpectrumWidget, tr("Common"));

	QString tabName = m_tabWidget->tabText(index);
	m_tabWidgetsMap.insert(tabName, commonTabSpectrumWidget);

	AtlantTabWidget* atlant = new AtlantTabWidget(m_tabWidget);
	m_tabWidget->addTab(atlant, tr("Atlant"));

	return submodulesCount;
}

void TabManager::setDbManager(IDbManager *dbManager)
{
	m_dbManager = dbManager;
}

void TabManager::setDbStationController(DBStationController *controller)
{
	m_dbStationController = controller;
}

QString TabManager::getStationName(const int id)
{
	Station *t = m_stationsMap.value(id);
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

void TabManager::readRpcSettings()
{
	QString rpcSettingsFile = QCoreApplication::applicationDirPath() + "/Tabs/RPC.ini";
	QSettings rpcSettings( rpcSettingsFile, QSettings::IniFormat );
	rpcSettings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	m_rpcHost = rpcSettings.value("RPC_UI/IP", "127.0.0.1").toString();
	m_rpcPort = rpcSettings.value("RPC_UI/Port", DEFAULT_RPC_PORT).toUInt();
}

int TabManager::readStationSettings(const QString& settingsFile)
{
	// read settings for RPC UI connections
	readRpcSettings();

	// read settings to generate list of stations
	m_stationsMap.clear();

	QSettings stationSettings( settingsFile, QSettings::IniFormat );
	stationSettings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	QStringList childKeys = stationSettings.childGroups();
	foreach( const QString &childKey, childKeys ) {
		stationSettings.beginGroup(childKey);

		Station *station = new Station( m_dbManager, m_rpcFlakonClient, this );

		station->setId(stationSettings.value("Id", 0).toInt());
		station->setName(stationSettings.value("Name", "Unknown").toString());
		station->setLatitude(stationSettings.value("Latitude", 0).toDouble());
		station->setLongitude(stationSettings.value("Longitude", 0).toDouble());
		station->setPrm300Ip(stationSettings.value("IPprm300", "127.0.0.1").toString());
		station->setAdcIp(stationSettings.value("IPADC", "127.0.0.1").toString());
		station->setAdcPort(stationSettings.value("portADC", 1030).toInt());

		m_stationsMap.insert(station->getId(), station);

		stationSettings.endGroup();
	}

	return m_stationsMap.count();
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
