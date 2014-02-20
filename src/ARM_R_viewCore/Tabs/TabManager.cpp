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
}

TabManager::~TabManager()
{
}

void TabManager::start()
{
	changeTabSlot(m_tabWidget->currentIndex());
}

int TabManager::createSubModules(const QString& settingsFile)
{
	m_correlationControllers = new CorrelationControllersContainer();

	int submodulesCount = readSettings(settingsFile);

	m_correlationControllers->init(m_tabsPropertyMap.count() - 1);

	CommonSpectrumTabWidget* commonTabSpectrumWidget = new CommonSpectrumTabWidget(m_dbManager, m_tabWidget);
	commonTabSpectrumWidget->setCorrelationComponent(m_correlationControllers);

	QMap<int, TabsProperty* >::iterator it;
	for(it = m_tabsPropertyMap.begin(); it != m_tabsPropertyMap.end(); ++it)
	{
		TabSpectrumWidgetController* tabController =  new TabSpectrumWidgetController(it.value(), m_correlationControllers, m_dbManager, this);
		TabSpectrumWidget* tabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);

		tabController->appendView(tabSpectrumWidget);

		int index = m_tabWidget->addTab(tabSpectrumWidget, it.value()->get_name());

		QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tabSpectrumWidget->getIndicator());
		}

		m_tabWidgetsMap.insert(it.value()->get_name(), tabController);

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

QString TabManager::getStationName(const int id)
{
	TabsProperty *t = m_tabsPropertyMap.value(id);
	return t->get_name();
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

}

/// read settings for generated submodules (tabs)
int TabManager::readSettings(const QString& settingsFile)
{
	int count = 0;
	
	QSettings settings(settingsFile, QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	QStringList childKeys = settings.childGroups();
	foreach (const QString &childKey, childKeys)
	{
		settings.beginGroup(childKey);

		TabsProperty *prop = new TabsProperty(this);

		prop->set_id(settings.value("Id", 0).toInt());
		prop->set_name(settings.value("Name", 0).toString());
		prop->set_latitude(settings.value("Latitude", "0").toDouble());
		prop->set_longitude(settings.value("Longitude", "0").toDouble());
		prop->set_ip_prm300(settings.value("IPprm300", "127.0.0.1").toString());
		prop->set_ip_adc(settings.value("IPADC", "127.0.0.1").toString());
		prop->set_port_adc(settings.value("portADC", 1030).toInt());

		m_tabsPropertyMap.insert(settings.value("Id", 0).toInt(), prop);
		settings.endGroup();
		count++;
	}

	return count;
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


void TabManager::onSettingsNodeChanged(const SettingsNode &)
{
}

void TabManager::onPropertyChanged(const Property & property)
{
	/*Property inProperty = property;

	TypeCommand commandType = TypeUnknownCommand;

	int commandCode = 0;

	if(DB_FREQUENCY_PROPERTY == inProperty.name) {
		commandCode = COMMAND_PRM_SET_FREQ;
		commandType = TypeGraphicCommand;
	} else if(DB_LEADING_OP_PROPERTY == inProperty.name) {
		commandCode = COMMAND_FLAKON_SET_MAIN_STATION_COR;
		commandType = TypeGraphicCommand;
	} else if(DB_AVERAGING_PROPERTY == inProperty.name) {
		commandCode = COMMAND_FLAKON_SET_AVARAGE;
		commandType = TypeGraphicCommand;
	} else if(DB_PANORAMA_START_PROPERTY == inProperty.name) {
		commandCode = COMMAND_SET_PANORAMA_START_VALUE;
		commandType = TypePanoramaCommand;
	} else if(DB_PANORAMA_END_PROPERTY == inProperty.name) {
		commandCode = COMMAND_SET_PANORAMA_END_VALUE;
		commandType = TypePanoramaCommand;
	}

	if (0 == commandCode) {
		return;
	}

	CommandMessage *msg = new CommandMessage(commandCode, property.value);

	QString stationName = m_dbManager->getObjectName(property.pid);

	/// TODO: update
	sendCommand(stationName, commandType, msg);*/
}

void TabManager::onCleanSettings()
{
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
