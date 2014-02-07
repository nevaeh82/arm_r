#include "arm_r_view.h"
#include "ui_arm_r_view.h"

#include <QDebug>

Pw::Logger::ILogger* ARM_R_view::m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(ARM_R_view));

ARM_R_view::ARM_R_view(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ARM_R_view)
{
    ui->setupUi(this);

    m_logger->info(QString("Started"));

    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaTypeStreamOperators<QByteArray>("QByteArray");
    qRegisterMetaType<quint32>("quint32");
    qRegisterMetaTypeStreamOperators<quint32>("quint32");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");


	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    this->setWindowTitle(tr("Завируха"));

    _layout = new QGridLayout();

    _control_panel = new ControlPanel(this);
    _layout->addWidget(_control_panel);

    _tab_manager = new TabManager(this);
    _layout->addWidget(_tab_manager);

    ui->centralwidget->setLayout(_layout);

    QString tabs_setting_file = QCoreApplication::applicationDirPath();
    tabs_setting_file.append("/Tabs/Tabs.ini");
    int count_tabs = _tab_manager->createSubModules(tabs_setting_file);

    this->showMaximized();

    _tab_manager->start();

    _create_actions();
    _create_menu();

#ifdef QT_DEBUG
	_serviceNames.append("./ARM_R_Serverd");
#else
	_serviceNames.append("./ARM_R_Server");
#endif

    _start_services();
}

ARM_R_view::~ARM_R_view()
{
    delete _control_panel;
    delete ui;
}

void ARM_R_view::_create_menu()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(exitAct);

    settingsMenu = menuBar()->addMenu(tr("&Настройки"));
    settingsMenu->addAction(diagPelengatorAct);
}

void ARM_R_view::_start_services()
{
    m_logger->debug("onStartServices >>>");

    foreach (QString serviceName, _serviceNames)
    {
        _serviceList.append(new ServiceHandler(serviceName, QStringList(), NULL, this));
    }

}

void ARM_R_view::_create_actions()
{
    newAct = new QAction(tr("&Новый"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Новый"));
    //connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("В&ыход"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Выйти из приложения"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
//
    diagPelengatorAct = new QAction(tr("&Атлант"), this);
    diagPelengatorAct->setShortcuts(QKeySequence::Preferences);
    diagPelengatorAct->setStatusTip(tr("Атлант (настройки)"));
//        connect(diagPelengatorAct, SIGNAL(triggered()), _diag_txt, SLOT(slotShow()));

}
