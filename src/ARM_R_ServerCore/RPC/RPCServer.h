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
#include <QHostAddress>

#include <PwLogger/PwLogger.h>

#include "IRPC.h"
#include "RPCClientFlakon.h"
#include "../Common/IRouter.h"
#include "../Common/Subscriber.h"

#include "Message.h"

#include "../Atlant/AtlantController.h"
#include "RPCClient_OD.h"

#include "Rpc/RpcDefines.h"
#include "Rpc/RpcServerBase.h"

class RPCServer : public RpcServerBase, public IRPC
{
    Q_OBJECT
public:
	RPCServer(IRouter* router, QObject *parent = 0);
    ~RPCServer();

public:
	virtual bool start(quint16 port, QHostAddress address = QHostAddress::Any);

	virtual quint64 getClientId(IClient* client);

private slots:
	void slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void slotRPCConnetion(quint64 client);
	void slotRPCDisconnected(quint64 client);

public slots:
	void rpcSlotSetClientId(quint64 client, int id);
	void rpcSlotSetMainStationCor(quint64 client, int id, int station);
	void rpcSlotSetBandwidth(quint64 client, int id, float bandwidth);
	void rpcSlotSetShift(quint64 client, int id, float shift);
	void rpcSlotRecognize(quint64 client, int id, int type);
	void rpcSlotSsCorrelation(quint64 client, int id, bool enable);
	void rpcSlotSetAvarageSpectrum(quint64 client, int id, int avarage);

    ///prm300 from rpc client
	void rpcSlotPrmSetFreq(quint64, int id, short freq);
	void rpcSlotPrmRequestFreq(quint64 client, int id);
	void rpcSlotPrmSetAtt1(quint64 client, int id, int value);
	void rpcSlotPrmSetAtt2(quint64 client, int id, int value);
	void rpcSlotPrmSetFilter(quint64 client, int id, int index);

    /// solver
	void rpcSlotSetDataToSolver(quint64 client, QByteArray data);
	void rpcSlotSetClearToSolver(quint64 client, QByteArray data);

    /// to clients
	void rpcSlotSendFft(quint64 client, rpc_send_points_vector points);
	void rpcSlotsenddetectedbandwidth(quint64 client, rpc_send_points_vector points);
	void rpcSlotSendCorr(quint64 client, quint32 point1, quint32 point2, rpc_send_points_vector points);

	void rpcSlotSendRespModulation(quint64 client, QString modulation);

    ///PRM
	void rpcSlotPrmStatus(quint64 client, QByteArray *data);

    /// connection status
	void rpcSlotStatus(quint64 client, QByteArray *data);

    ///ATLANT
	void rpcSlotSendAtalntData(quint64 client, QByteArray *data);
	void rpcSlotSendAtalntDataPos(quint64 client, QByteArray *data);

    /// ATLANT from client
	void rpcSlotSetAtlantFrequency(quint64 clint, QByteArray data);

    /// ARM_OD
	void rpcSlotSendBpla(quint64 client, QByteArray *data);
	void rpcSlotSendBplaAuto(quint64 client, QByteArray *data);

	void rpcSlotSendAtlantDirection(quint64 client, QByteArray* data);
	void rpcSlotSendAtlantPosition(quint64 client, QByteArray* data);

	void rpcSlotRequestStatus(quint64 client, int id);

private:
	QMap<quint64, IClient *> m_mapClients;

	IRouter*        m_router;
	ISubscriber*    m_subscriber;

signals:
    void finished();

public slots:
    void aboutToQuitApp();
};

Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QVector<QPointF>)
//Q_DECLARE_METATYPE(rpc_send_points_vector)

#endif // RPCSERVER_H
