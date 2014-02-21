#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	BaseTcpDeviceController(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonController)), parent)
{
	m_tcpDeviceName = FLAKON_TCP_DEVICE;
	m_logger->debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpFlakonController::TcpFlakonController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonController)), parent)
{
	connect(this, SIGNAL(createTcpFlakonCoderInternalSignal()), this, SLOT(createTcpFlakonCoderInternalSlot()));
}

TcpFlakonController::~TcpFlakonController()
{
}

void TcpFlakonController::createTcpDeviceCoder()
{
	emit createTcpFlakonCoderInternalSignal();
}

QObject* TcpFlakonController::asQObject()
{
	return this;
}

void TcpFlakonController::createTcpFlakonCoderInternalSlot()
{
	m_logger->debug("Creating TcpFlakonCoder...");
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
}
