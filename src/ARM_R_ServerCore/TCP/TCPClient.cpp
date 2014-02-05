#include "TCPClient.h"
#include "assert.h"

Pw::Logger::ILogger* TCPClient::_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TCPClient));


TCPClient::TCPClient(QString host, quint16 port, IRouter *router)
    : _should_stop(false),
      _isRead(false),
      _state(false),
      _id(-1),
      _type(-1),
      _number(0),
      _is_connected(false),
      _flakon_coordinator(NULL)
{
    _host = host;
    _port = port;

    _router = router;

    QThread *thread;

    _subscriber = router->get_subscriber();
    _read_settings();
    switch(_type)
    {
    case 0:
        break;
    case 1:
        _parser = new ParserFlakon(_router);
        _subscriber->add_subscription(RPC_MAIN_STATION_COR, this);
        _subscriber->add_subscription(RCP_COMMAND_RECOGNIZE, this);
        _subscriber->add_subscription(RCP_SHIFT_TYPE, this);
        _subscriber->add_subscription(RCP_BANDWIDTH_TYPE, this);
        _subscriber->add_subscription(RPC_SS_CORRELATION, this);
        _subscriber->add_subscription(RPC_AVARAGE_SPECTRUM, this);

        thread = new QThread;

        _flakon_coordinator = new FlakonCoordinateCounter(_router);
        _flakon_coordinator->set_id(101);
        _flakon_coordinator->set_type(1);

        connect(thread, SIGNAL(started()), _flakon_coordinator, SLOT(initSolver()));
        connect(_flakon_coordinator, SIGNAL(signalFinished()), thread, SLOT(quit()));
        connect(this, SIGNAL(finished()), _flakon_coordinator, SLOT(deleteLater()));

        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        _flakon_coordinator->moveToThread(thread);
        thread->start();


        _subscriber->add_subscription(FLAKON_CORRELATION, _flakon_coordinator);

        _subscriber->add_subscription(SET_SOLVER, _flakon_coordinator);
        _subscriber->add_subscription(SET_SOLVER_CLEAR, _flakon_coordinator);

        _subscriber->add_subscription(REQUEST_STATUS, this);

        break;
    case 2:
        _parser = new ParserPRM300(_router, _id);
        _subscriber->add_subscription(PRM_SET_FREQ, this);
        _subscriber->add_subscription(PRM_REQUEST_FREQ, this);
        _subscriber->add_subscription(PRM_SET_ATT1, this);
        _subscriber->add_subscription(PRM_SET_ATT2, this);
        _subscriber->add_subscription(PRM_SET_FILTER, this);
        _subscriber->add_subscription(REQUEST_STATUS, this);

        break;

    ///Atlant
    case 3:
        _parser = new ParserAtlant(_router);

        thread = new QThread;

        connect(dynamic_cast<ParserAtlant *>(_parser), SIGNAL(signalFinished()), thread, SLOT(quit()));
        connect(this, SIGNAL(finished()), dynamic_cast<ParserAtlant *>(_parser), SLOT(deleteLater()));

        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        dynamic_cast<ParserAtlant *>(_parser)->moveToThread(thread);
        thread->start();

        _subscriber->add_subscription(ATLANT_SET_FREQ, this);


//        _subscriber->add_subscription(ATLANT_DIRECTION, _flakon_coordinator);
        break;
    default:
        break;
    }

    connect(this, SIGNAL(signalSend(QByteArray)), this, SLOT(_slotWrite(QByteArray)));
    connect(this, SIGNAL(signalPrepareToWrite(QSharedPointer<IMessage>)), this, SLOT(_slotPrepareToWrite(QSharedPointer<IMessage>)));
}

TCPClient::~TCPClient()
{
    QTextStream(stdout) << "TCP client Destructor" << endl;

    if(_flakon_coordinator != NULL)
    {
        delete _flakon_coordinator;
    }
    _parser->distruct();
}

