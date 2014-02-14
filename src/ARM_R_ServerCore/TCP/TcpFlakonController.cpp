#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonController)), parent)
{
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
	m_tcpDeviceName = FLAKON_TCP_DEVICE;
}

TcpFlakonController::~TcpFlakonController()
{
}

QObject* TcpFlakonController::asQObject()
{
	return this;
}
