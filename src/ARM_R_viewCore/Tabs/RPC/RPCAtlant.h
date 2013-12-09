#ifndef RPCATLANT_H
#define RPCATLANT_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include "IRPC.h"
#include "../../Common/IMessage.h"
#include "../../Common/CommandMessage.h"
#include "../ITabAtlant.h"

/// to server
#define RPC_SLOT_SET_CLIENT_ID                      "rpc_slot_set_client_id"
#define RPC_SLOT_SET_ATLANT_FREQUENCY               "rpc_slot_set_atlant_frequency"

/// from server ATLANT
#define RPC_SLOT_SERVER_ATLANT_DIRECTION            "rpc_slot_server_atlant_direction"

typedef struct A_Dir_Ans_msg
{
    //id запроса
    int requestId;
    //id источника, при ответе на DirectionFindingRequest совпадает с айди запроса
    //              при ответе на ScanRequest назначается пеленгатором
    int sourceId;
    //время засечки
    //DateTime.Utc.Now - время в 100наносекундных интервалах начиная с 1 января 1 года
    quint64 dateTime;

    //имя поста
    QString post;
    double postLatitude;     //GPS Geo Coords
    double postLongitude;    //GPS Geo Coords
    double postHeight;       //meters

    //центральная частота
    double frequency;
    //ширина пика
    double widht;

    //пеленг
    double direction;
    //угол места
    double angle;
    //уровень
    double level;
    //качество
    double quality;

    int motionType;
    double motionConfidence;
}A_Dir_Ans_msg;

typedef QByteArray                  rpc_send_atlant_data;

class RPCAtlant : public QObject, public IRPC
{
    Q_OBJECT
public:
    RPCAtlant(int id, ITabAtlant* parent_tab);
    ~RPCAtlant();

private slots:
    virtual int start();
    virtual int stop();

public:
    void set_command(IMessage* msg);

private slots:
    void _slotSetCommand(IMessage* msg);

private:
    QxtRPCPeer*         _rpc_client;
    QString             _ip_RPC;
    quint16             _port_RPC;
    IMessage*           _command_msg;
    int                 _id;
    ITabAtlant*         _parent_tab;

private:
    int     _read_settings(QString path_to_ini_file_RPC);
    void    _form_command(IMessage *msg);
    void    _send_freq(QVariant data);


private slots:
    void _close();
    void _slotRCPConnetion();
    void _slotRPCDisconnection();
    void _slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
    void _slotReconnection();

signals:
    void signalSetCommand(IMessage *msg);
    void signalFinished();

    void signalStart();
    void signalStop();
    void signalFinishRPC();

    ///RPC signals
signals:
    void signalSetClientId(int id);
    void signalSetFreq(QByteArray data);
    void signalReconnection();

public slots:
    void slotInit();
    void slotStart();
    void slotStop();
    void slotFinish();

    ///rpc_server

private slots:
    void rpc_slot_server_atlant_direction(QByteArray data);

private:




};

#endif // RPCATLANT_H
