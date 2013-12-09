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
#include "RPCClientFlakon.h"
#include "../Common/IRouter.h"
#include "../Common/Subscriber.h"

#include "Message.h"

#include "../Atlant/AtlantController.h"
#include "RPCClient_OD.h"

#define RPC_SLOT_SET_CLIENT_ID          "rpc_slot_set_client_id"
#define RPC_SLOT_SET_MAIN_STATION_COR   "rpc_slot_set_main_station_cor"
#define RPC_SLOT_SET_BANDWIDTH          "rpc_slot_set_bandwidth"
#define RPC_SLOT_SET_SHIFT              "rpc_slot_set_shift"
#define RPC_SLOT_RECOGNIZE              "rpc_slot_recognize"
#define RPC_SLOT_SS_CORRELATION         "rpc_slot_ss_correlation"
#define RPC_SLOT_AVARAGE_SPECTRUM       "rpc_slot_avarage_spectrum"

///PRM300 from rpc client
#define RPC_SLOT_PRM_SET_FREQ       "rpc_slot_prm_set_freq"
#define RPC_SLOT_PRM_REQUEST_FREQ   "rpc_slot_prm_request_freq"
#define RPC_SLOT_PRM_SET_ATT1       "rpc_slot_prm_set_att1"
#define RPC_SLOT_PRM_SET_ATT2       "rpc_slot_prm_set_att2"
#define RPC_SLOT_PRM_SET_FILTER     "rpc_slot_prm_set_filter"




///server
#define RPC_SLOT_SERVER_SEND_POINTS                 "rpc_slot_server_send_points"
#define RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION    "rpc_slot_server_send_response_modulation"
#define RPC_SLOT_SERVER_SEND_CORRELATION            "rpc_slot_server_send_correlation"

///PRM to RPC client
#define RPC_SLOT_SERVER_PRM_STATUS                   "rpc_slot_server_prm_status"

///ATLANT
#define RPC_SLOT_SERVER_ATLANT_DIRECTION            "rpc_slot_server_atlant_direction"
#define RPC_SLOT_SERVER_ATLANT_POSITION             "rpc_slot_server_atlant_position"

/// ATLANT from client
#define RPC_SLOT_SET_ATLANT_FREQUENCY               "rpc_slot_set_atlant_frequency"



/// BPLA
#define RPC_SLOT_SERVER_SEND_BPLA_DEF                 "rpc_slot_server_send_bpla_def"
#define RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO            "rpc_slot_server_send_bpla_def_auto"


/// connectiont status
#define RPC_SLOT_SERVER_STATUS                      "rpc_slot_server_status"

#define RPC_SLOT_REQUEST_STATUS                    "rpc_slot_request_status"


/// to solver
#define RPC_SLOT_SET_DATA_TO_SOLVER                "rpc_slot_set_data_to_solver"
#define RPC_SLOT_SET_CLEAR_TO_SOLVER               "rpc_slot_set_clear_to_solver"




class RPCServer : public QObject, public IRPC
{
    Q_OBJECT
public:
    RPCServer(IRouter* router);
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
    void rpc_slot_set_main_station_cor(quint64 client, int id, int station);
    void rpc_slot_set_bandwidth(quint64 client, int id, float bandwidth);
    void rpc_slot_set_shift(quint64 client, int id, float shift);
    void rpc_slot_recognize(quint64 client, int id, int type);
    void rpc_slot_ss_correlation(quint64 client, int id, bool enable);
    void rpc_slot_set_avarage_spectrum(quint64 client, int id, int avarage);

    ///prm300 from rpc client

    void rpc_slot_prm_set_freq(quint64 client, int id, short freq);
    void rpc_slot_prm_request_freq(quint64 client, int id);
    void rpc_slot_prm_set_att1(quint64 client, int id, int value);
    void rpc_slot_prm_set_att2(quint64 client, int id, int value);
    void rpc_slot_prm_set_filter(quint64 client, int id, int index);

    /// solver
    void rpc_slot_set_data_to_solver(quint64 client, QByteArray data);
    void rpc_slot_set_clear_to_solver(quint64 client, QByteArray data);




    /// to clients
    void rpc_slot_send_FFT(quint64 client, rpc_send_points_vector points);
    void rpc_slot_send_corr(quint64 client, quint32 point1, quint32 point2, rpc_send_points_vector points);

    void rpc_slot_send_resp_modulation(quint64 client, QString modulation);

    ///PRM
    void rpc_slot_prm_status(quint64 client, QByteArray *data);

    /// connection status
    void rpc_slot_status(quint64 client, QByteArray *data);

    ///ATLANT
    void rpc_slot_send_atalnt_data(quint64 client, QByteArray *data);
    void rpc_slot_send_atalnt_data_pos(quint64 client, QByteArray *data);

    /// ATLANT from client
    void rpc_slot_set_atlant_frequency(quint64 clint, QByteArray data);

    /// ARM_OD
    void rpc_slot_send_bpla(quint64 client, QByteArray *data);
    void rpc_slot_send_bpla_auto(quint64 client, QByteArray *data);

    void rpc_slot_send_atlant_direction(quint64 client, QByteArray* data);
    void rpc_slot_send_atlant_position(quint64 client, QByteArray* data);

    void rpc_slot_request_status(quint64 client, int id);
private:
    QxtRPCPeer*     _rpc_server;

    QMap<quint64, IClient *> _map_clients;

    IRouter*        _router;
    ISubscriber*    _subscriber;


signals:
    void finished();

public slots:
    void aboutToQuitApp();
};

Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QVector<QPointF>)
//Q_DECLARE_METATYPE(rpc_send_points_vector)

#endif // RPCSERVER_H
