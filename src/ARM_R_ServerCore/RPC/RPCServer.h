#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDataStream>
#include <QHostAddress>
#include <QTime>

#include <Rpc/RpcRoutedServer.h>

#include "Rpc/RpcMessageStruct.h"
#include "Rpc/RpcDefines.h"

Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QVector<QPointF>)

class RpcServer : public RpcRoutedServer
{
	Q_OBJECT

public:

	RpcServer(QObject* parent = NULL);
	virtual ~RpcServer();

public:
	virtual bool start(quint16 port, QHostAddress address = QHostAddress::Any);

private slots:
	void logConnectionError(QAbstractSocket::SocketError socketError);
	void logConnectionSuccess(quint64 client);
	void logClientDisconected(quint64 client);

	void requestGetStationListSlot(quint64 client, QString configFilename);
	void requestGetAtlantConfigurationSlot(quint64 client, QString configFilename);
	void requestGetDbConfigurationSlot(quint64 client, QString configFilename);

	void setMainStationCorrelation(quint64 client, int id, QString station);
	void setBandwidth(quint64 client, int id, float bandwidth);
	void setShift(quint64 client, int id, float shift);
	void recognize(quint64 client, int id, int);
	void ssCorrelation(quint64 client, int id, bool enable);
	void setAvarageSpectrum(quint64 client, int id, int avarage);

	///prm300 from rpc client
	void setPrmFrequency(quint64 client, QString name, short freq);
	void requestPrmFrequency(quint64 client, QString name);
	void setPrmAtt1(quint64 client, QString name, int value);
	void setPrmAtt2(quint64 client, QString name, int value);
	void setPrmFilter(quint64 client, QString name, int index);

	/// solver
	void setDataToSolver(quint64 client, QByteArray data);
	void setClearToSolver(quint64 client, QByteArray data);

	/// ATLANT from client
	void setAtlantFrequency(quint64 clint, QByteArray data);

	void requestStatus(quint64 client, QString name);
	void sendResponseModulation(quint64 client, QString modulation);

signals:
	/// Rpc signals
	void serverSendPointsRpcSignal(QByteArray);
	void serverSendDetectedBandwidthRpcSignal(QByteArray);
	void serverSendCorrelationRpcSignal(uint, uint, QByteArray);
	void serverSendAtlantDirectionRpcSignal(QByteArray);
	void serverSendAtlantPositionRpcSignal(QByteArray);
	void serverSendPrmStatusRpcSignal(int, int, int, int);
	void serverSendPrmStatusRpcSignalBool(QByteArray);
	void serverSendBplaDefRpcSignal(QByteArray);
	void serverSendBplaDefAutoRpcSignal(QByteArray);

};


#endif // RPCSERVER_H
