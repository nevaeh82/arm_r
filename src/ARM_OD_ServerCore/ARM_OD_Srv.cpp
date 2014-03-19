#include "ARM_OD_Srv.h"

ARM_OD_Srv::ARM_OD_Srv()
{

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    qRegisterMetaType<rpc_msg> ("rpc_msg");
    qRegisterMetaType<rpc_QPointF> ("rpc_QPointF");
	qRegisterMetaType<QSharedPointer<IMessageOld> > ("QSharedPointer<IMessage>");

    qRegisterMetaType<QByteArray>("rpc_send_ba_data");

    qRegisterMetaTypeStreamOperators<QByteArray>("QByteArray");

    _router = new Router();

    _subscriber_up = new Subscriber();

    _router->set_subscriber(_subscriber_up);
    _tcp_controller = new TCPController(_router);
    _router->set_tcp_controller(_tcp_controller);


    _read_settings();


// connect up the signals
//    QObject::connect(_rpc_server, SIGNAL(finished()), &a, SLOT(quit()));
//    QObject::connect(&a, SIGNAL(aboutToQuit()), _rpc_server, SLOT(aboutToQuitApp()));

    _rpc_server = new RPCServer(_router);
    _rpc_server->start();


	_read_settingsKTR();

    _rpc_client1 = new RPCClient_R(_router);


    QThread *thread_rpc_client = new QThread;
    qDebug() << "create thread for rpc client ";

//    connect(thread_rpc_client, SIGNAL(started()), _rpc_client1, SLOT(slotInit()));

    connect(this, SIGNAL(signalStartRPC()), _rpc_client1, SLOT(slotStart()));
    connect(_rpc_client1, SIGNAL(signalFinished()), thread_rpc_client, SLOT(quit()));
    connect(thread_rpc_client, SIGNAL(finished()), thread_rpc_client, SLOT(deleteLater()));

    connect(_rpc_client1, SIGNAL(signalFinished()), _rpc_client1, SLOT(deleteLater()));
    connect(this, SIGNAL(signalStopRPC()), _rpc_client1, SLOT(slotStop()));
    connect(this, SIGNAL(signalFinishRPC()), _rpc_client1, SLOT(slotFinish()));

    _rpc_client1->setParent(0);
    _rpc_client1->moveToThread(thread_rpc_client);

    _subscriber_up->add_subscription(SOLVER_SET, _rpc_client1);
    _subscriber_up->add_subscription(SOLVER_CLEAR, _rpc_client1);
    thread_rpc_client->start();

//    _rpc_client1->start();

    _niipp1 = new NIIPPController(_router);
    _niipp1->set_id(100);
    _niipp1->set_type(1);
    _niipp1->setController("192.168.10.50", 6340);


    _niipp2 = new NIIPPController(_router);
    _niipp2->set_id(101);
    _niipp2->set_type(1);
    _niipp2->setController("192.168.245.51", 6340);
//    _niipp2 = new NIIPPController(_router);
//    _niipp2->set_id(102);
    emit signalStartRPC();


//    BLASimulator* bla_sim = new BLASimulator(_router, 0.3, true);
//    bla_sim->start();

//    BLASimulator* bpla_sim = new BLASimulator(_router, 0.1, false);
//    bpla_sim->start();

	_ais = new Ais(_router);
//    QThread *thread = new QThread;

//    connect(_ais, SIGNAL(finished()), thread, SLOT(quit()));
//    connect(_ais, SIGNAL(finished()), _ais, SLOT(deleteLater()));

//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    connect(this, SIGNAL(closeALL()), _ais, SLOT(stop()));

//    _ais->moveToThread(thread);
//    thread->start();




//    AtlantSimulator* a_sim = new AtlantSimulator(_router, 42.623, 41.746917/*42.741194, 41.42694*/, 33);
//    a_sim->set_id(778);
//    a_sim->set_type(1);
//    _subscriber_up->add_subscription(KTR_BLA, a_sim);

//    QThread *thread_atlant_sim = new QThread;

//    connect(a_sim, SIGNAL(signalFinished()), thread_atlant_sim, SLOT(quit()));
//    connect(thread_atlant_sim, SIGNAL(finished()), thread_atlant_sim, SLOT(deleteLater()));

//    connect(a_sim, SIGNAL(signalFinished()), a_sim, SLOT(deleteLater()));

//    a_sim->setParent(0);
//    a_sim->moveToThread(thread_atlant_sim);
//    thread_atlant_sim->start();



	TcpManager* m_tcpManager = new TcpManager;
	QThread* tcpManagerThread = new QThread;
	connect(tcpManagerThread, SIGNAL(finished()), m_tcpManager, SLOT(deleteLater()));
	connect(tcpManagerThread, SIGNAL(finished()), tcpManagerThread, SLOT(deleteLater()));
	m_tcpManager->moveToThread(tcpManagerThread);
	tcpManagerThread->start();

//	m_tcpManager->addTcpDevice(NIIPP_TCP_DEVICE, "192.168.10.50", 6340);
//	m_tcpManager->addTcpDevice(NIIPP_TCP_DEVICE, "192.168.245.51", 6340);
//	m_tcpManager->addTcpDevice(KTR_TCP_DEVICE, "192.168.137.98", 64300);

}

ARM_OD_Srv::~ARM_OD_Srv()
{
    signalFinishRPC();
    delete _rpc_server;
    delete _router;
    delete _subscriber_up;
    delete _tcp_controller;
    delete _niipp1;
    delete _niipp2;

    emit closeALL();
}

/// read settings from ini file
void ARM_OD_Srv::_read_settings()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString app_dir = QCoreApplication::applicationDirPath();
    app_dir.append("/TCP/coders.ini");
    QSettings m_settings(app_dir, QSettings::IniFormat);

    m_settings.setIniCodec(codec);
    QString ip = "127.0.0.1";
    quint16 port = 1024;

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        qDebug() << "m_settings = " << childKey.toLatin1();
        m_settings.beginGroup(childKey);

        ip = m_settings.value("ip", "127.0.0.1").toString();
        port = m_settings.value("Port", 2323).toInt();

//        _tcp_controller->add_connection(ip, port, _router);

        m_settings.endGroup();
    }
}

/// read settings from ini file
void ARM_OD_Srv::_read_settingsKTR()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString app_dir = QCoreApplication::applicationDirPath();
    app_dir.append("/KTR/KTR.ini");
    QSettings m_settings(app_dir, QSettings::IniFormat);

    m_settings.setIniCodec(codec);
    QString ip = "127.0.0.1";
    quint16 port = 64300;

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        qDebug() << "m_settings = " << childKey.toLatin1();
        m_settings.beginGroup(childKey);

        _ip_ktr = m_settings.value("IP", "127.0.0.1").toString();
        _port_ktr = m_settings.value("Port", 64300).toInt();

        BLAController* bla_controller = new BLAController(_router);
        bla_controller->setController(_ip_ktr, _port_ktr);

//        _tcp_controller->add_connection(ip, port, _router);

        m_settings.endGroup();
    }
}
