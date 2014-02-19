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

#include "IRPC.h"
#include "Common/IMessage.h"
#include "Common/CommandMessage.h"
#include "Tabs/ITabAtlant.h"

#include "Rpc/RpcDefines.h"
#include "Rpc/RpcClientBase.h"

typedef	QByteArray	rpc_send_atlant_data;

class RPCAtlant : public RpcClientBase
{
	Q_OBJECT
private:
	IMessage*           m_commandMsg;
	int                 m_id;
	ITabAtlant*         m_parentTab;

public:
	RPCAtlant(int id, ITabAtlant* parent_tab, QObject*  = 0);
	~RPCAtlant();

	bool start(quint16 port, QHostAddress address);
	void set_command(IMessage* msg);

private:
	void formCommand(IMessage* msg);
	void sendFreq(QVariant data);

private slots:
	void slotRCPConnetion();
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
	void signalSetClientId(int id);
	void signalSetFreq(QByteArray data);
	void signalReconnection();
};

#endif // RPCATLANT_H
