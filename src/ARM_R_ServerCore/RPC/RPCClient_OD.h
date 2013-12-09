#ifndef RPCCLIENT_OD_H
#define RPCCLIENT_OD_H

#include <QObject>
#include <QSharedPointer>
#include <QMetaType>
#include "../Common/IClient.h"
#include "../Common/IRouter.h"
#include "../Common/IMessage.h"
#include "../Common/ISubscriber.h"
#include "IRPC.h"

typedef QSharedPointer<IMessage> rpc_flakon_msg;

class RPCClient_OD : public QObject, public IClient
{
    Q_OBJECT
public:
    RPCClient_OD(IRouter* router, IRPC* server);
    ~RPCClient_OD();

public:
    virtual void set_id(int _id);
    virtual int  get_id();
    virtual void set_type(int type);
    virtual int get_type();
    virtual void send_data(QSharedPointer<IMessage> msg_ptr);


private:
    int             _id;
    int             _type;
    IRouter*        _router;
    IRPC*           _server;

private slots:
    void _slotGetData(rpc_flakon_msg msg_ptr);

signals:
    void signalSendToRPCODBpla(quint64, QByteArray*);
    void signalSendToRPCODBplaAuto(quint64, QByteArray*);

    void signalSendRPCODAtlantDirection(quint64, QByteArray*);
    void signalSendRPCODAtlantPosition(quint64, QByteArray*);

};

#endif // RPCCLIENT_OD_H
