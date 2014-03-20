#include "TcpAtlantController.h"

TcpAtlantController::TcpAtlantController(QObject* parent) :
	BaseTcpDeviceController(parent)
{
	m_tcpDeviceName = ATLANT_TCP_DEVICE;
	debug(QString("Created %1").arg(m_tcpDeviceName));

	connect(this, SIGNAL(createTcpDeviceCoderInternalSignal()), this, SLOT(createTcpDeviceCoderInternalSlot()));
}

TcpAtlantController::TcpAtlantController(const QString& tcpDeviceName, QObject* parent) :
	BaseTcpDeviceController(tcpDeviceName, parent)
{
	connect(this, SIGNAL(createTcpAtlantCoderInternalSignal()), this, SLOT(createTcpAtlantCoderInternalSlot()));
}

TcpAtlantController::~TcpAtlantController()
{
}

void TcpAtlantController::createTcpDeviceCoder()
{
	emit createTcpAtlantCoderInternalSignal();
}

QObject* TcpAtlantController::asQObject()
{
	return this;
}

void TcpAtlantController::createTcpAtlantCoderInternalSlot()
{
	debug("Creating TcpAtlantCoder...");
	m_tcpDeviceCoder = new TcpAtlantCoder(this);
}
