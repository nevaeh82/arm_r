#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpManager));
	m_rpcServer = NULL;
}

TcpManager::~TcpManager()
{
}

void TcpManager::addTcpDevice(const QString& deviceType, const QString& host, const quint32& port)
{
	QString key = host + ":" + QString::number(port);

	BaseTcpDeviceController* controller = NULL;

	if (deviceType == FLAKON_TCP_DEVICE) {
		controller = new TcpFlakonController(FLAKON_TCP_DEVICE);
	} else if (deviceType == ATLANT_TCP_DEVICE) {
		controller = new TcpAtlantController(ATLANT_TCP_DEVICE);
	} else if (deviceType == PRM300_TCP_DEVICE) {
		controller = new TcpPRM300Controller(PRM300_TCP_DEVICE);
	}
	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		m_logger->debug(QString("Unable to create %1 with %2").arg(deviceType).arg(key));
		return;
	}

	controller->connectToHost(host, port);
	controller->registerReceiver(this);

	QThread* controllerThread = new QThread;
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);

	m_controllersMap.insert(deviceType, controller);

	controllerThread->start();

	m_logger->debug(QString("Added device connection for %1 with %2").arg(deviceType).arg(key));
}

void TcpManager::removeTcpDevice(const QString& deviceType, const QString& host, const quint32& port)
{
	if (!m_controllersMap.contains(deviceType)) {
		m_logger->debug(QString("Map doesn't contain %1").arg(deviceType));
		return;
	}

	BaseTcpDeviceController* controller = m_controllersMap.value(deviceType);
	controller->deregisterReceiver(this);
	controller->disconnectFromHost();
	m_controllersMap.remove(deviceType);
}

void TcpManager::setRpcServer(IRPC* rpcServer)
{
	m_rpcServer = rpcServer;
}

QObject* TcpManager::asQObject()
{
	return this;
}

void TcpManager::onMessageReceived(const QString& device, const IMessage<QByteArray>* argument)
{
	/// TODO

	if (device == FLAKON_TCP_DEVICE) {

	} else if (device == ATLANT_TCP_DEVICE) {

	} else if (device == PRM300_TCP_DEVICE) {

	}
}
