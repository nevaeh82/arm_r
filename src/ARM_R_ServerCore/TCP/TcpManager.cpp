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

void TcpManager::addConnection(const QString& deviceType, const QString& host, const quint32& port)
{
	QString key = host + ":" + QString::number(port);

	BaseTcpDeviceController* controller = NULL;

	if (deviceType == FLAKON_TCP_DEVICE) {
		controller = new TcpFlakonController;
	} else if (deviceType == ATLANT_TCP_DEVICE) {
		controller = new TcpAtlantController;
	} else if (deviceType == PRM300_TCP_DEVICE) {
		controller = new TcpPRM300Controller;
	}

	if (controller == NULL) {
		m_logger->debug(QString("Unable to create %1 with %2").arg(deviceType).arg(key));
		return;
	}

	controller->connectToHost(host, port);
	controller->registerReceiver(this);

	QThread* controllerThread = new QThread;
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);

	QMap<QString, BaseTcpDeviceController*> controllersInternalMap = m_controllersMap.value(key, deviceType);
	controllersInternalMap.insert(key, controller);
	m_controllersMap.insert(deviceType, controllersInternalMap);

	controllerThread->start();

	m_logger->debug(QString("Added device connection for %1 with %2").arg(deviceType).arg(key));
}

void TcpManager::removeConnection(const QString& deviceType, const QString& host, const quint32& port)
{
	if (!m_controllersMap.contains(deviceType)) {
		m_logger->debug(QString("Map doesn't contain %1").arg(deviceType));
		return;
	}

	/// TODO
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
}
