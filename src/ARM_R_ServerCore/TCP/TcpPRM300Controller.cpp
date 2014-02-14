#include "TcpPRM300Controller.h"

TcpPRM300Controller::TcpPRM300Controller(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Controller)), parent)
{
	m_tcpDeviceCoder = TcpPRM300Coder(this);
	m_tcpDeviceName = PRM300_TCP_DEVICE;
}

TcpPRM300Controller::~TcpPRM300Controller()
{
}

QObject* TcpPRM300Controller::asQObject()
{
	return this;
}
