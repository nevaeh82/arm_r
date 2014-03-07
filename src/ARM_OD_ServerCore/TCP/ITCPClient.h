#ifndef ITCPCLIENT_H
#define ITCPCLIENT_H

#include <QObject>
#include <QByteArray>

#include "ITCPParser.h"

class ITCPClient: public QObject
{
public:
    virtual ~ITCPClient(){};

    virtual void set_parser(ITCPParser* parser) = 0;

    virtual void set(QByteArray data)           = 0;
//    virtual QByteArray get()                = 0;
    virtual bool get_state()                    = 0;

public slots:
    virtual void slotDestroy()                  = 0;

signals:
    void signalConnected(bool state);
};

#endif // ITCPCLIENT_H
