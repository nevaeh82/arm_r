#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include <QDataStream>

#include <QMap>

#include "IRPC.h"
#include "RPCClient.h"
#include "../Common/IRouter.h"
#include "../Common/Subscriber.h"

#include "Message.h"

#include "Client/RPCClient_R.h"


#define RPC_SLOT_SET_CLIENT_ID   "rpc_slot_set_client_id"
//#define RPC_SLOT_SET_BANDWIDTH   "rpc_slot_set_bandwidth"
//#define RPC_SLOT_SET_SHIFT       "rpc_slot_set_shift"
//#define RPC_SLOT_RECOGNIZE       "rpc_slot_recognize"
//#define RPC_SLOT_SS_CORRELATION  "rpc_slot_ss_correlation"

/////PRM300 from rpc client
//#define RPC_SLOT_PRM_SET_FREQ       "rpc_slot_prm_set_freq"
//#define RPC_SLOT_PRM_REQUEST_FREQ   "rpc_slot_prm_request_freq"
//#define RPC_SLOT_PRM_SET_ATT1       "rpc_slot_prm_set_att1"
//#define RPC_SLOT_PRM_SET_ATT2       "rpc_slot_prm_set_att2"
//#define RPC_SLOT_PRM_SET_FILTER     "rpc_slot_prm_set_filter"




///server
#define RPC_SLOT_SERVER_SEND_BLA_POINTS               "rpc_slot_server_send_bla_points"

/// AIS
#define RPC_SLOT_SERVER_SEND_AIS_DATA                 "rpc_slot_server_send_ais_data"
//#define RPC_SLOT_SERVER_SEND_POINTS                 "rpc_slot_server_send_points"
//#define RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION    "rpc_slot_server_send_response_modulation"
//#define RPC_SLOT_SERVER_SEND_CORRELATION            "rpc_slot_server_send_correlation"

/// from ARM_R
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS              "rpc_slot_server_send_bpla_point_from_arm_r"
#define RPC_SLOT_SERVER_SEND_BPLA_POINTS_AUTO         "rpc_slot_server_send_bpla_point_from_arm_r_auto"

/// NIIPP
#define RPC_SLOT_SERVER_SEND_NIIPP_DATA                "rpc_slot_server_send_NIIPP_data"

/// to NIIPP
#define RPC_SLOT_SET_NIIPP_BPLA                         "rpc_slot_set_niipp_data"

/// to SOLVER
#define RPC_SLOT_SET_SOLVER_DATA    "rpc_slot_set_solver_data"
#define RPC_SLOT_SET_SOLVER_CLEAR   "rpc_slot_set_solver_clear"



/// from ARM_R Atlant direction
#define RPC_SLOT_SERVER_SEND_ATLANT_DIRECTION           "rpc_slot_server_send_atlant_direction"
#define RPC_SLOT_SERVER_SEND_ATLANT_POSITION            "rpc_slot_server_send_atlant_position"

///PRM to RPC client
//#define RPC_SLOT_SERVER_PRM_STATUS                   "rpc_slot_server_prm_status"


class RPCServer : public QObject, public IRPC
{
    Q_OBJECT
public:
    RPCServer(IRouter* router/*, IRPC* r_client*/);
    ~RPCServer();

public:
    virtual int start();
    virtual int stop();
    virtual quint64 get_client_id(IClient* client);


private slots:
    void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
    void _slotRPCConnetion(quint64 client);
    void _slotRPCDisconnected(quint64 client);

public slots:
    void rpc_slot_set_client_id(quint64 client, int id);

    void rpc_slot_set_niipp_data(quint64 client, QByteArray data);

    void rpc_slot_set_solver_data(quint64 client, QByteArray data);
    void rpc_slot_set_solver_clear(quint64 client, QByteArray data);

//    void rpc_slot_set_bandwidth(quint64 client, int id, float bandwidth);
//    void rpc_slot_set_shift(quint64 client, int id, float shift);
//    void rpc_slot_recognize(quint64 client, int id, int type);
//    void rpc_slot_ss_correlation(quint64 client, int id, bool enable);

//    ///prm300 from rpc client

//    void rpc_slot_prm_set_freq(quint64 client, int id, short freq);
//    void rpc_slot_prm_request_freq(quint64 client, int id);
//    void rpc_slot_prm_set_att1(quint64 client, int id, int value);
//    void rpc_slot_prm_set_att2(quint64 client, int id, int value);
//    void rpc_slot_prm_set_filter(quint64 client, int id, int index);




    /// to clients

//    void rpc_slot_send_FFT(quint64 client, rpc_send_points_vector points);
//    void rpc_slot_send_corr(quint64 client, quint32 point1, quint32 point2, rpc_send_points_vector points);

//    void rpc_slot_send_resp_modulation(quint64 client, QString modulation);

//    ///PRM
//    void rpc_slot_prm_status(quint64 client, QByteArray *data);

    /// BLA
    void rpc_slot_send_bla_points(quint64 client, int id, rpc_QPointF points, double alt, double speed, double course, int state);

    /// AIS
    void rpc_slot_send_ais_data(quint64 client, QByteArray* data);

    /// ARM_R_Server
    void rpc_slot_send_bpla_points(quint64 client, QByteArray* data);
    void rpc_slot_send_bpla_points_auto(quint64 client, QByteArray* data);

    void rpc_slot_send_atlant_direction(quint64 client, QByteArray* data);
    void rpc_slot_send_atlant_position(quint64 client, QByteArray* data);


    /// NIIPP
    void rpc_slot_send_NIIPP_data(quint64 client, QByteArray* data);



//    void rpc_slot_send_bla_point(quint64 client, QPointF point, double alt);
private:
    QxtRPCPeer*     _rpc_server;

    QMap<quint64, IClient *> _map_clients;

    IRouter*        _router;
    ISubscriber*    _subscriber;

//    IRPC*           _r_client;


signals:
    void finished();

public slots:
    void aboutToQuitApp();
};

//Q_DECLARE_METATYPE(QPointF)
//Q_DECLARE_METATYPE(QVector<QPointF>)
//Q_DECLARE_METATYPE(rpc_send_points_vector)

#endif // RPCSERVER_H
