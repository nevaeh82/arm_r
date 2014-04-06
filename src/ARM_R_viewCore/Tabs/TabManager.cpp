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
{
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));
	m_rpcFlakonClient = new RpcFlakonClient(this);
}

TabManager::~TabManager()
{
}

void TabManager::setRpcConfig(const quint16& port, const QString& host)
{
	m_rpcHost = host;
	m_rpcPort = port;
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
		station->setAdcPort(stationConf.portADC);

		m_stationsMap.insert(station->getId(), station);
	}
}

void TabManager::setAtlantConfiguration(const AtlantConfiguration& atlantConfig)
{
	AtlantTabWidget* atlant = new AtlantTabWidget(m_tabWidget);
	atlant->setConfig(atlantConfig);
	m_tabWidget->addTab(atlant, tr("Atlant"));
}

void TabManager::addStationTabs()
{
	m_rpcFlakonClient->start(m_rpcPort, QHostAddress(m_rpcHost));

	m_correlationControllers = new CorrelationControllersContainer(this);
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

	emit readyToStart();
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
