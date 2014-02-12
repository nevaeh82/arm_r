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

#include "Rpc/RpcDefines.h"

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
	void rpc_slotSendDetectedBandwidth(quint64 client, rpc_send_points_vector points);
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
