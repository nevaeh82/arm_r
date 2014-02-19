#include "TcpPRM300Controller.h"

TcpPRM300Controller::TcpPRM300Controller(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Controller)), parent)
{
	m_tcpDeviceCoder = new TcpPRM300Coder(this);
	m_tcpDeviceName = PRM300_TCP_DEVICE;
	m_logger->debug(QString("Created %1").arg(m_tcpDeviceName));
}

TcpPRM300Controller::TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Controller)), parent)
{
	m_tcpDeviceCoder = new TcpPRM300Coder(this);
}

TcpPRM300Controller::~TcpPRM300Controller()
{
}

QObject* TcpPRM300Controller::asQObject()
{
	return this;
}
