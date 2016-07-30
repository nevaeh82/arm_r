#ifndef RPCCONFIGCLIENT_H
#define RPCCONFIGCLIENT_H

#include <Rpc/RpcRoutedClient.h>

#include "Info/StationConfiguration.h"
#include "RPC/RpcDefines.h"
#include "Interfaces/IRpcConfigClient.h"

class RpcConfigClient : public RpcRoutedClient, public IRpcConfigClient
{
	Q_OBJECT
public:
	explicit RpcConfigClient(QObject* parent = NULL);
	virtual ~RpcConfigClient();

	// IRpcConfigClient interface
public:
	virtual void requestGetStationList(const QString& filename);
	virtual void requestGetDbConfiguration(const QString& filename);

signals:
	void getStationListSignal(QString);
	void connectionEstablishedSignal();

private slots:
	void receivedStationListSlot(QByteArray data);
	void receivedDbConfigurationSlot(QByteArray data);
	void receivedLocSystem(QByteArray data);
};

#endif // RPCCONFIGCLIENT_H