void TCPClient::proccess()
{
    this->socket_ = new QTcpSocket(this);
    this->connect(socket_, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    this->connect(socket_, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    this->connect(socket_, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    this->connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError))/*, Qt::DirectConnection*/);
    this->connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slotState(QAbstractSocket::SocketState)));
    this->connect(this, SIGNAL(signalReconnection()), this, SLOT(reconnection()), Qt::QueuedConnection);
    this->socket_->connectToHost(_host, _port);
    if(!socket_->waitForConnected(100))
    {
        QString thiserror;
        thiserror.append(("Ошибка! Истекло время ожидания подключния!"));
        emit error(thiserror);
        finish_connection();
    }
}

void TCPClient::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
//    QTextStream(stdout) << this << endl;
//    QTextStream(stdout) << "ERROR CONNECTION!" << _host << ":" << _port << endl;

    QString thiserror;
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        thiserror.append(("Ошибка! Соеденение с пунктом потеряно!"));
        return;
        break;
    case QAbstractSocket::HostNotFoundError:
        thiserror.append(("Ошибка! Не удалось подключиться к пункту!"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        thiserror.append(("Ошибка! Отказано в соединении"));
        break;
    default:
        thiserror.append(("Ошибка! Произошла ошибка: " + socket_->errorString()));
        break;
    }
//    QTextStream(stdout) << thiserror << endl;

    this->_send_status(false);
    finish_connection();
}

void TCPClient::slotState(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
    {
        emit signalReconnection();
    }
}

void TCPClient::onSocketConnected()
{
    _mutex.lock();
    _is_connected = true;
    _mutex.unlock();

    switch(_type)
    {
    case 1:
        break;
    case 2:
        emit signalSend(_prm_get_freq());

        break;
    default:
        break;
    }

    _state = true;
    emit connected();
    emit signalPing(true);
    QTextStream(stdout) << "connected" << endl;
    socket_->setReadBufferSize(2000000000);

    this->_send_status(true);
}

void TCPClient::onSocketDisconnected()
{
    _mutex.lock();
    _is_connected = false;
    _mutex.unlock();
    QTextStream(stdout) << "Disconnected" << endl;

    _state = false;
    this->_send_status(true);

    emit signalPing(false);
    finish_connection();
}

/// get connection state
bool TCPClient::get_state()
{
    return _state;
}

void TCPClient::set_id(int id)
{
    _id = id;
}

void TCPClient::send_data(QSharedPointer<IMessage> msg_ptr)
{    
    emit signalPrepareToWrite(msg_ptr);
}

void TCPClient::_slotPrepareToWrite(QSharedPointer<IMessage> msg_ptr)
{

    int type1 = 1;
    int id = 0;
    IMessage *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream streamprm(dd, QIODevice::ReadOnly);

    QByteArray ba;

    if(type1 == REQUEST_STATUS)
    {
        _send_status(_state);
        return;
    }



    unsigned short avalue;
    int int_value = 0;
    if(_type == 2)
    {
        switch(type1)
        {
        case PRM_SET_FREQ:
            streamprm >> avalue;
            ba = _prm_set_freq(avalue);
            emit signalSend(ba);
            break;
        case PRM_REQUEST_FREQ:
            emit signalSend(_prm_get_freq());
            break;
        case PRM_SET_ATT1:
            streamprm >> int_value;
            ba = _prm_set_att1(int_value);
            emit signalSend(ba);
            break;
        case PRM_SET_ATT2:
            streamprm >> int_value;
            ba = _prm_set_att2(int_value);
            emit signalSend(ba);
            break;
        case PRM_SET_FILTER:
            streamprm >> int_value;
            ba = _prm_set_filter(int_value);
            emit signalSend(ba);
            break;
        default:
            return;
            break;

        }

//        emit signalSend(*dd);
        return;
    }
    if(_type == 3)
    {
        switch(type1)
        {
        case ATLANT_SET_FREQ:
            ba = _atlant_set_freq(dd);
            emit signalSend(ba);
            break;
        default:
            break;
        }
        return;
    }

    _header.magic = 0xffeeccff;
    _header.number = _number;
    _header.id = 0;//_id;
    _header.flags = 0;
    _header.timestamp = 0;
    _header.type = -1;




//    float data = 0;
//    int dev_id = 0;
    _header.type = type1;
    _header.length = dd->size();
    QTextStream(stdout) << "length" << _header.length << endl;

    CRCs crc;
    _header.messageCRC = crc.crc16(reinterpret_cast<unsigned char *>(dd->data()), dd->length());
    _header.headerCRC = crc.crc8(reinterpret_cast<unsigned char *>(&_header), sizeof(ZaviruhaPayloadPacketHeader) - sizeof(short));
//    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);

    stream << _header.magic;
    stream << _header.number;
    stream << _header.id;
    stream << _header.flags;
    stream << _header.timestamp;
    stream << _header.type;
    stream << _header.length;
    stream << _header.reserved;
    stream << _header.messageCRC;
    stream << _header.headerCRC;


    if(type1 == RPC_MAIN_STATION_COR)
    {
        int poin;
        streamprm >> poin ;
        QString s(*dd);
        qDebug() << "poin" << poin << s;

        stream << poin;
    }else
    if(type1 == RPC_AVARAGE_SPECTRUM)
    {
        int average;
        streamprm >> average ;
        qDebug() << "average" << average;
        stream << average;
    }
    else
    {
        ba.append(*dd);
    }
    emit signalSend(ba);
}

