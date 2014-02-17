#include "BaseTcpDeviceController.h"

BaseTcpDeviceController::BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;

	m_tcpClient = new BaseTcpClient(this);
	m_tcpClient->registerReceiver(this);

	m_tcpDeviceCoder = new BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(BaseTcpDeviceCoder)), this);
	m_tcpDeviceName = BASE_TCP_DEVICE;

	connect(this, SIGNAL(connectToHostInternalSignal(QString,quint32)), this, SLOT(connectToHostInternalSlot(QString,quint32)));
	connect(this, SIGNAL(disconnectFromHostInternalSignal()), this, SLOT(disconnectFromHostInternalSlot()));
	connect(this, SIGNAL(sendDataInternalSignal(const IMessage<QByteArray>*)), this, SLOT(sendDataInternalSlot(const IMessage<QByteArray>*)));
	connect(this, SIGNAL(onDataReceivedInternalSignal(QVariant)), this, SLOT(onDataReceivedInternalSlot(QVariant)));
}

BaseTcpDeviceController::BaseTcpDeviceController(const QString& tcpDeviceName, Pw::Logger::ILogger* logger, QObject* parent) :
	QObject(parent)
{
	m_logger = logger;

	m_tcpClient = new BaseTcpClient(this);
	m_tcpClient->registerReceiver(this);

	m_tcpDeviceCoder = new BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(BaseTcpDeviceCoder)), this);
	m_tcpDeviceName = tcpDeviceName;

	connect(this, SIGNAL(connectToHostInternalSignal(QString,quint32)), this, SLOT(connectToHostInternalSlot(QString,quint32)));
	connect(this, SIGNAL(disconnectFromHostInternalSignal()), this, SLOT(disconnectFromHostInternalSlot()));
	connect(this, SIGNAL(sendDataInternalSignal(const IMessage<QByteArray>*)), this, SLOT(sendDataInternalSlot(const IMessage<QByteArray>*)));
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

QString BaseTcpDeviceController::getHost()
{
	return m_tcpClient->getHost();
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

void BaseTcpDeviceController::sendDataInternalSlot(const IMessage<QByteArray>* message)
{
	QByteArray decodedData = m_tcpDeviceCoder->decode(message);

	if (decodedData.size() <= 0) {
		m_logger->debug(QString("decodedData.size() <= 0 for %1").arg(m_tcpDeviceName));
		return;
	}

	m_tcpClient->writeData(decodedData);
}

void BaseTcpDeviceController::onDataReceivedInternalSlot(const QVariant& argument)
{
	IMessage<QByteArray>* message = m_tcpDeviceCoder->encode(argument.toByteArray());

	if (message == NULL) {
		m_logger->debug(QString("message == NULL for %1").arg(m_tcpDeviceName));
		return;
	}

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(m_tcpDeviceName, message);
	}
}
