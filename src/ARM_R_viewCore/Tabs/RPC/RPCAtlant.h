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

#include "IRPC.h"
#include "../../Common/IMessage.h"
#include "../../Common/CommandMessage.h"
#include "../ITabAtlant.h"

#include "Rpc/RpcDefines.h"

typedef	QByteArray	rpc_send_atlant_data;

class RPCAtlant : public QObject, public IRPC
{
	Q_OBJECT
private:
	QxtRPCPeer*         m_rpcClient;
	QString             m_ipRpc;
	quint16             m_portRpc;
	IMessage*           m_commandMsg;
	int                 m_id;
	ITabAtlant*         m_parentTab;

public:
	RPCAtlant(int id, ITabAtlant* parent_tab);
	~RPCAtlant();

	void set_command(IMessage* msg);

private:
	bool readSettings(const QString& path_to_ini_file_RPC);
	void formCommand(IMessage* msg);
	void sendFreq(QVariant data);

public slots:
	void start();
	void stop();

	void slotInit();
	void slotFinish();

private slots:
	virtual int slotStart();
	virtual int slotStop();
	void slotClose();
	void slotRCPConnetion();
	void slotRPCDisconnection();
	void slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void slotReconnection();
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
