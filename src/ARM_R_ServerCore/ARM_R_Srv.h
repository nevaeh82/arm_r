#ifndef ARM_R_SRV_H
#define ARM_R_SRV_H

#include <QObject>
#include <QTextCodec>

#include <QMetaType>

#include "RPC/RPCServer.h"
#include "TCP/TCPController.h"

#include "Common/Router.h"
#include "Common/Subscriber.h"

#include "Atlant/AtlantController.h"

#include "Flakon/StationProperty.h"

#include "Flakon/Server/PServer.h"

typedef QSharedPointer<IMessage>    rpc_flakon_msg;
typedef QVector<QPointF>            rpc_send_points_vector;
typedef QByteArray                  rpc_send_atlant_data;

typedef DataFromRadioLocation       DataFromRadioLocation;
typedef DataFromFlacon              DataFromFlacon;


class ARM_R_Srv : public QObject
{
    Q_OBJECT
public:
    ARM_R_Srv();
    ~ARM_R_Srv();

public:

private:
    RPCServer*          _rpc_server;
    Router*             _router;
    Subscriber*         _subscriber_up;
    TCPController*      _tcp_controller;
    AtlantController*   _atlant_controller;

    QMap<int, StationProperty* >*    _map_station_property;

    PServer*                        _pserver;

private:
    void _read_settings();
    void _count_distances();

};

Q_DECLARE_METATYPE(rpc_flakon_msg)
Q_DECLARE_METATYPE(quint32)
Q_DECLARE_METATYPE(rpc_send_atlant_data)
//Q_DECLARE_METATYPE(rpc_send_points_vector)
//Q_DECLARE_METATYPE(DataFromFlacon)

//Q_DECLARE_METATYPE(DataFromRadioLocation)


#endif // ARM_R_SRV_H
