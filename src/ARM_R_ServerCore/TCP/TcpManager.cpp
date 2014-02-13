#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpManager));
	m_rpcServer = NULL;
}

TcpManager::~TcpManager()
{
}

void TcpManager::setRpcServer(IRPC* rpcServer)
{
	m_rpcServer = rpcServer;
}

QObject* TcpManager::asQObject()
{
	return this;
}
