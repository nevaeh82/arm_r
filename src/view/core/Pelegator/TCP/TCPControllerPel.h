#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QHash>

#include "TCPClientPel.h"

class TCPControllerPel : public QObject
{
    Q_OBJECT
public:
    TCPControllerPel();
    ~TCPControllerPel();

public:
    bool start();
    bool add_connection(QString host, quint16 port);
    void stop();
    void delete_all_connections();
    int count_connections();
    TCPClientPel *get_client(QString host, quint16 port);

private:
    bool _is_started;
    QHash<QString, TCPClientPel *> _sockets_container; //QString signature: "host:port"


signals:
    void error_connection();
    void disconnectfromhost();
    void add_error(QString);
    void finished_connections();

public slots:
    void stopped_error();
    void errorHandler(QString err);
};

#endif // TCPCONTROLLER_H
