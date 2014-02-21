#include "TcpPRM300Controller.h"

TcpPRM300Controller::TcpPRM300Controller(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Controller)), parent)
{
	m_tcpDeviceName = PRM300_TCP_DEVICE;
	m_logger->debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpPRM300Controller::TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpPRM300Controller)), parent)
{
	connect(this, SIGNAL(createTcpPRM300CoderInternalSignal()), this, SLOT(createTcpPRM300CoderInternalSlot()));
}

TcpPRM300Controller::~TcpPRM300Controller()
{
}

void TcpPRM300Controller::createTcpDeviceCoder()
{
	emit createTcpPRM300CoderInternalSignal();
}

QObject* TcpPRM300Controller::asQObject()
{
	return this;
}

void TcpPRM300Controller::createTcpPRM300CoderInternalSlot()
{
	m_logger->debug("Creating TcpPRM300Coder...");
	m_tcpDeviceCoder = new TcpPRM300Coder(this);
}
