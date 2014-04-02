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
#include <QHostAddress>

#include <Rpc/RpcDefines.h>
#include <Rpc/RpcRoutedClient.h>

#include "Common/IMessage.h"
#include "Common/CommandMessage.h"
#include "Tabs/ITabAtlant.h"


typedef	QByteArray	rpc_send_atlant_data;

class RpcAtlantClient : public RpcRoutedClient
{
	Q_OBJECT
private:
	IMessage*           m_commandMsg;
	int                 m_id;
	ITabAtlant*         m_parentTab;

public:
	RpcAtlantClient(int id, ITabAtlant* parent_tab, QObject*  = 0);
	~RpcAtlantClient();

	bool start(quint16 port, QHostAddress address);
	void set_command(IMessage* msg);

private:
	void formCommand(IMessage* msg);
	void sendFreq(QVariant data);

private slots:
	void registerRoute();
	void slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void slotSetCommand(IMessage* msg);

	///rpc_server
	void rpcSlotServerAtlantDirection(QByteArray data);

signals:
	void signalSetCommand(IMessage* msg);
	void signalFinished();

	void signalStart();
	void signalStop();
	void signalFinishRPC();

	///RPC signals
	void signalSetFreq(QByteArray data);
	void signalReconnection();
};

#endif // RPCATLANT_H
