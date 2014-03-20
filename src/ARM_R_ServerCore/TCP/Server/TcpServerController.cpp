#include "TcpServerController.h"

TcpServerController::TcpServerController(const QString& tcpServerName, QObject *parent) :
	BaseTcpServerController(tcpServerName, parent)
{
	debug(QString("Created server %1").arg(tcpServerName));
}

TcpServerController::~TcpServerController()
{
}

void TcpServerController::createTcpServerCoder()
{
	debug("Creating TcpServerCoder...");
	m_tcpServerCoder = new TcpServerCoder(this);
}
