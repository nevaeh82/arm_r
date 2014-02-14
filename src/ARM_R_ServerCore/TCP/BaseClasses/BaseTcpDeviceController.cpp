#include "BaseTcpDeviceController.h"

BaseTcpDeviceController::BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;

	m_tcpClient = new BaseTcpClient(this);
	m_tcpClient->registerReceiver(this);

	m_tcpDeviceCoder = new BaseTcpDeviceCoder(this);

	connect(this, SIGNAL(connectToHostInternalSignal(QString,quint32)), this, SLOT(connectToHostInternalSlot(QString,quint32)));
	connect(this, SIGNAL(disconnectFromHostInternalSignal()), this, SLOT(disconnectFromHostInternalSlot()));
	connect(this, SIGNAL(sendDataInternalSignal(const IMessage<QByteArray>*)), this, SLOT(sendDataInternalSlot(const IMessage<QByteArray>*)));
}

BaseTcpDeviceController::~BaseTcpDeviceController()
{
	m_tcpClient->deregisterReceiver(this);
}

void BaseTcpDeviceController::connectToHost(const QString& host, const quint32& port)
{
	emit connectToHostInternalSignal(host, port);
}

void BaseTcpDeviceController::disconnectFromHost()
{
	emit disconnectFromHostInternalSignal();
}

bool BaseTcpDeviceController::isConnected()
{
	return m_tcpClient->isConnected();
}

void BaseTcpDeviceController::sendData(const IMessage<QByteArray>* message)
{
	emit sendDataInternalSignal(message);
}

QObject* BaseTcpDeviceController::asQObject()
{
	return this;
}

void BaseTcpDeviceController::onDataReceived(const QVariant& argument)
{
	IMessage<QByteArray>* encodedData = m_tcpDeviceCoder->encode(argument.toByteArray());

	foreach(ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(BASE_TCP_DEVICE, QVariant(encodedData));
	}
}

void BaseTcpDeviceController::connectToHostInternalSlot(const QString& host, const quint32& port)
{
	m_tcpClient->connectToHost(host, port);
}

void BaseTcpDeviceController::disconnectFromHostInternalSlot()
{
	m_tcpClient->disconnectFromHost();
}

void BaseTcpDeviceController::sendDataInternalSlot(const IMessage<QByteArray>* message)
{
	QByteArray decodedData = m_tcpDeviceCoder->decode(message);
	m_tcpClient->writeData(decodedData);
}

