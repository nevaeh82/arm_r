#include "TabManager.h"

#include <QDebug>

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
	if(_map_tabs.count() > 0)
	{
		QMap<int, TabSpectrumWidget*>::iterator its;
		for(its = _map_tabs.begin(); its != _map_tabs.end(); ++its)
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

	_current_tab_widget  = static_cast<TabSpectrumWidget* >(m_tabWidget->currentWidget());
	_current_tab_widget->start();
	//    _map_settings.value(this->currentIndex());
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
		TabSpectrumWidget* tab_sp = new TabSpectrumWidget(it.value(), _common_spectra, _common_correlations, _model_spectrum, _db_manager_spectrum, this);
		int index = m_tabWidget->addTab(tab_sp, it.value()->get_name());

		/// TODO: update
		//m_tabWidget->tabBar()->setTabButton(i++, QTabBar::LeftSide, tab_sp->get_indicator());
		QTabBar* tabBar = m_tabWidget->findChild<QTabBar *>(QLatin1String("qt_tabwidget_tabbar"));

		if (tabBar != NULL) {
			tabBar->setTabButton(index, QTabBar::LeftSide, tab_sp->get_indicator());
		}

		_map_tabs.insert(it.key(), tab_sp);
	}

	checkStatus();

	TabAtlant* atlant = new TabAtlant(/*_map_settings.value(6)*/);

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
void TabManager::send_data(int pid, IMessage *msg)
{
	TabSpectrumWidget* tab_sp = static_cast<TabSpectrumWidget* >(m_tabWidget->widget(pid));
	tab_sp->set_command(msg);
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
	if(index > 5)
	{
		return;
	}
	_current_tab_widget->stop();

	_current_tab_widget = static_cast<TabSpectrumWidget* >(m_tabWidget->widget(index));
	_current_tab_widget->start();
	TabsProperty *prop = _current_tab_widget->get_tab_property();

	_model_spectrum->fill_model(prop->get_id());
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

		TabsProperty *prop = new TabsProperty();

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
	QMap<int, TabSpectrumWidget* >::iterator it;
	for(it = _map_tabs.begin(); it != _map_tabs.end(); ++it)
	{
		it.value()->check_status();
	}
}
