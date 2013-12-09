#ifndef ISUBSCRIBER_H
#define ISUBSCRIBER_H

#include <QVector>
#include <QSharedPointer>

#include "../Common/IMessage.h"
#include "../Common/IClient.h"

#define RPC_MAIN_STATION_COR    100
#define RCP_BANDWIDTH_TYPE      101
#define RCP_SHIFT_TYPE          102
#define RPC_SS_CORRELATION      103
#define RCP_COMMAND_RECOGNIZE   104
#define RPC_AVARAGE_SPECTRUM    105


#define FLAKON_FFT              1001
#define FLAKON_CORRELATION      1004
#define FLAKON_SIGNAL_TYPE      1005
/// BPLA
#define FLAKON_BPLA             1006
#define FLAKON_BPLA_AUTO        1007

///ATLANT
#define ATLANT_DIRECTION        3001
#define ATLANT_POSITION         3002

/// ATLANT from client
#define ATLANT_SET_FREQ         301

///PRM
#define PRM_SET_FREQ            200
#define PRM_REQUEST_FREQ        201
#define PRM_SET_ATT1            202
#define PRM_SET_ATT2            203
#define PRM_SET_FILTER          204


#define PRM_STATUS              2000

#define CONNECTION_STATUS       4000

#define REQUEST_STATUS          500

#define SET_SOLVER              700
#define SET_SOLVER_CLEAR        708


class IClient;

class ISubscriber
{
public:
    virtual ~ISubscriber(){};

    virtual void data_ready(int type, QSharedPointer<IMessage> msg)         = 0;

    virtual void add_subscription(int type, IClient* client)                = 0;
    virtual void remove_subscription(int type, IClient* client)             = 0;
    virtual void remove_subscription(int type)                              = 0;
    virtual void remove_subscription(IClient* client)                       = 0;

    virtual QVector<IClient *> get_subscribers(int type)                    = 0;

};

#endif // ISUBSCRIBER_H
