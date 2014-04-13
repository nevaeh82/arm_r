#ifndef RPCFLAKONCLIENT_H
#define RPCFLAKONCLIENT_H

#include <Rpc/RpcRoutedClient.h>

#include "Interfaces/IStation.h"

class RpcFlakonClient : public RpcRoutedClient
{
	Q_OBJECT
public:
	explicit RpcFlakonClient(QObject *parent = 0);

	void sendMainStationCorrelation(const int id, const QString& value);
	void sendBandwidth(const int id, const float bandwidth);
	void sendShift(const int id, const float shift);
	void recognize(const int id, const int type);
	void sendCorrelation(const int id, const bool enable);
	void sendAvarageSpectrum(const int id, const int avarage);

	bool start(quint16 port, QHostAddress ipAddress);


protected slots:
	void registerRoute();
	void pointsReceived(QByteArray data);
	void bandwidthReceived(QByteArray data);
	void correlationReceived(QByteArray data);

signals:
	void signalEnableCorrelation(int, bool);

private slots:
	void slotEnableCorrelation(int, bool);
};

#endif // RPCFLAKONCLIENT_H
