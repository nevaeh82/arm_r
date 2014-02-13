#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonController)), parent)
{
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
}

TcpFlakonController::~TcpFlakonController()
{
}
