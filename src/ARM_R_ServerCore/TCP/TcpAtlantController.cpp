#include "TcpAtlantController.h"

TcpAtlantController::TcpAtlantController(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpAtlantController)), parent)
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
