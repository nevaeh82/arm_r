#include <Logger.h>

#include <Interfaces/IRpcListener.h>
#include <Rpc/RpcRoutedServer.h>

#include "TcpManager.h"

TcpManager::TcpManager(QObject* parent)
	: QObject(parent)
	, m_rpcServer( NULL )
	, m_flakonController( NULL )
{
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

	//Uncomment this if you want simulate sending bpla points from R to OD through RPC
	//connect(&m_timer, SIGNAL(timeout()), this, SLOT(emulateBplaPoint()));
	//m_timer.start(5);
}

TcpManager::~TcpManager()
{
	emit threadTerminateSignal();
}

void TcpManager::addStationToFlakon(QString name, BaseTcpDeviceController* controller)
{
	if (m_flakonController != NULL) {
		m_flakonController->stations().insert(name, controller);
	} else {
		m_flakonStations.insert(name, controller);
	}
}

void TcpManager::addTcpDevice(const QString& deviceName, const int& type)
{
	log_debug(QString("Creating %1 with type %2").arg(QString(deviceName)).arg(type));
	//	QString key = host + ":" + QString::number(port);

	TcpDeviceController* controller = NULL;

	switch(type)
	{
		case RETRRANSLATOR_TCP_DEVICE:
			break;
		case FLAKON_TCP_DEVICE:
			controller = new TcpFlakonController(deviceName);
			log_debug(QString("Created TcpFlakonController"));

			m_flakonController = (TcpFlakonController*) controller;

			// add all added before station devices to Flakon controller
			foreach( QString name, m_flakonStations.keys() ) {
				m_flakonController->stations().insert( name, m_flakonStations.value(name) );
			}
			m_flakonStations.clear();

			break;
		case ATLANT_TCP_DEVICE:
			controller = new TcpAtlantController(deviceName);
			log_debug(QString("Created TcpAtlantController"));


			break;
		case PRM300_TCP_DEVICE:
			controller = new TcpPRM300Controller(deviceName);
			log_debug(QString("Created TcpPRM300Controller"));

			addStationToFlakon(deviceName, controller);
			break;
		default:
			return;
			break;
	}

	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		log_debug(QString("Unable to create %1 with %2").arg(deviceName).arg(type));
		return;
	}

	controller->createTcpClient();
	controller->createTcpDeviceCoder();

	QThread* controllerThread = new QThread;
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));
	controller->asQObject()->moveToThread(controllerThread);
	controllerThread->start();

	m_controllersMap.insert(deviceName, controller);

	controller->registerReceiver(this);
	if (type == FLAKON_TCP_DEVICE) {
		controller->registerReceiver(m_coordinatesCounter);
	}

	controller->connectToHost();

	// register controller as listener for RPC server
	RpcRoutedServer *routedServer = dynamic_cast<RpcRoutedServer *>( m_rpcServer );
	if (routedServer != NULL) {
		routedServer->registerReceiver( (IRpcListener*) controller, controller->getRouteId() );
	}

	log_debug(QString("Added device connection for %1 with %2").arg(deviceName).arg(type));
}

void TcpManager::removeTcpDevice(const QString& deviceName)
{
	if (!m_controllersMap.contains(deviceName)) {
		log_debug(QString("Map doesn't contain %1").arg(deviceName));
		return;
	}

	BaseTcpDeviceController* controller = m_controllersMap.value(deviceName, NULL);
	if (controller == NULL) {
		return;
	}

	// deregister controller from listeners for RPC server
	RpcRoutedServer *routedServer = dynamic_cast<RpcRoutedServer *>( m_rpcServer );
	if (routedServer != NULL) {
		routedServer->deregisterReceiver( (IRpcListener*) controller );
	}

	// remote controller from flakon stations list
	m_flakonStations.remove(deviceName);
	if (m_flakonController != NULL) {
		m_flakonController->stations().remove(deviceName);
	}

	controller->deregisterReceiver(this);
	controller->disconnectFromHost();
	m_controllersMap.remove(deviceName);
}

void TcpManager::setRpcServer(IRpcControllerBase* rpcServer)
{
	if (rpcServer == m_rpcServer) return;

	RpcRoutedServer *routedServer;

	// deregister controllers from old server
	routedServer = dynamic_cast<RpcRoutedServer *>( m_rpcServer );
	if (m_rpcServer != NULL) {
		foreach (TcpDeviceController *controller, m_controllersMap) {
			routedServer->deregisterReceiver( (IRpcListener*) controller );
		}
	}

	m_rpcServer = rpcServer;

	// register controllers for new one server
	routedServer = dynamic_cast<RpcRoutedServer *>( m_rpcServer );
	if (routedServer != NULL) {
		foreach (TcpDeviceController *controller, m_controllersMap) {
			routedServer->registerReceiver( (IRpcListener*) controller, controller->getRouteId() );
		}
	}
}

void TcpManager::setTcpServer(ITcpServerController *tcpServer)
{
	m_tcpServer = tcpServer;
}

