#ifndef NIIPPPARSER_H
#define NIIPPPARSER_H

#include <QObject>
#include "../ITCPParser.h"
#include "../../Common/IRouter.h"
#include "../../Common/IMessage.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"
#include <QByteArray>
#include <QStringList>
#include <QDateTime>

class NIIPPParser : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    NIIPPParser(int id, int type);
    ~NIIPPParser();

public:
    void distruct();
    void set_router(IRouter *router);
    virtual void parsing(QByteArray data);

private:
    int             _id;
    int             _type;
    IRouter*        _router;
    ISubscriber*    _subscriber;
//    const double    _zone[52];
};


#endif // NIIPPPARSER_H
