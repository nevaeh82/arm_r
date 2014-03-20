#include "TcpFlakonController.h"

TcpFlakonController::TcpFlakonController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = FLAKON_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpFlakonController::TcpFlakonController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
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
	debug("Creating TcpFlakonCoder...");
	m_tcpDeviceCoder = new TcpFlakonCoder(this);
}
