#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDataStream>
#include <QHostAddress>

#include <PwLogger/PwLogger.h>

#include "IRPC.h"

#include "Rpc/RpcDefines.h"
#include "Rpc/RpcServerBase.h"

Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QVector<QPointF>)

class RPCServer : public RpcServerBase, public IRPC
{
	Q_OBJECT
public:

	RPCServer(QObject* parent = NULL);
	virtual ~RPCServer();

public:
	virtual bool start(quint16 port, QHostAddress address = QHostAddress::Any);
	virtual void sendDataByRpc(const QString& signalType, const QByteArray& data);

private slots:
	void slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void slotRPCConnetion(quint64 client);
	void slotRPCDisconnected(quint64 client);

public slots:
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

	/// ATLANT from client
	void rpcSlotSetAtlantFrequency(quint64 clint, QByteArray data);

	void rpcSlotRequestStatus(quint64 client, int id);
	void rpcSlotSendRespModulation(quint64 client, QString modulation);

signals:
	/// Rpc signals
	void serverSendPointsRpcSignal(QByteArray);
	void serverSendDetectedBandwidthRpcSignal(QByteArray);
	void serverSendCorrelationRpcSignal(uint, uint, QByteArray);
	void serverSendAtlantDirectionRpcSignal(QByteArray);
	void serverSendAtlantPositionRpcSignal(QByteArray);
	void serverSendPrmStatusRpcSignal(int, int, int, int);
	void serverSendBplaDefRpcSignal(QByteArray);
	void serverSendBplaDefAutoRpcSignal(QByteArray);

};


#endif // RPCSERVER_H
