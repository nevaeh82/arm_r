#ifndef RPCFLAKONCLIENT_H
#define RPCFLAKONCLIENT_H

#include <Rpc/RpcRoutedClient.h>

#include "Interfaces/IStation.h"

class RpcFlakonClient : public RpcRoutedClient
{
	Q_OBJECT
public:
	explicit RpcFlakonClient(QObject *parent = 0);

	void sendMainStationCorrelation(IStation *station, const QString& value);
	void sendBandwidth(IStation *station, const float bandwidth);
	void sendShift(IStation *station, const float shift);
	void recognize(IStation *station, const int type);
	void sendCorrelation(IStation *station, const bool enable);
	void sendAvarageSpectrum(IStation *station, const int avarage);

protected slots:
	void registerRoute();
	void pointsReceived(QByteArray data);
	void bandwidthReceived(QByteArray data);
	void correlationReceived(uint point1, uint point2, QByteArray points);
};

#endif // RPCFLAKONCLIENT_H
