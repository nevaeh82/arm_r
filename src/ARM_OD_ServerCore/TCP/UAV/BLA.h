#ifndef BLA_H
#define BLA_H

#include <QObject>
#include <QString>
#include "../ITCPController.h"
#include "../ITCPClient.h"
#include "../TCPClient.h"
#include "BLAParser.h"
#include "BLAParser2.h"
#include "IBLAController.h"

class BLA : public QObject
{
    Q_OBJECT
public:
    BLA(QString ip, int port, int id, int dev, IRouter *router);
    ~BLA();

public:
    void set();

private:
    QString _ip;
    int _port;
    int _id;
    int _dev;
    ITCPController  *_tcp_controller;
    IRouter         *_router;
    BLAParser*      _parser;
    BLAParser2*     _parser2;

public slots:
    void _send_request(bool state);
    void _send_request2(bool state);



};

#endif // BLA_H
