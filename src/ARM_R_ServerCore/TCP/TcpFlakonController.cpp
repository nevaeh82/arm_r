#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonController)), parent)
{
	m_tcpDeviceName = FLAKON_TCP_DEVICE;
	m_logger->debug(QString("Created %1").arg(m_tcpDeviceName));
}

TcpFlakonController::TcpFlakonController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonController)), parent)
{
}

TcpFlakonController::~TcpFlakonController()
{
}

void TcpFlakonController::createTcpDeviceCoder()
{
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
}

QObject* TcpFlakonController::asQObject()
{
	return this;
}
