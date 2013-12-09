#ifndef ATLANTCONTROLLER_H
#define ATLANTCONTROLLER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QMetaType>
#include <QByteArray>

//#include "ControllerPelengator.h"
#include "../RPC/IRPC.h"


#include "../Common/IClient.h"
#include "../Common/IRouter.h"
#include "../Common/IMessage.h"
#include "../Common/ISubscriber.h"

typedef QByteArray  rpc_send_atlant_data;

typedef QSharedPointer<IMessage> rpc_flakon_msg;

class AtlantController : public QObject, public IClient
{
    Q_OBJECT
public:
    AtlantController(IRouter* router, IRPC* server);
    ~AtlantController();

public:
    void connect();
    void change_mode(int mode);

private:
//    ControllerPelengator*   _controller;
    QString                 _host;
    unsigned short          _port;
    QString                 _name;
    int                     _current_mode;
    IRouter*                _router;

public:
    virtual void set_id(int _id);
    virtual int  get_id();
    virtual void set_type(int type);
    virtual int  get_type();
    virtual void send_data(QSharedPointer<IMessage> msg_ptr);

    void _slotGetData(rpc_flakon_msg msg_ptr);


private:
    int             _id;
    IRPC*           _server;
    int             _type;

signals:
    void signalSendToRPCAtlantDirection(quint64 cid, QByteArray* data);
    void signalSendToRPCAtlantPosition(quint64 cid, QByteArray* data);

};

#endif // ATLANTCONTROLLER_H