QByteArray TCPClient::_prm_get_freq()
{
    QByteArray ba;
    QDataStream streamWrite(&ba, QIODevice::WriteOnly);

    quint8 aStart, aAddr, aSize, aBodyType, aCrc;

    //Запрос о текущей частоте
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1;            //0x01
    aBodyType=4;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    CRCs crc;
    aCrc = crc.calcCRC(aForCrcCount);

    streamWrite << aStart << aAddr << aSize << aBodyType << aCrc;

    return ba;
}

QByteArray TCPClient::_prm_set_att1(int value)
{
    quint8 aValue = static_cast<quint8>(value);
    QByteArray ba;
    QDataStream streamWrite(&ba, QIODevice::WriteOnly);
    quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
    quint8 aSpecialB;

    //Установка аттенюатора 1
    aStart = 170;         //0xAA
    aAddr = 255;          //0xFF
    aSize = 1+1;
    aBodyType = 3;
    aBodyData = aValue;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aForCrcCount.append(aBodyData);
    CRCs crc;
    aCrc = crc.calcCRC(aForCrcCount);

    streamWrite << aStart << aAddr << aSize << aBodyType << aBodyData;

    //Проверка специальных символов
    if (_specialSymb(aCrc, aSpecialB))
    {
        streamWrite << aCrc << aSpecialB;
    }
    else
    {
        streamWrite << aCrc;
    }
    return ba;
}

QByteArray TCPClient::_prm_set_att2(int value)
{
    quint8 aValue = static_cast<quint8>(value);
    QByteArray ba;
    QDataStream streamWrite(&ba, QIODevice::WriteOnly);

    quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
    quint8 aSpecialB;

    //Установка аттенюатора 2
    aStart = 170;         //0xAA
    aAddr = 255;          //0xFF
    aSize = 1+1;
    aBodyType = 7;
    aBodyData = aValue;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aForCrcCount.append(aBodyData);
    CRCs crc;
    aCrc = crc.calcCRC(aForCrcCount);

    streamWrite << aStart << aAddr << aSize << aBodyType << aBodyData;

    //Проверка специальных символов
    if (_specialSymb(aCrc, aSpecialB))
    {
        streamWrite << aCrc << aSpecialB;
    }
    else
    {
        streamWrite << aCrc;
    }
    return ba;
}

QByteArray TCPClient::_prm_set_filter(int value)
{
    quint8 aValue = static_cast<quint8>(value);
    QByteArray ba;
    QDataStream streamWrite(&ba, QIODevice::WriteOnly);

    quint8 aStart, aAddr, aSize, aBodyType, aBodyData, aCrc;
    quint8 aSpecialB;

    //Установка фильтра
    aStart = 170;         //0xAA
    aAddr = 255;          //0xFF
    aSize = 1+1;
    aBodyType = 6;
    aBodyData = aValue;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);
    aForCrcCount.append(aBodyData);
    CRCs crc;
    aCrc = crc.calcCRC(aForCrcCount);

    streamWrite << aStart << aAddr << aSize << aBodyType << aBodyData;

    //Проверка специальных символов
    if (_specialSymb(aCrc, aSpecialB))
    {
        streamWrite << aCrc << aSpecialB;
    }
    else
    {
        streamWrite << aCrc;
    }
    return ba;
}

