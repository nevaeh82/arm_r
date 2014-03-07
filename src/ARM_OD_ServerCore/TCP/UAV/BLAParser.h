#ifndef BLAPARSER_H
#define BLAPARSER_H

#include <QObject>
#include <QByteArray>
#include <QFile>
#include <QDateTime>
#include "../ITCPParser.h"
#include "../../Common/IRouter.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"

class BLAParser : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    BLAParser(int id, IRouter *router);
    ~BLAParser();

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

#endif // BLAPARSER_H
