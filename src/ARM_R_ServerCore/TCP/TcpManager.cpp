#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpManager));
	m_rpcServer = NULL;

	m_coordinatesCounter = new CoordinateCounter(FLAKON_COORDINATE_COUNTER);
	m_coordinatesCounter->registerReceiver(this);

	QThread* coordinateCounterThread = new QThread;
	connect(m_coordinatesCounter, SIGNAL(signalFinished()), coordinateCounterThread, SLOT(quit()));
	connect(this, SIGNAL(finished()), m_coordinatesCounter, SLOT(deleteLater()));
	connect(coordinateCounterThread, SIGNAL(finished()), coordinateCounterThread, SLOT(deleteLater()));
	m_coordinatesCounter->moveToThread(coordinateCounterThread);
	coordinateCounterThread->start();
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
		controller->registerReceiver(m_coordinatesCounter);
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
//			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_CORRELATION, messageData);
		}
		else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_DEF, messageData);
		}
		else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, messageData);
		}
	} else if (device == ATLANT_TCP_DEVICE) {
		if (messageType == TCP_ATLANT_ANSWER_DIRECTION) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_ATLANT_DIRECTION, messageData);
		}
		else if (messageType == TCP_ATLANT_ANSWER_POSITION) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_ATLANT_POSITION, messageData);
		}
	} else if (device == PRM300_TCP_DEVICE) {
		if (messageType == TCP_PRM300_ANSWER_STATUS) {
			m_rpcServer->sendDataByRpc(RPC_SLOT_SERVER_PRM_STATUS, messageData);
		}
	}
}

void TcpManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	if (method == RPC_SLOT_SET_MAIN_STATION_COR) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION, argument.toByteArray());
		m_controllersMap.value(FLAKON_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_AVARAGE_SPECTRUM) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM, argument.toByteArray());
		m_controllersMap.value(FLAKON_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_PRM_SET_FREQ) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_PRM300_REQUEST_SET_FREQUENCY, argument.toByteArray());
		m_controllersMap.value(PRM300_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_PRM_REQUEST_FREQ) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_PRM300_REQUEST_GET_FREQUENCY, argument.toByteArray());
		m_controllersMap.value(PRM300_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_PRM_SET_ATT1) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_PRM300_REQUEST_SET_ATTENUER_ONE, argument.toByteArray());
		m_controllersMap.value(PRM300_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_PRM_SET_ATT2) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_PRM300_REQUEST_SET_ATTENUER_TWO, argument.toByteArray());
		m_controllersMap.value(PRM300_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_PRM_SET_FILTER) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_PRM300_REQUEST_SET_FILTER, argument.toByteArray());
		m_controllersMap.value(ATLANT_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_SET_ATLANT_FREQUENCY) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_ATLANT_REQUEST_SET_FREQUENCY, argument.toByteArray());
		m_controllersMap.value(ATLANT_TCP_DEVICE)->sendData(message);
	}
	else if (method == RPC_SLOT_SET_DATA_TO_SOLVER) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray());
		m_coordinatesCounter->sendData(message);
	}
	else if (method == RPC_SLOT_SET_CLEAR_TO_SOLVER) {
		IMessage<QByteArray>* message = new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray());
		m_coordinatesCounter->sendData(message);
	}

}
