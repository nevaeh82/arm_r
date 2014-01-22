#include "ARM_R_Srv.h"
#include <QTextStream>

Pw::Logger::ILogger* ARM_R_Srv::_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(ARM_R_Srv));


ARM_R_Srv::ARM_R_Srv()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    qRegisterMetaType<rpc_flakon_msg> ("rpc_flakon_msg");
    qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
    qRegisterMetaType<QByteArray>("rpc_send_atlant_data");

    qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
    qRegisterMetaTypeStreamOperators<QByteArray>("QByteArray");

    qRegisterMetaType<QSharedPointer<IMessage> > ("QSharedPointer<IMessage>");
    qRegisterMetaType<quint32>("quint32");

    qRegisterMetaType<DataFromFlacon> ("DataFromFlacon");
    qRegisterMetaType<DataFromRadioLocation> ("DataFromRadioLocation");


    _map_station_property = new QMap<int, StationProperty* >;


	_router = new Router(this);


	_subscriber_up = new Subscriber(this);

    _router->set_subscriber(_subscriber_up);
	_tcp_controller = new TCPController(this, _router);

    _router->set_tcp_controller(_tcp_controller);


    _read_settings();
    _router->set_station_property(_map_station_property);
    _rpc_server = new RPCServer(_router);

//    _atlant_controller = new AtlantController();


// connect up the signals
//    QObject::connect(_rpc_server, SIGNAL(finished()), &a, SLOT(quit()));
//    QObject::connect(&a, SIGNAL(aboutToQuit()), _rpc_server, SLOT(aboutToQuitApp()));

    _rpc_server->start();

    _pserver = new PServer(10240);
    _subscriber_up->add_subscription(FLAKON_BPLA, _pserver);

    QThread *thread = new QThread;

//        qDebug() << client;

//        connect(client, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
    connect(thread, SIGNAL(started()), _pserver, SLOT(startServer()));
    connect(_pserver, SIGNAL(finished()), thread, SLOT(quit()));
    connect(_pserver, SIGNAL(finished()), _pserver, SLOT(deleteLater()));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    _pserver->moveToThread(thread);
    thread->start();


}

ARM_R_Srv::~ARM_R_Srv()
{
    delete _map_station_property;
    delete _atlant_controller;
    delete _rpc_server;
//    delete _router;
//    delete _subscriber_up;
//    delete _tcp_controller;
}

/// read settings from ini file
void ARM_R_Srv::_read_settings()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString app_dir = QCoreApplication::applicationDirPath();
    app_dir.append("/TCP/coders.ini");
    QSettings m_settings(app_dir, QSettings::IniFormat);

    m_settings.setIniCodec(codec);
    int id = -1;
    QString name = "";
    QString ip = "127.0.0.1";
    quint16 port = 1024;
    double latitude;
    double longitude;
    int geo_altitude;
    int altitude;
    QString trace;
    int wire_length;
    int type;

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
        _logger->info(QString("m_settings = %1").arg(childKey));
        m_settings.beginGroup(childKey);

        ip = m_settings.value("ip", "127.0.0.1").toString();
        port = m_settings.value("Port", 2323).toInt();
        type = m_settings.value("type", -1).toInt();

        id = m_settings.value("id", -1).toInt();

        if(id == -1)
        {
            _tcp_controller->add_connection(ip, port, _router);
        }

        if(id < 0)
        {
            m_settings.endGroup();
            continue;
        }

        name = m_settings.value("name", "").toString();
        latitude = m_settings.value("latitude", 0).toDouble();
        longitude = m_settings.value("longitude", 0).toDouble();
        geo_altitude = m_settings.value("geo_altitude", 0).toInt();
        altitude = m_settings.value("altitude", 0).toInt();
        trace = m_settings.value("trace", "0").toString();
        wire_length = m_settings.value("wire_length", 0).toInt();

        StationProperty* prop = new StationProperty();
        prop->set_id(id);
        prop->set_name(name);
        prop->set_ip(ip);
        prop->set_port(port);
        prop->set_latitude(latitude);
        prop->set_longitude(longitude);
        prop->set_geo_altitude(geo_altitude);
        prop->set_altitude(altitude);
        prop->set_trace(trace);
        prop->set_wire_length(wire_length);
        prop->set_type(type);

        _map_station_property->insert(id, prop);
        m_settings.endGroup();
    }
    _count_distances();
}

void ARM_R_Srv::_count_distances()
{
    QMap<int, StationProperty* >::iterator it;
    for(it = _map_station_property->begin(); it != _map_station_property->end(); ++it)
    {
        it.value()->set_distance_to_center(_map_station_property);
    }
}
