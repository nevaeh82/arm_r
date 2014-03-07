#ifndef BLAPARSER2_H
#define BLAPARSER2_H

#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QDateTime>
#include <QStringList>
#include "../ITCPParser.h"
#include "../../Common/IRouter.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"

class BLAParser2 : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    BLAParser2(int id, IRouter *router);
    ~BLAParser2();

public:
    virtual void parsing(QByteArray data);
    virtual void distruct();

private:
    IRouter*        _router;
    ISubscriber*    _subscriber;
    double          _latitude;
    double          _longitude;
    double          _altitude;
    int             _id;
    QFile*          _fl;
};

#endif // BLAPARSER2_H
