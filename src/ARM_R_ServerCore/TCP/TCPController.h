#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QHash>

#include "TCPClient.h"

#include "../Common/IRouter.h"
#include "ITCPController.h"

class TCPController : public QObject, public ITCPController
{
    Q_OBJECT
public:
	TCPController(QObject* parent = 0, IRouter *router = 0);
    ~TCPController();

public:
	bool start();
    bool add_connection(QString host, quint16 port, IRouter *router);
    void stop();
    void delete_all_connections();
    int count_connections();
    TCPClient *get_client(QString host, quint16 port);

private:
    bool _is_started;
    QHash<QString, TCPClient *> _sockets_container; //QString signature: "host:port"
    IRouter     *_router;


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