QByteArray TCPClient::_prm_set_freq(unsigned short aFreq)
{
    QByteArray ba;
    QDataStream streamWrite(&ba, QIODevice::WriteOnly);
    quint8 aStart, aAddr, aSize, aBodyType, aCrc, aFreqFirst, aFreqLast, aTemp;
    quint8 aSpecialB;
    aTemp=0;

    //РЈСЃС‚Р°РЅРѕРІРєР° С‡Р°СЃС‚РѕС‚С‹
    aStart=170;         //0xAA
    aAddr=255;          //0xFF
    aSize=1+2;
    aBodyType=2;

    QVector<quint8> aForCrcCount;
    aForCrcCount.append(aAddr);
    aForCrcCount.append(aSize);
    aForCrcCount.append(aBodyType);

    aFreqFirst=((aFreq>>8)&0xFF);
    aFreqLast=(aFreq&0xFF);

    aForCrcCount.append(aFreqFirst);
    aForCrcCount.append(aFreqLast);
    CRCs crc;
    aCrc = crc.calcCRC(aForCrcCount);

    streamWrite<<aStart<<aAddr<<aSize<<aBodyType;

    //РџСЂРѕРІРµСЂРєР° СЃРїРµС†РёР°Р»СЊРЅС‹С… СЃРёРјРІРѕР»РѕРІ
    if (_specialSymb(aFreqFirst, aSpecialB)) streamWrite<<aFreqFirst<<aSpecialB;
    else streamWrite<<aFreqFirst;

    if (_specialSymb(aFreqLast, aSpecialB)) streamWrite<<aFreqLast<<aSpecialB;
    else streamWrite<<aFreqLast;

    if (_specialSymb(aCrc, aSpecialB)) streamWrite<<aCrc<<aSpecialB;
    else streamWrite<<aCrc;

    return ba;
}

bool TCPClient::_specialSymb(quint8 &aByteA, quint8 &aByteB)
{
    bool aTemp=false;
    if (aByteA==0x5A)
    {
        aByteA=0x5A;
        aByteB=0x00;
        aTemp=true;
    }
    else if (aByteA==0x55)
    {
        aByteA=0x5A;
        aByteB=0x01;
        aTemp=true;
    }
    else if (aByteA==0xA5)
    {
        aByteA=0x5A;
        aByteB=0x02;
        aTemp=true;
    }
    else if (aByteA==0xAA)
    {
        aByteA=0x5A;
        aByteB=0x03;
        aTemp=true;
    }
    return aTemp;
}

int TCPClient::get_id()
{
    return _id;
}

void TCPClient::set_type(int type)
{
    _type = type;
}

int TCPClient::get_type()
{
    return _type;
}

void TCPClient::stop()
{
    QTextStream(stdout) << "stop" << endl;

    socket_->disconnectFromHost();
//    write_timer->stop();
}

void TCPClient::slotDestroy()
{
    stop();
    emit finished();
}

///called then closed connection
void TCPClient::finish_connection()
{
    emit signalPing(false);
//    reconnection();
}

void TCPClient::reconnection()
{
    if(socket_->state() != QAbstractSocket::UnconnectedState)
        return;

    socket_->reset();
    if(!socket_->isValid())
    {
        socket_->disconnectFromHost();
        QTextStream(stdout) << "socket_->isNOTValid()" << endl;

    }
    this->socket_->connectToHost(_host, _port);
    this->socket_->waitForConnected(5000);
}

/// ready to read
void TCPClient::onSocketReadyRead()
{
    if(socket_->bytesAvailable() < 1)
        return;

    QByteArray data = socket_->readAll();

    _parser->parsing(data);
}


