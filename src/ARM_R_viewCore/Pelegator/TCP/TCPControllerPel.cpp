#include "TCPControllerPel.h"

TCPControllerPel::TCPControllerPel()
    : _is_started(false)
{
}

TCPControllerPel::~TCPControllerPel()
{
    qDebug() << "stop controller";
    if(_sockets_container.count() != 0)
    {
        delete_all_connections();
    }
}

void TCPControllerPel::delete_all_connections()
{
    finished_connections();
//    QHash<QString, TCPClient *>::iterator it;
//    for(it = _sockets_container.begin(); it != _sockets_container.end(); ++it)
//    {
//        (it.value())->stop();
//    }
}

bool TCPControllerPel::start()
{
    _is_started = true;
    return true;
}

bool TCPControllerPel::add_connection(QString host, quint16 port)
{
    try
    {
        QString sig_con = host + ":" + QString::number(port);

        if(_sockets_container.contains(sig_con))
        {
            QString error = QObject::tr("���������� ") + host + ":" + port + QObject::tr("��� ����������!");
            emit add_error(error);
            return false;
        }

        QThread *thread = new QThread;
        TCPClientPel *client = new TCPClientPel(host, port);

//        qDebug() << client;

//        connect(client, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));
        connect(thread, SIGNAL(started()), client, SLOT(proccess()));
        connect(client, SIGNAL(finished()), thread, SLOT(quit()));
        connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));

        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(this, SIGNAL(disconnectfromhost()), client, SLOT(stop()));
        connect(this, SIGNAL(finished_connections()), client, SLOT(slotDestroy()));

        _sockets_container.insert(sig_con, client);

        client->moveToThread(thread);
        thread->start();
    }
    catch(...)
    {
        qDebug() << "Error controller";
        return false;
    }

    return true;
}

void TCPControllerPel::stop()
{
    if(_is_started == true)
    {
//        QHashIterator<QString, TCPClient *> i(_sockets_container);
//        while(i.hasNext())
//        {

//        }
        emit disconnectfromhost();
        _is_started = false;
    }
}

int TCPControllerPel::count_connections()
{
    return _sockets_container.size();
}

void TCPControllerPel::stopped_error()
{
    _is_started = false;
    emit error_connection();
}

TCPClientPel *TCPControllerPel::get_client(QString host, quint16 port)
{
    QString sig_con = host + ":" + QString::number(port);
    if(_sockets_container.contains(sig_con))
    {
        return _sockets_container.value(sig_con);
    }
    return NULL;
}

void TCPControllerPel::errorHandler(QString err)
{
//    _is_started = false;
}