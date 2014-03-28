#include "TabManager.h"

#include <QDebug>

#include "CommonSpectrumTabWidget.h"
#include "TabSpectrumWidgetController.h"

TabManager::TabManager(QTabWidget *tabWidget, QObject *parent):
	QObject(parent)
{
	m_tabWidget = tabWidget;
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));

	m_correlationControllers = NULL;
	m_currentTabWidget = NULL;
	m_dbManager = NULL;
    m_dbStationController = NULL;
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

	int submodulesCount = readSettings(settingsFile);

	m_correlationControllers->init(m_stationsMap.count());

	QStringList stationNamesList = createStationNamesList();

	CommonSpectrumTabWidget* commonTabSpectrumWidget = new CommonSpectrumTabWidget(m_tabWidget);
	commonTabSpectrumWidget->setDbManager(m_dbManager);
	commonTabSpectrumWidget->setStationNamesList(stationNamesList);
	commonTabSpectrumWidget->setCorrelationComponent(m_correlationControllers);

	foreach (Station* station, m_stationsMap) {
		TabSpectrumWidget* tabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);

		TabSpectrumWidgetController* tabController = new TabSpectrumWidgetController(station, m_correlationControllers, this, tabSpectrumWidget);
		tabController->setDbManager(m_dbManager);
        tabController->setDbStationController(m_dbStationController);
		tabController->setStationNamesList(stationNamesList);
		m_dbManager->registerReceiver(tabController);
		tabController->appendView(tabSpectrumWidget);

		int index = m_tabWidget->addTab(tabSpectrumWidget, station->getName());

		QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tabSpectrumWidget->getIndicator());
		}

		m_tabWidgetsMap.insert(station->getName(), tabController);
		commonTabSpectrumWidget->insertSpectrumWidget(tabController->getSpectrumWidget());
	}

	checkStatus();

	/// FOR FUTURE
	//	TabSpectrumWidgetController* commonTabController =  new TabSpectrumWidgetController(it.value(), _common_spectra, _common_correlations, _model_spectrum, _db_manager_spectrum, this);
	//	TabSpectrumWidget* commonTabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);
	//	commonTabController->appendView(commonTabSpectrumWidget);

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

/// call this method when data in tree has changed
void TabManager::sendCommand(const QString &stationName, TypeCommand type, IMessage *msg)
{
	ITabWidget* tabController = m_tabWidgetsMap.value(stationName, NULL);

	if (NULL == tabController) {
		return;
	}

	///TODO: update in future

	TabSpectrumWidgetController* tabController1 = static_cast<TabSpectrumWidgetController*>(tabController);
	tabController1->sendCommand(type, msg);
}

void TabManager::setActiveTab(const int id)
{
	QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

	if (tabBar != NULL) {
		tabBar->setCurrentIndex(id);
	}
	checkStatus();

	//    emit signalChangeTab(id);
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
	checkStatus();

}

/// read settings for generated submodules (tabs)
int TabManager::readSettings(const QString& settingsFile)
{
	m_stationsMap.clear();
	
	QSettings settings(settingsFile, QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	QStringList childKeys = settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);

		Station *station = new Station(this);

		station->setId(settings.value("Id", 0).toInt());
		station->setName(settings.value("Name", "Unknown").toString());
		station->setLatitude(settings.value("Latitude", 0).toDouble());
		station->setLongitude(settings.value("Longitude", 0).toDouble());
		station->setPrm300Ip(settings.value("IPprm300", "127.0.0.1").toString());
		station->setAdcIp(settings.value("IPADC", "127.0.0.1").toString());
		station->setAdcPort(settings.value("portADC", 1030).toInt());

		m_stationsMap.insert(station->getId(), station);

		settings.endGroup();
	}

	return m_stationsMap.count();
}

void TabManager::checkStatus()
{
	///TODO: update
	/*QMap<int, ITabWidget* >::iterator it;
	for(it = tabWidgetsMap.begin(); it != tabWidgetsMap.end(); ++it)
	{
		it.value()->check_status();
	}*/
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
