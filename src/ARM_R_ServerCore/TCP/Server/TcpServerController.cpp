#include "TcpServerController.h"

TcpServerController::TcpServerController(QObject *parent) :
	BaseTcpServerController(tcpServerName, parent)
{
	debug(QString("Created server %1").arg("TypeArmrTcpServer"));
	m_tcpServerName = TypeArmrTcpServer;
	m_deviceType = TypeArmrTcpServer;
}

TcpServerController::~TcpServerController()
{
}

void TcpServerController::createTcpServerCoder()
{
	debug("Creating TcpServerCoder...");
	m_tcpServerCoder = new TcpServerCoder(this);
}

QObject* TcpServerController::asQObject()
{
	return this;
}
