#ifndef RPCCONFIGCLIENT_H
#define RPCCONFIGCLIENT_H

#include <Rpc/RpcRoutedClient.h>

#include "Info/StationConfiguration.h"
#include "RPC/RpcDefines.h"

class RpcConfigClient : public RpcRoutedClient
{
	Q_OBJECT
public:
	explicit RpcConfigClient(QObject* parent = NULL);
	virtual ~RpcConfigClient();

signals:
	void getStationListSignal(QString);
	void getAtlantConfigSignal(QString);

private slots:
	void receivedStationListSlot(QByteArray data);
	void receivedAtlantConfigSlot(QByteArray data);
};

#endif // RPCCONFIGCLIENT_H
