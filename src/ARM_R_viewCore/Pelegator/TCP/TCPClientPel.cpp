#include "TCPClientPel.h"

TCPClientPel::TCPClientPel(QString host, quint16 port)
    : _should_stop(false),
      _blockSize(0),
      _isRead(false)
{
    _host = host;
    _port = port;

    write_timer = new QTimer(this);
    read_timer = new QTimer(this);
    connect(write_timer, SIGNAL(timeout()), SLOT(writing()));
    connect(read_timer, SIGNAL(timeout()),  SLOT(read_by_timer()));

    write_timer->start(1000);
    read_timer->start(1000);

//    QThread *thread_parser = new QThread;
//    _parser = new TCPParser();
//    qDebug() << "create parser " << port;

////    connect(_parser, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
////    connect(thread_parser, SIGNAL(started()), _parser, SLOT(proccess()));
//    connect(_parser, SIGNAL(signalFinished()), thread_parser, SLOT(quit()));
//    connect(thread_parser, SIGNAL(finished()), thread_parser, SLOT(deleteLater()));

//    connect(_parser, SIGNAL(signalFinished()), _parser, SLOT(deleteLater()));
//    connect(this, SIGNAL(signal_finish_parsing()), _parser, SLOT(slotStop()));

//    connect(this, SIGNAL(signalParsing(QByteArray)), _parser, SLOT(slotParsing(QByteArray)));

//    _parser->moveToThread(thread_parser);
//    thread_parser->start();


//    _income_data = new char[65535];
}

TCPClientPel::~TCPClientPel()
{
    qDebug() <<"TCP client Destructor";
//    stop();
//    delete[] _income_data;
//    delete _income_data;
}

void TCPClientPel::proccess()
{
    this->socket_ = new QTcpSocket(this);
    this->connect(socket_, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    this->connect(socket_, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    this->connect(socket_, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    this->connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError)), Qt::DirectConnection);
    this->connect(this, SIGNAL(signalReconnection()), this, SLOT(reconnection()));
    this->socket_->connectToHost(_host, _port);
    if(!socket_->waitForConnected(1000))
    {
        QString thiserror;
        thiserror.append(("Ошибка! Истекло время ожидания подключния!"));
        emit error(thiserror);
        finish_connection();
        emit signalReconnection();
    }
}

void TCPClientPel::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << this;
    qDebug() << "ERROR CONNECTION!";
    QString thiserror;
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        thiserror.append(("Ошибка! Соеденение с пунктом потеряно!"));
//        if(socket_->state() == QAbstractSocket::ConnectedState)
//        {
//            socket_->disconnectFromHost();
//        }
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
    qDebug() << thiserror;
    //emit error(thiserror);
    finish_connection();
//    stop();
//    qDebug() << thiserror;
//    reconnection();
    emit signalReconnection();
}

void TCPClientPel::onSocketConnected()
{
    emit connected();
//    socket_->setReadBufferSize(50000000);
    qDebug() << "connected";
}

void TCPClientPel::onSocketDisconnected()
{
     qDebug() << "onSocketDisconnected";
    //socket_->disconnectFromHost();
    finish_connection();
    emit signalReconnection();
}

void TCPClientPel::stop()
{
    emit signal_finish_parsing();
    qDebug() << "stop";
    socket_->disconnectFromHost();
    write_timer->stop();
    read_timer->stop();
    //emit finished();

}

void TCPClientPel::slotDestroy()
{
    stop();
    emit finished();
}

///called then closed connection
void TCPClientPel::finish_connection()
{
    emit ping(false);
}

void TCPClientPel::reconnection()
{
    qDebug() << "state = " << socket_->state();
    if(socket_->isValid())
    {
        qDebug() << "socket_->isValid()";
//        this->socket_->disconnectFromHost();
        this->socket_->connectToHost(_host, _port);
        if(!socket_->waitForConnected(1000))
        {
            QString thiserror;
            thiserror.append(("Ошибка! Истекло время ожидания подключния!"));
            emit error(thiserror);
            finish_connection();
            emit signalReconnection();
        }
        else
        {
            qDebug() << "Con";
        }
    }
    else
    {
        socket_->disconnectFromHost();
        qDebug() << "socket_->isNOTValid()";
    }
}

void TCPClientPel::onSocketReadyRead()
{
    _isRead = true;
}

/// choose action via message
void TCPClientPel::_complete_msg(QDataStream &stream)
{

}

void TCPClientPel::writing(QByteArray data)
{
    try
    {
        QMutexLocker    locker(&_mutex);
        onWrite(data, data.size());
    }
    catch(...)
    {
        qDebug() << "error writing message!!!";

    }

}

void TCPClientPel::onWrite(QByteArray data, int size)
{
//    qDebug() << size;
    qDebug() << "Wrote size  =  " << socket_->write(data);
//    socket_->write("tttttttttttttttttttt");
}

void TCPClientPel::read_by_timer()
{
//    emit ping(_isRead);
//    qDebug() << _isRead;
    if(_isRead == false)
    {
        /// need uncomment this in real work
//        onSocketDisplayError(QAbstractSocket::RemoteHostClosedError);
    }
    _isRead = false;
}

/// request data
void TCPClientPel::slotRequestData(unsigned int id, unsigned int type, int *data, unsigned int length)
{
}

