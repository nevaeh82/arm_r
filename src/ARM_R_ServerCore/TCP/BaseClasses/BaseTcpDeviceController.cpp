#include "BaseTcpDeviceController.h"

BaseTcpDeviceController::BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;

	m_tcpClient = new BaseTcpClient(this);
	m_tcpClient->registerReceiver(this);

	m_tcpDeviceCoder = new BaseTcpDeviceCoder(this);
}

BaseTcpDeviceController::~BaseTcpDeviceController()
{
	m_tcpClient->deregisterReceiver(this);
}

void BaseTcpDeviceController::connectToHost(const QString& host, const quint32& port)
{
	m_tcpClient->connectToHost(host, port);
}

void BaseTcpDeviceController::disconnectFromHost()
{
	m_tcpClient->disconnectFromHost();
}

void BaseTcpDeviceController::isConnected()
{
	return m_tcpClient->isConnected();
}

void BaseTcpDeviceController::sendData(const QByteArray& data)
{
	QByteArray encodedData = m_tcpDeviceCoder->encode(data);
	m_tcpClient->writeData(encodedData);
}

QObject* BaseTcpDeviceController::asQObject()
{
	return this;
}

void BaseTcpDeviceController::onDataReceived(const QVariant& argument)
{
	QByteArray decodedData = m_tcpDeviceCoder->decode(argument.toByteArray());

	foreach(ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(decodedData));
	}
}
