#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpManager));
	m_rpcServer = NULL;

	m_coordinatesCounter = new CoordinateCounter(FLAKON_COORDINATE_COUNTER);
	m_coordinatesCounter->registerReceiver(this);

	QThread* coordinateCounterThread = new QThread;
	connect(coordinateCounterThread, SIGNAL(started()), m_coordinatesCounter, SLOT(initSolver()));
	connect(m_coordinatesCounter, SIGNAL(signalFinished()), coordinateCounterThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), coordinateCounterThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), m_coordinatesCounter, SLOT(deleteLater()));
	connect(coordinateCounterThread, SIGNAL(finished()), coordinateCounterThread, SLOT(deleteLater()));
	m_coordinatesCounter->moveToThread(coordinateCounterThread);
	coordinateCounterThread->start();

	connect(this, SIGNAL(onMethodCalledInternalSignal(QString,QVariant)), this, SLOT(onMethodCalledInternalSlot(QString,QVariant)));
}

TcpManager::~TcpManager()
{
	emit threadTerminateSignal();
}

void TcpManager::addTcpDevice(const QString& deviceType, const QString& host, const quint32& port)
{
	m_logger->debug(QString("Creating %1").arg(deviceType));
	QString key = host + ":" + QString::number(port);

	BaseTcpDeviceController* controller = NULL;

	if (deviceType == FLAKON_TCP_DEVICE) {
		controller = new TcpFlakonController(FLAKON_TCP_DEVICE);
		m_logger->debug(QString("Created TcpFlakonController"));
	} else if (deviceType == ATLANT_TCP_DEVICE) {
		controller = new TcpAtlantController(ATLANT_TCP_DEVICE);
		m_logger->debug(QString("Created TcpAtlantController"));
	} else if (deviceType == PRM300_TCP_DEVICE) {
		controller = new TcpPRM300Controller(PRM300_TCP_DEVICE);
		m_logger->debug(QString("Created TcpPRM300Controller"));
	}
	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		m_logger->debug(QString("Unable to create %1 with %2").arg(deviceType).arg(key));
		return;
	}

	QThread* controllerThread = new QThread;
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	m_controllersMap.insert(deviceType, controller);

	controller->registerReceiver(this);
	if (deviceType == FLAKON_TCP_DEVICE) {
		controller->registerReceiver(m_coordinatesCounter);
	}

	controller->createTcpDeviceCoder();
	controller->createTcpClient();
	controller->connectToHost(host, port);

	m_logger->debug(QString("Added device connection for %1 with %2").arg(deviceType).arg(key));
}

void TcpManager::removeTcpDevice(const QString& deviceType, const QString& host, const quint32& port)
{
	if (!m_controllersMap.contains(deviceType)) {
		m_logger->debug(QString("Map doesn't contain %1").arg(deviceType));
		return;
	}

	BaseTcpDeviceController* controller = m_controllersMap.value(deviceType, NULL);
	if (controller == NULL) {
		return;
	}

	controller->deregisterReceiver(this);
	controller->disconnectFromHost();
	m_controllersMap.remove(deviceType);
}

void TcpManager::setRpcServer(IRPC* rpcServer)
{
	m_rpcServer = rpcServer;

}

void TcpManager::setTcpServer(ITcpServer *tcpServer)
{
	m_tcpServer = tcpServer;
}

QObject* TcpManager::asQObject()
{
	return this;
}

void TcpManager::onMessageReceived(const QString& device, const MessageSP argument)
{
	/// TODO : refactor it. It's bad.

	QString messageType = argument->type();
	QByteArray messageData = argument->data();

	if (device == FLAKON_TCP_DEVICE) {
		if (messageType == TCP_FLAKON_ANSWER_FFT) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_POINTS, messageData);
		}
		else if (messageType == TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, messageData);
		}
		else if (messageType == TCP_FLAKON_ANSWER_CORRELATION) {
			/// send data to FlakonCoordinatesCounter
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_CORRELATION, messageData);
		}
		else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_DEF, messageData);
		}
		else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, messageData);
		}
	} else if (device == ATLANT_TCP_DEVICE) {
		if (messageType == TCP_ATLANT_ANSWER_DIRECTION) {
			/*TODO: REMOVE RPCSERVER USAGE*/
			m_tcpServer->sendData(messageData);
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_ATLANT_DIRECTION, messageData);
		}
		else if (messageType == TCP_ATLANT_ANSWER_POSITION) {
			m_tcpServer->sendData(messageData);
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_ATLANT_POSITION, messageData);
		}
	} else if (device == PRM300_TCP_DEVICE) {
		if (messageType == TCP_PRM300_ANSWER_STATUS) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_PRM_STATUS, messageData);
		}
	}

	/*else if (method == RPC_SLOT_SET_DATA_TO_SOLVER) {
		if (m_coordinatesCounter == NULL) {
			return;
		}

		m_coordinatesCounter->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SET_CLEAR_TO_SOLVER) {
		if (m_coordinatesCounter == NULL) {
			return;
		}

		m_coordinatesCounter->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray())));
	}*/
}

void TcpManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	emit onMethodCalledInternalSignal(method,argument);
}

void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
	if (method == RPC_SLOT_SET_MAIN_STATION_COR) {
		BaseTcpDeviceController* controller = m_controllersMap.value(FLAKON_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SET_BANDWIDTH) {
		BaseTcpDeviceController* controller = m_controllersMap.value(FLAKON_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_SET_BANDWIDTH, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SET_SHIFT) {
		BaseTcpDeviceController* controller = m_controllersMap.value(FLAKON_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_SET_SHIFT, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_RECOGNIZE) {
		BaseTcpDeviceController* controller = m_controllersMap.value(FLAKON_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}
		controller->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_RECOGNIZE, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SS_CORRELATION) {
		BaseTcpDeviceController* controller = m_controllersMap.value(FLAKON_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_SS_CORRELATION, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_AVARAGE_SPECTRUM) {
		BaseTcpDeviceController* controller = m_controllersMap.value(FLAKON_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_PRM_SET_FREQ) {
		BaseTcpDeviceController* controller = m_controllersMap.value(PRM300_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_PRM300_REQUEST_SET_FREQUENCY, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_PRM_REQUEST_FREQ) {
		BaseTcpDeviceController* controller = m_controllersMap.value(PRM300_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_PRM300_REQUEST_GET_FREQUENCY, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_PRM_SET_ATT1) {
		BaseTcpDeviceController* controller = m_controllersMap.value(PRM300_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_PRM300_REQUEST_SET_ATTENUER_ONE, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_PRM_SET_ATT2) {
		BaseTcpDeviceController* controller = m_controllersMap.value(PRM300_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_PRM300_REQUEST_SET_ATTENUER_TWO, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_PRM_SET_FILTER) {
		BaseTcpDeviceController* controller = m_controllersMap.value(PRM300_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_PRM300_REQUEST_SET_FILTER, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SET_ATLANT_FREQUENCY) {
		BaseTcpDeviceController* controller = m_controllersMap.value(ATLANT_TCP_DEVICE, NULL);
		if (controller == NULL) {
			return;
		}

		controller->sendData(MessageSP(new Message<QByteArray>(TCP_ATLANT_REQUEST_SET_FREQUENCY, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SET_DATA_TO_SOLVER) {
		if (m_coordinatesCounter == NULL) {
			return;
		}

		m_coordinatesCounter->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray())));
	}
	else if (method == RPC_SLOT_SET_CLEAR_TO_SOLVER) {
		if (m_coordinatesCounter == NULL) {
			return;
		}

		m_coordinatesCounter->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray())));
	}
}
