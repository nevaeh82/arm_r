#include "TabManager.h"

#include <QDebug>

#include "CommonSpectrumTabWidget.h"
#include "TabSpectrumWidgetController.h"

TabManager::TabManager(QTabWidget *tabWidget, QObject *parent):
	QObject(parent)
{
	m_tabWidget = tabWidget;

	_common_spectra = NULL;
	_common_correlations = NULL;
}

TabManager::~TabManager()
{
	///TODO: recheck
	//disconnect(this, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));

	if(_map_settings.count() > 0)
	{
		QMap<int, TabsProperty *>::iterator it;
		for(it = _map_settings.begin(); it != _map_settings.end(); ++it)
		{
			delete it.value();
		}
	}
	if(m_tabWidgetsMap.count() > 0)
	{
		QMap<QString, ITabWidget*>::iterator its;
		for(its = m_tabWidgetsMap.begin(); its != m_tabWidgetsMap.end(); ++its)
		{
			delete its.value();
		}
	}

	if(_common_spectra != NULL)
	{
		delete _common_spectra;
	}

	if(_common_correlations != NULL)
	{
		delete _common_correlations;
	}
}

int TabManager::start()
{
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changeTabSlot(int)));

	changeTabSlot(m_tabWidget->currentIndex());

	return 0;
}

int TabManager::createSubModules(const QString& settingsFile)
{
	_common_spectra = new CommonSpectra();
	_common_correlations = new CommonCorrelations();

	int submodulesCount = readSettings(settingsFile);

	/// create common database manager for spectrum tabs
	_db_manager_spectrum = new DBManager(this);

	/// create common model for spectrum tabs
	QStringList headers;
	headers << tr("Name") << tr("Property");
	_model_spectrum = new TreeModel(headers);

	_db_manager_spectrum->set_model(_model_spectrum);
	_model_spectrum->set_db(_db_manager_spectrum);

	_common_correlations->init(_map_settings.count() - 1);

	QMap<int, TabsProperty* >::iterator it;
	for(it = _map_settings.begin(); it != _map_settings.end(); ++it)
	{
		TabSpectrumWidgetController* tabController =  new TabSpectrumWidgetController(it.value(), _common_spectra, _common_correlations, _model_spectrum, _db_manager_spectrum, this);
		TabSpectrumWidget* tabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);

		tabController->appendView(tabSpectrumWidget);

		int index = m_tabWidget->addTab(tabSpectrumWidget, it.value()->get_name());

		QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tabSpectrumWidget->getIndicator());
		}

		m_tabWidgetsMap.insert(it.value()->get_name(), tabController);
	}

	checkStatus();

	CommonSpectrumTabWidget* commonTabSpectrumWidget = new CommonSpectrumTabWidget(m_tabWidget);
	commonTabSpectrumWidget->setCorrelationComponent(_common_correlations);

	/// FOR FUTURE
	//	TabSpectrumWidgetController* commonTabController =  new TabSpectrumWidgetController(it.value(), _common_spectra, _common_correlations, _model_spectrum, _db_manager_spectrum, this);
	//	TabSpectrumWidget* commonTabSpectrumWidget = new TabSpectrumWidget(m_tabWidget);
	//	commonTabController->appendView(commonTabSpectrumWidget);

	foreach(ITabWidget* widget, m_tabWidgetsMap) {

		if (NULL != widget->getSpectrumWidget()) {
			commonTabSpectrumWidget->insertSpectrumWidget(widget->getSpectrumWidget());
		}
	}

	commonTabSpectrumWidget->deactivate();

	int index = m_tabWidget->addTab(commonTabSpectrumWidget, tr("Common"));

	QString tabName = m_tabWidget->tabText(index);
	m_tabWidgetsMap.insert(tabName, commonTabSpectrumWidget);


	AtlantTabWidget* atlant = new AtlantTabWidget(m_tabWidget);

	m_tabWidget->addTab(atlant, tr("Atlant"));

	connect(this, SIGNAL(changeTabSignal(int)), this, SLOT(changeTabSlot(int)));

	return submodulesCount;
}

QString TabManager::getStationName(int id)
{
	m_mutex.lock();
	TabsProperty *t = _map_settings.value(id);
	m_mutex.unlock();
	return t->get_name();
}

/// call this method when data in tree has changed
void TabManager::send_data(int pid, TypeCommand type, IMessage *msg)
{
	QString tabName =m_tabWidget->tabText(pid);

	ITabWidget* tabController = m_tabWidgetsMap.value(tabName, NULL);

	if (NULL == tabController) {
		return;
	}

	//TabSpectrumWidget* tab_sp = static_cast<TabSpectrumWidget* >(m_tabWidget->widget(pid));

	TabSpectrumWidgetController* tabController1 = static_cast<TabSpectrumWidgetController*>(tabController);
	tabController1->set_command(type, msg);
}

void TabManager::set_tab(int id)
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


	///TODO: update
	//_current_tab_widget = static_cast<TabSpectrumWidget* >(m_tabWidget->widget(index));


	//_current_tab_widget->activate();

	//TabsProperty *prop = _current_tab_widget->get_tab_property();

	//_model_spectrum->fill_model(prop->get_id());

	TabSpectrumWidgetController* currentWidgetController = static_cast<TabSpectrumWidgetController* >(m_currentTabWidget);
	TabsProperty *prop = currentWidgetController->get_tab_property();

	if (m_currentTabWidget->getWidgetType() != TypeCommonSpectrum) {
		_model_spectrum->fill_model(prop->get_id());
	}

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

		_map_settings.insert(settings.value("Id", 0).toInt(), prop);
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
