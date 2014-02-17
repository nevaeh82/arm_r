#include "TcpAtlantController.h"

TcpAtlantController::TcpAtlantController(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpAtlantController)), parent)
{
	m_tcpDeviceCoder = new TcpAtlantCoder(this);
	m_tcpDeviceName = ATLANT_TCP_DEVICE;
}

TcpAtlantController::TcpAtlantController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpAtlantController)), parent)
{
	m_tcpDeviceCoder = new TcpAtlantCoder(this);
}

TcpAtlantController::~TcpAtlantController()
{
}

QObject* TcpAtlantController::asQObject()
{
	return this;
}
