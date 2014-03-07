#include "arm_od_view.h"
#include "ui_arm_od_view.h"

#include <QDebug>

ARM_OD_view::ARM_OD_view(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ARM_OD_view)
{
    ui->setupUi(this);

    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaType<rpc_QPointF> ("rpc_QPointF");   
    qRegisterMetaType<QMap<int, QVector<QString> > > ("QMap<int, QVector<QString> >");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    this->setWindowTitle(tr("Завируха"));
    QIcon icon("images/zaviruha.png");
    this->setWindowIcon(icon);


    _tab_manager = new TabManager(this);
    _layout = new QGridLayout();
    _layout->addWidget(_tab_manager);
    ui->centralwidget->setLayout(_layout);

    QString tabs_setting_file = QCoreApplication::applicationDirPath();
    tabs_setting_file.append("/Tabs/Tabs.ini");
    int count_tabs = _tab_manager->createSubModules(tabs_setting_file);
//    qDebug() << "count tabs = " << count_tabs;

    _read_settings();

    this->showMaximized();

    _tab_manager->start();

    _solver = new Solver(701, _tab_manager);

    _create_actions();
    _create_menu();


//    _tabs_controller = new TabsController();
//    _tabs_controller->createCommonComponents();
//    _main_tab = new MainTabWidget(this, _map_settings, _diag_db_lists->get_view(), _router);

}

ARM_OD_view::~ARM_OD_view()
{
    delete ui;
}

void ARM_OD_view::_create_menu()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(exitAct);

    mapMenu = menuBar()->addMenu(tr("&Карта"));
    mapMenu->addAction(openMapFromAtlas);
    mapMenu->addAction(openMapFromLocalFile);

    settingsMenu = menuBar()->addMenu(tr("&Настройки"));
    settingsMenu->addAction(solverAction);
//    settingsMenu->addAction(diagDBListAct);
//    settingsMenu->addAction(diagKTRSettings);
}

void ARM_OD_view::_create_actions()
{
    newAct = new QAction(tr("&Новый"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Новый"));
    //connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("В&ыход"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Выйти из приложения"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    openMapFromAtlas = new QAction(tr("&Открыть из атласа"), this);
//    openMapFromAtlas->setShortcuts(QKeySequence::Open);
    openMapFromAtlas->setStatusTip(tr("Открыть карту из атласа"));
    connect(openMapFromAtlas, SIGNAL(triggered()), _tab_manager->get_map_controller(), SLOT(open_map_from_Atlas()));

    openMapFromLocalFile = new QAction(tr("&Открыть из файла"), this);
//    openMapFromLocalFile->setShortcuts(QKeySequence::Open);
    openMapFromLocalFile->setStatusTip(tr("Открыть карту из файла"));
    connect(openMapFromLocalFile, SIGNAL(triggered()), _tab_manager->get_map_controller(), SLOT(open_map_from_local_file()));
//    _slot_change_tab(main_tab->currentIndex());

    solverAction = new QAction(tr("&Координатометрия"), this);
    solverAction->setShortcuts(QKeySequence::Preferences);
    solverAction->setStatusTip(tr("Параметры координатометрии"));
    connect(solverAction, SIGNAL(triggered()), _solver, SLOT(slot_show()));
//    diagMsgServerAct = new QAction(tr("&Сообщения сервера"), this);
//    diagMsgServerAct->setShortcuts(QKeySequence::Preferences);
//    diagMsgServerAct->setStatusTip(tr("Сообщения сервера (флакон)"));
//    connect(diagMsgServerAct, SIGNAL(triggered()), _diag_txt, SLOT(slotShow()));

//    diagDBListAct = new QAction(tr("&Списки"), this);
//    diagDBListAct->setShortcuts(QKeySequence::Preferences);
//    diagDBListAct->setStatusTip(tr("Списки (частот из базы данных)"));
//    connect(diagDBListAct, SIGNAL(triggered()), _diag_db_lists, SLOT(slotShow()));

//    diagKTRSettings = new QAction(tr("&КТР"), this);
//    diagKTRSettings->setShortcuts(QKeySequence::Preferences);
//    diagKTRSettings->setStatusTip(tr("Настройки КТР модема"));
//    connect(diagKTRSettings, SIGNAL(triggered()), _diag_ktr_settings, SLOT(slotShow()));
}

void ARM_OD_view::_read_settings()
{
//    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
//    QString app_dir = QCoreApplication::applicationDirPath();
//    app_dir.append("/Zaviruha8.ini");
//    QSettings m_settings(app_dir, QSettings::IniFormat);

//    m_settings.setIniCodec(codec);
//    QString ip = "127.0.0.1";
//    quint16 port = 1024;

//    QStringList childKeys = m_settings.childGroups();
//    foreach (const QString &childKey, childKeys)
//    {
//        qDebug() << "m_settings = " << childKey.toLatin1();
//        m_settings.beginGroup(childKey);
//        if(childKey.toLatin1() == "ServerM")
//        {
//            ip = m_settings.value("IP", "127.0.0.1").toString();
//            port = m_settings.value("Port", 1024).toInt();
//            m_settings.endGroup();
//            continue;
//        }

//        TabsProperty *prop = new TabsProperty();

//        prop->set_ip_Flakon(ip);
//        prop->set_port_Flakon(port);

//        prop->set_id(m_settings.value("Id", 0).toInt());
//        prop->set_name(m_settings.value("Name", 0).toString());
//        prop->set_ip(m_settings.value("IP", "127.0.0.1").toString());
//        prop->set_port(m_settings.value("Port", 2323).toInt());
//        prop->set_latitude(m_settings.value("Latitude", "0").toDouble());
//        prop->set_longitude(m_settings.value("Longitude", "0").toDouble());

//        _map_settings.insert(m_settings.value("Id", 0).toInt(), prop/*values*/);
//        m_settings.endGroup();
//    }
}
