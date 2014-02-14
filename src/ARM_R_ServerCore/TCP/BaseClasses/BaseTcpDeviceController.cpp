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
	connect(this, SIGNAL(sendDataInternalSignal(QByteArray)), this, SLOT(sendDataInternalSlot(QByteArray)));
	connect(this, SIGNAL(onDataReceivedInternalSignal(QVariant)), this, SLOT(onDataReceivedInternalSlot(QVariant)));
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

void BaseTcpDeviceController::sendData(const QByteArray& data)
{
	emit sendDataInternalSignal(data);
}

QObject* BaseTcpDeviceController::asQObject()
{
	return this;
}

void BaseTcpDeviceController::onDataReceived(const QVariant& argument)
{
	emit onDataReceivedInternalSignal(argument);
}

void BaseTcpDeviceController::connectToHostInternalSlot(const QString& host, const quint32& port)
{
	m_tcpClient->connectToHost(host, port);
}

void BaseTcpDeviceController::disconnectFromHostInternalSlot()
{
	m_tcpClient->disconnectFromHost();
}

void BaseTcpDeviceController::sendDataInternalSlot(const QByteArray& data)
{
	QByteArray encodedData = m_tcpDeviceCoder->encode(data);
	m_tcpClient->writeData(encodedData);
}

void BaseTcpDeviceController::onDataReceivedInternalSlot(const QVariant& argument)
{
	QByteArray decodedData = m_tcpDeviceCoder->decode(argument.toByteArray());

	foreach(ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(decodedData));
	}
}
