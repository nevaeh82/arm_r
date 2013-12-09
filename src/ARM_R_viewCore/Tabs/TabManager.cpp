#include "TabManager.h"

#include <QDebug>

TabManager::TabManager(QWidget *parent):
    QTabWidget(parent)
{
//    this->setGeometry(QRect(10, 10, 300, 250));
//    _layout = new QVBoxLayout();
//    _layout->alignment();
//    this->setLayout(_layout);
    _common_spectra = NULL;
    _common_correlations = NULL;

//    _model_controller = new Controller();

}

TabManager::~TabManager()
{
    disconnect(this, SIGNAL(currentChanged(int)), this, SLOT(_slot_change_tab(int)));
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
        QMap<int, TabSpectrum *>::iterator its;
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
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(_slot_change_tab(int)));

     _current_tab_widget  = static_cast<TabSpectrum* >(this->currentWidget());
     _current_tab_widget->start();
//    _map_settings.value(this->currentIndex());
    return 0;
}

int TabManager::stop()
{
    return 0;
}

void TabManager::show()
{

}

void TabManager::hide()
{

}

int TabManager::createSubModules(QString path_to_ini_file)
{
    _common_spectra = new CommonSpectra();
    _common_correlations = new CommonCorrelations();

    int count = _read_settings(path_to_ini_file);
    /// create common database manager for spectrum tabs
    _db_manager_spectrum = new DBManager(this);
    /// create common model for spectrum tabs
    QStringList headers;
    headers << tr("Название") << tr("Свойство");
    _model_spectrum = new TreeModel(headers);

    _db_manager_spectrum->set_model(_model_spectrum);
    _model_spectrum->set_db(_db_manager_spectrum);

    _common_correlations->init(_map_settings.count() - 1);

    int i = 0;
    QMap<int, TabsProperty* >::iterator it;
    for(it = _map_settings.begin(); it != _map_settings.end(); ++it)
    {
        TabSpectrum* tab_sp = new TabSpectrum(it.value(), _common_spectra, _common_correlations, _model_spectrum, _db_manager_spectrum, this);
        this->addTab(tab_sp, it.value()->get_name());
        this->tabBar()->setTabButton(i++, QTabBar::LeftSide, tab_sp->get_indicator());
        _map_tabs.insert(it.key(), tab_sp);
    }

    _check_status();

    TabAtlant* atlant = new TabAtlant(/*_map_settings.value(6)*/);

    this->addTab(atlant, tr("Антлант"));

    connect(this, SIGNAL(signalChangeTab(int)), this, SLOT(_slot_change_tab(int)));
    return count;
}

QString TabManager::getStationName(int id)
{
    _mux.lock();
    TabsProperty *t = _map_settings.value(id);
    _mux.unlock();
    return t->get_name();
}

/// call this method when data in tree has changed
void TabManager::send_data(int pid, IMessage *msg)
{
    TabSpectrum* tab_sp = static_cast<TabSpectrum* >(this->widget(pid));
    tab_sp->set_command(msg);
}

void TabManager::set_tab(int id)
{
    emit this->tabBar()->setCurrentIndex(id);
//    emit signalChangeTab(id);
}

/// slot tab change
void TabManager::_slot_change_tab(int index)
{
    if(index > 5)
    {
        return;
    }
    _current_tab_widget->stop();

    _current_tab_widget = static_cast<TabSpectrum* >(this->widget(index));
    _current_tab_widget->start();
    TabsProperty *prop = _current_tab_widget->get_tab_property();

    _model_spectrum->fill_model(prop->get_id());
}

/// read settings for generated submodules (tabs)
int TabManager::_read_settings(QString path_to_ini_file)
{
    int count = 0;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QSettings m_settings(path_to_ini_file, QSettings::IniFormat);

    m_settings.setIniCodec(codec);

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        m_settings.beginGroup(childKey);

        TabsProperty *prop = new TabsProperty();

        prop->set_id(m_settings.value("Id", 0).toInt());
        prop->set_name(m_settings.value("Name", 0).toString());
        prop->set_latitude(m_settings.value("Latitude", "0").toDouble());
        prop->set_longitude(m_settings.value("Longitude", "0").toDouble());
        prop->set_ip_prm300(m_settings.value("IPprm300", "127.0.0.1").toString());
        prop->set_ip_adc(m_settings.value("IPADC", "127.0.0.1").toString());
        prop->set_port_adc(m_settings.value("portADC", 1030).toInt());

        _map_settings.insert(m_settings.value("Id", 0).toInt(), prop);
        m_settings.endGroup();
        count++;
    }

    return count;
}

void TabManager::_check_status()
{
    QMap<int, TabSpectrum* >::iterator it;
    for(it = _map_tabs.begin(); it != _map_tabs.end(); ++it)
    {
        it.value()->check_status();
    }
}
