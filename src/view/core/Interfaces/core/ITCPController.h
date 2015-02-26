#ifndef ITCPCONTROLLER_H
#define ITCPCONTROLLER_H

#include <QString>

class TCPClient;
class ITCPParser;

class ITCPController
{
public:
    virtual ~ITCPController(){};
    virtual TCPClient *get_client(QString host, quint16 port, int id)                       = 0;
    virtual bool add_connection(QString host, quint16 port, ITCPParser *parser, int id)     = 0;
    virtual void stop()                                                                     = 0;
};

#endif // ITCPCONTROLLER_H
