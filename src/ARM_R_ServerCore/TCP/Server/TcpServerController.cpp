#include "TcpServerController.h"

TcpServerController::TcpServerController(QObject *parent) :
	BaseTcpServerController(parent)
{
	debug(QString("Created server %1").arg("TypeArmrTcpServer"));
	m_tcpServerName = ARMR_TCP_SERVER;
	m_deviceType = ARMR_TCP_SERVER;
}

TcpServerController::TcpServerController(const QString &tcpServerName, QObject *parent) :
	BaseTcpServerController(tcpServerName, parent)
{
	m_tcpServerName = tcpServerName;
	m_deviceType = ARMR_TCP_SERVER;
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