QObject* TcpManager::asQObject()
{
	return this;
}

void TcpManager::onMessageReceived(const quint32 deviceType, const QString& deviceName, const MessageSP argument)
{
	QString messageType = argument->type();
	QVariant data = QVariant( argument->data() );

	IRpcListener *sender = NULL;
	if(deviceName == FLAKON_COORDINATE_COUNTER) {
		/// HACK, fix it
		sender =(IRpcListener*)m_controllersMap.value("Флакон", NULL);
	}
	else {
		sender = (IRpcListener*)m_controllersMap.value(deviceName,NULL);
	}

	if(sender == NULL && deviceType != ARMR_TCP_SERVER){
		return;
	}

	switch(deviceType) {
		case FLAKON_TCP_DEVICE:
			if (messageType == TCP_FLAKON_ANSWER_FFT) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_POINTS, data, sender );
			}
			else if (messageType == TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, data, sender );
			}
			else if (messageType == TCP_FLAKON_ANSWER_CORRELATION) {
				/// send data to FlakonCoordinatesCounter
				m_rpcServer->call( FLAKON_COORDINATE_COUNTER, data, sender );
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_CORRELATION, data, sender );
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, data);
			}
			else if (messageType == TCP_FLAKON_STATUS) {
				m_rpcServer->call( RPC_SLOT_FLAKON_STATUS, data, sender );
			}
			break;
		case ATLANT_TCP_DEVICE:
			if (messageType == TCP_ATLANT_ANSWER_DIRECTION) {
				m_rpcServer->call( RPC_SLOT_SERVER_ATLANT_DIRECTION, data);
			}
			else if (messageType == TCP_ATLANT_ANSWER_POSITION) {
				m_rpcServer->call( RPC_SLOT_SERVER_ATLANT_POSITION, data, sender );
			}
			break;
		case PRM300_TCP_DEVICE:
			if (messageType == TCP_PRM300_ANSWER_STATUS) {
				m_rpcServer->call( RPC_SLOT_SERVER_PRM_STATUS, data, sender );
			}else if(messageType == TCP_PRM300_STATUS) {
				m_rpcServer->call( RPC_SLOT_SERVER_STATUS, data, sender );
			}
			break;
		case ARMR_TCP_SERVER:
			if (messageType == TCP_ARMR_SEND_SOLVER_DATA) {
				if (m_coordinatesCounter == NULL) {
					return;
				}
				m_coordinatesCounter->sendData(
							MessageSP(new Message<QByteArray>(
										  TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER,
										  argument->data())));
			} else if (messageType == TCP_ARMR_SEND_SOLVER_CLEAR) {
				if (m_coordinatesCounter == NULL) {
					return;
				}
				m_coordinatesCounter->sendData(
							MessageSP(new Message<QByteArray>(
										  TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER_CLEAR,
										  argument->data())));
			}

		default:
			break;
	}
}

void TcpManager::emulateBplaPoint(IRpcListener *sender)
{
	static double centerLat = 60 + (double)qrand() / RAND_MAX * 0.2;
	static double centerLon = 30 + (double)qrand() / RAND_MAX;
	static int alt = 1500 + qrand() % 1000;
	static QVector<QPointF> path;

	static int mode = 1;
	static int bearing = 1;
	static int angle = 0;


	// calculate new position of BPLA
	angle++;
	double radius = 0.005 + (double)qrand() / RAND_MAX * 0.005;
	double lon = cos((180 - angle) * M_PI / 180) * radius + centerLon;
	double lat = sin((180 - angle) * M_PI / 180) * radius + centerLat;

	path << QPointF( lat, lon );

	// limit path points
	if (path.size() > 50) {
		path.remove( 0, path.size() - 50 );
	}

	// prepare data to send
	QByteArray data;
	QDataStream stream( &data, QIODevice::WriteOnly );

	stream << QTime();
	stream << mode;
	stream << path.last();
	stream << path;
	stream << (double) (500 + qrand() % 1000);
	stream << (double) alt;
	stream << (double) bearing;

	m_rpcServer->call(RPC_SLOT_SERVER_SEND_BPLA_DEF, QVariant(data), sender);
}

void TcpManager::onMethodCalled(const QString& method, const QVariant& argument)
{
	emit onMethodCalledInternalSignal(method, argument);
}

void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
	/// todo: this methods doesn't called from any RPC clients, move to TcpDeviceControoler child
	if (false) {}
	else if (method == RPC_METHOD_SET_DATA_TO_SOLVER) {
		if (m_coordinatesCounter == NULL) {
			return;
		}

		m_coordinatesCounter->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER, argument.toByteArray())));
	}
	else if (method == RPC_METHOD_SET_CLEAR_TO_SOLVER) {
		if (m_coordinatesCounter == NULL) {
			return;
		}

		m_coordinatesCounter->sendData(MessageSP(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER_CLEAR, argument.toByteArray())));
	}
}
