#ifndef ITCPCONTROLLER_H
#define ITCPCONTROLLER_H

#include <QString>

class TCPClient;
class IRouter;

class ITCPController
{
public:
    virtual ~ITCPController(){};
    virtual TCPClient *get_client(QString host, quint16 port)                       = 0;
    virtual bool add_connection(QString host, quint16 port, IRouter *router)        = 0;
    virtual void stop()                                                             = 0;
};

#endif // ITCPCONTROLLER_H