void TCPClient::writing(QByteArray data)
{
    try
    {
        QMutexLocker    locker(&_mutex);
        //onWrite(data, data.size());
    }
    catch(...)
    {
        QTextStream(stdout) << "error writing message!!!" << endl;

    }
}

QByteArray TCPClient::_atlant_set_freq(QByteArray *data)
{
    QDataStream ds(data, QIODevice::ReadOnly);

    int id;
    int post;
    double freq;
    double width;

    ds >> id;
    ds >> post;
    ds >> freq;
    ds >> width;


    qDebug() << id << freq << width;
    EMS::EagleMessage e_msg;
    QString type = "DIRECTION_FINDING_REQUEST";
    QDateTime dt(QDateTime::currentDateTime());
    qint64 d = 0; //datetime
    QString name = "ARM_R";
    QString label = "ARM_R1";
    e_msg.set_type(type.toStdString().c_str(), type.size());
    e_msg.set_sendername(name.toStdString().c_str(), name.size());
    e_msg.set_datetime(d);
    e_msg.add_label(label.toStdString().c_str(), label.size());

    Storm::DirectionFindingRequest d_msg;
    d_msg.set_id(id);
    d_msg.add_frequency(freq);
    d_msg.set_width(width);


//    char* d1 = new char(d_msg.ByteSize());
//    qDebug() << d_msg.SerializeToArray(d1 , d_msg.ByteSize());
    std::string inner = d_msg.SerializeAsString();

    e_msg.set_innermessage(inner);

//    char* d2 = new char(e_msg.ByteSize());
    //qDebug() <<
    std::string message = e_msg.SerializeAsString();

    QByteArray ret;

    qDebug() << message.size();

    //QByteArray bg = QByteArray::number(message.size());
    unsigned int size = message.size();
    ret.append(reinterpret_cast<char *>(&size), sizeof(size));
    ret.append(message.c_str(), message.size());
    //    QByteArray ba = QByteArray::fromRawData(e_msg.innermessage().c_str(), e_msg.innermessage().size());

//    if(type
    return ret;
}

void TCPClient::_send_status(bool state)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << state;

    QSharedPointer<IMessage> msg(new Message(_id, CONNECTION_STATUS, ba));
    _subscriber->data_ready(CONNECTION_STATUS, msg);
}

void TCPClient::_slotWrite(QByteArray data)
{
    int writesize = -1;
    bool connected = false;
    do
    {
//        _mutex.lock();
        if(!_is_connected)
        {
//            _mutex.unlock();
            return;
        }
//        _mutex.unlock();

        writesize = socket_->write(data, data.size());
        socket_->flush();
//        QTextStream(stdout) << "wrote size = " << writesize << endl;

    }while(writesize == -1);
}

void TCPClient::set(QByteArray data)
{
    if(socket_->state() == QAbstractSocket::UnconnectedState)
        return;
    emit signalSend(data);
}

/// read settings from ini file
void TCPClient::_read_settings()
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString app_dir = QCoreApplication::applicationDirPath();
	app_dir.append("./TCP/coders.ini");
    QSettings m_settings(app_dir, QSettings::IniFormat);

	m_settings.setIniCodec(codec);

    QString ip = "127.0.0.1";
    quint16 port = 1024;

    QStringList childKeys = m_settings.childGroups();
    foreach (const QString &childKey, childKeys)
    {
//        QTextStream(stdout) << "m_settings = " << childKey.toLatin1() << endl;
        _logger->info(QString("m_settings = %1").arg(childKey));

        m_settings.beginGroup(childKey);

        ip = m_settings.value("ip", "127.0.0.1").toString();
        if(_host != ip)
        {
            m_settings.endGroup();
            continue;
        }
        port = m_settings.value("Port", 2323).toInt();
        if(_port != port)
        {
            m_settings.endGroup();
            continue;
        }


        _type = m_settings.value("type", "0").toInt();

        this->set_type(_type);

        if(_type < 2)
        {
            m_settings.endGroup();
            continue;
        }

        _name = m_settings.value("name", "").toString();
        _id = m_settings.value("id", "-1").toInt();

        this->set_id(_id);
        m_settings.endGroup();
    }
}
