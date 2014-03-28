#ifndef ARM_R_SRV_H
#define ARM_R_SRV_H

#include <QObject>
#include <QThread>

#include <PwLogger/PwLogger.h>

#include "RPC/RPCServer.h"
#include "TCP/TcpManager.h"
#include "TCP/TcpSettingsManager.h"
#include "TCP/Server/TcpServerController.h"

Q_DECLARE_METATYPE(MessageSP)

class ARM_R_Srv : public QObject
{
	Q_OBJECT

public:
	explicit ARM_R_Srv(QObject* parent = NULL);
	virtual ~ARM_R_Srv();

private:
	static Pw::Logger::ILogger* m_logger;
	RPCServer* m_rpcServer;
	BaseTcpServerController* m_tcpServer;
	TcpManager* m_tcpManager;
};



#endif // ARM_R_SRV_H
