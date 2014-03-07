#include "TCPClient.h"
#include "assert.h"


TCPClient::TCPClient(QString host, quint16 port, IRouter *router)
    : _should_stop(false),
      _isRead(false),
      _state(false),
      _id(-1),
      _type(-1),
      _is_connected(false),
      _parser(NULL)
{
    _host = host;
    _port = port;

    _subscriber = router->get_subscriber();

    connect(this, SIGNAL(signalSend(QByteArray)), this, SLOT(_slotWrite(QByteArray)));
}

TCPClient::~TCPClient()
{
    qDebug() <<"TCP client Destructor";
    if(_parser != NULL)
    {
        _parser->distruct();
    }
}

void TCPClient::set_parser(ITCPParser *parser)
{
    _parser = parser;
}

void TCPClient::proccess()
{
    this->socket_ = new QTcpSocket(this);
//    connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(_slot_stateCh(QAbstractSocket::SocketState)))
    this->connect(socket_, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    this->connect(socket_, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    this->connect(socket_, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    this->connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError))/*, Qt::DirectConnection*/);
    this->connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slotState(QAbstractSocket::SocketState)));
    this->connect(this, SIGNAL(signalReconnection()), this, SLOT(reconnection()), Qt::QueuedConnection);
    this->socket_->connectToHost(_host, _port);
    if(!socket_->waitForConnected(1000))
    {
        QString thiserror;
        thiserror.append(("������! ������� ����� �������� ����������!"));
        emit error(thiserror);
        finish_connection();
    }
}

void TCPClient::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
    QString thiserror;
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        thiserror.append(tr("������! ���������� � ������� ��������!"));
        return;
        break;
    case QAbstractSocket::HostNotFoundError:
        thiserror.append(tr("������! �� ������� ������������ � ������!"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        thiserror.append(tr("������! �������� � ����������"));
        break;
    default:
        thiserror.append((tr("������! ��������� ������: ") + socket_->errorString()));
        break;
    }
    QTextStream(stdout) << thiserror << endl;
    finish_connection();
}

void TCPClient::slotState(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
    {
        emit signalReconnection();
    }
    if(state == QAbstractSocket::ConnectedState)
    {
        qDebug() << "l";
    }
}

void TCPClient::onSocketConnected()
{
    _mutex.lock();
    _is_connected = true;
    _mutex.unlock();

    _state = true;
    emit connected();
    emit signalConnected(true);

    socket_->setReadBufferSize(2000000000);

    if(_id == 0)
    {
        QString request = "user link command ctrl";
        QByteArray ba(request.toAscii());
        this->set(ba);
    }
}

void TCPClient::onSocketDisconnected()
{
    _mutex.lock();
    _is_connected = false;
    _mutex.unlock();
    qDebug() << "Disconnected";
    _state = false;
    emit signalConnected(false);

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
//    emit signalPrepareToWrite(msg_ptr);
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
    qDebug() << "stop";
    socket_->disconnectFromHost();
}

void TCPClient::slotDestroy()
{
    stop();
    emit finished();
}

///called then closed connection
void TCPClient::finish_connection()
{
    emit signalConnected(false);
}

void TCPClient::reconnection()
{
    if(socket_->state() != QAbstractSocket::UnconnectedState)
        return;

    socket_->reset();
    if(!socket_->isValid())
    {
        socket_->disconnectFromHost();
        qDebug() << "socket_->isNOTValid()";
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

    if(_parser != NULL)
    {
        _parser->parsing(data);
    }
}

void TCPClient::_slotWrite(QByteArray data)
{
    int writesize = -1;
    bool connected = false;
    do
    {
        _mutex.lock();
        if(!_is_connected)
        {
            _mutex.unlock();
            return;
        }
        _mutex.unlock();

        writesize = socket_->write(data, data.size());
        socket_->flush();
        qDebug() << "wrote size = " << writesize;
    }while(writesize == -1);
}

void TCPClient::set(QByteArray data)
{
    if(socket_->state() == QAbstractSocket::UnconnectedState)
        return;
    emit signalSend(data);
}

void TCPClient::distruct()
{
    delete this;
}