#include <Logger/Logger.h>

#include <Interfaces/IRpcListener.h>
#include <Rpc/RpcRoutedServer.h>

#include "TcpManager.h"

TcpManager::TcpManager(int serverId, QObject* parent)
	: QObject(parent)
	, m_rpcServer( NULL )
	, m_flakonController( NULL )
	, m_rdsController( NULL )
	, m_timePoints(QTime::currentTime())
	, m_isCorrelAfterPoints(false)
	, m_rpcConfigReader(NULL)
    , m_serverId(serverId)
{
    m_coordinatesCounter = new CoordinateCounter("RDS");
	m_coordinatesCounter->registerReceiver(this);

	QThread* coordinateCounterThread = new QThread;
	connect(coordinateCounterThread, SIGNAL(started()), m_coordinatesCounter, SLOT(initSolver()));
	connect(m_coordinatesCounter, SIGNAL(signalFinished()), coordinateCounterThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), coordinateCounterThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), m_coordinatesCounter, SLOT(deleteLater()));
	connect(coordinateCounterThread, SIGNAL(finished()), coordinateCounterThread, SLOT(deleteLater()));
	m_coordinatesCounter->moveToThread(coordinateCounterThread);
	coordinateCounterThread->start();


	m_pServer = new PServer(10240);
	m_coordinatesCounter->registerReceiver(m_pServer);

	QThread* pServerThread = new QThread;
	connect(pServerThread, SIGNAL(started()), m_pServer, SLOT(startServer()));
	connect(m_pServer, SIGNAL(signalFinished()), pServerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), pServerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), m_pServer, SLOT(deleteLater()));
	connect(pServerThread, SIGNAL(finished()), pServerThread, SLOT(deleteLater()));
	m_pServer->moveToThread(pServerThread);
	pServerThread->start();

	QThread* clientServerThread = new QThread;
	m_clientTcpServer = new ClientTcpServer;
	m_coordinatesCounter->registerReceiver(m_clientTcpServer);
	m_clientTcpServer->getSolverEncoder()->registerReceiver(m_coordinatesCounter);

	connect(clientServerThread, SIGNAL(started()), m_clientTcpServer, SLOT(startServer()));
	connect(m_clientTcpServer, SIGNAL(destroyed()), clientServerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), clientServerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), m_clientTcpServer, SLOT(deleteLater()));
	connect(clientServerThread, SIGNAL(finished()), m_clientTcpServer, SLOT(stopServer()));
	connect(clientServerThread, SIGNAL(finished()), clientServerThread, SLOT(deleteLater()));
	m_clientTcpServer->moveToThread(clientServerThread);
	clientServerThread->start();

	connect(this, SIGNAL(onMethodCalledInternalSignal(QString,QVariant)), this, SLOT(onMethodCalledInternalSlot(QString,QVariant)));

	//Uncomment this if you want simulate sending bpla points from R to OD through RPC
	//connect(&m_timer, SIGNAL(timeout()), this, SLOT(emulateBplaPoint()));
	//m_timer.start(5);
}

TcpManager::~TcpManager()
{
	m_coordinatesCounter->deregisterReceiver(m_clientTcpServer);
	m_clientTcpServer->getSolverEncoder()->deregisterReceiver(m_coordinatesCounter);

	emit threadTerminateSignal();
	emit threadTerminateSignalForMapSolver();
	m_mapCoordinateCounter.clear();
}

void TcpManager::addStationToFlakon(QString name, BaseTcpDeviceController* controller)
{
	if (m_flakonController != NULL) {
		m_flakonController->stations().insert(name, controller);
	} else {
		m_flakonStations.insert(name, controller);
	}
}

void TcpManager::addSolver(QByteArray data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	float frequency;
	bool isEnabled;
	ds >> frequency;
	ds >> isEnabled;

	if(isEnabled == false)
	{
		emit threadTerminateSignalForMapSolver();
		m_mapCoordinateCounter.clear();
		return;
	}

	if(!m_mapCoordinateCounter.contains(frequency))
	{
		CoordinateCounter* coordinatesCounter = new CoordinateCounter(FLAKON_COORDINATE_COUNTER);
		coordinatesCounter->registerReceiver(this);

		QThread* coordinateCounterThread = new QThread;
		connect(coordinateCounterThread, SIGNAL(started()), coordinatesCounter, SLOT(initSolver()));
		connect(coordinatesCounter, SIGNAL(signalFinished()), coordinateCounterThread, SLOT(quit()));
		connect(this, SIGNAL(threadTerminateSignalForMapSolver()), coordinateCounterThread, SLOT(quit()));
		connect(this, SIGNAL(threadTerminateSignalForMapSolver()), coordinatesCounter, SLOT(deleteLater()));
		connect(coordinateCounterThread, SIGNAL(finished()), coordinateCounterThread, SLOT(deleteLater()));
		coordinatesCounter->moveToThread(coordinateCounterThread);
		coordinateCounterThread->start();

		coordinatesCounter->registerReceiver(m_pServer);

		m_mapCoordinateCounter.insert(frequency, coordinatesCounter);
	}

	m_flakonController->setCoordinateCounter(m_mapCoordinateCounter.value(frequency));
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

			// add coordinateCounter object to Flacon controller
			m_flakonController->setCoordinateCounter(m_coordinatesCounter);

			// add all added before station devices to Flakon controller
			foreach( QString name, m_flakonStations.keys() ) {
				m_flakonController->stations().insert( name, m_flakonStations.value(name) );
			}
			m_flakonStations.clear();

			break;
		case PRM300_TCP_DEVICE:
			controller = new TcpPRM300Controller(deviceName);
			log_debug(QString("Created TcpPRM300Controller"));

			addStationToFlakon(deviceName, controller);
			break;
		case RDS_TCP_DEVICE:
            controller = new TcpRDSController(m_serverId, deviceName);

			log_debug(QString("Created TcpRdsController"));
			m_rdsController = (TcpRDSController*) controller;

			m_rdsController->setCoordinateCounter(m_coordinatesCounter);

			break;
		case SOLVER_CLIENT_DEVICE:
			//QThread* clientSolverThread = new QThread;
			controller = new SolverClient1(deviceName);
			m_clientSolver = (SolverClient1*)controller;
			m_coordinatesCounter->registerReceiver(m_clientSolver);

			//m_clientSolver->getSolverEncoder()->registerReceiver(this);

//			connect(clientSolverThread, SIGNAL(started()), m_clientSolver, SLOT(connectToSolverServer()));
//			connect(m_clientSolver, SIGNAL(destroyed()), clientSolverThread, SLOT(quit()));
//			connect(this, SIGNAL(threadTerminateSignal()), clientSolverThread, SLOT(quit()));
//			connect(this, SIGNAL(threadTerminateSignal()), m_clientSolver, SLOT(deleteLater()));
//			connect(clientSolverThread, SIGNAL(finished()), m_clientSolver, SLOT(stopServer()));
//			connect(clientSolverThread, SIGNAL(finished()), clientSolverThread, SLOT(deleteLater()));
//			m_clientSolver->moveToThread(clientSolverThread);
//			clientSolverThread->start();
			break;
		default:
			break;
	}

	/// if something else, create new Tcp%Device%Controller with new name and/or class

	if (controller == NULL) {
		log_debug(QString("Unable to create %1 with %2").arg(deviceName).arg(type));
		return;
	}

	controller->createTcpClient();
	controller->createTcpDeviceCoder();

	QThread* controllerThread = new QThread();
	connect(controller->asQObject(), SIGNAL(destroyed()), controllerThread, SLOT(terminate()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(quit()));
	connect(this, SIGNAL(threadTerminateSignal()), controller->asQObject(), SLOT(deleteLater()));
	connect(this, SIGNAL(threadTerminateSignal()), controllerThread, SLOT(deleteLater()));

	controller->moveToThread(controllerThread);
	controllerThread->start();

	log_debug(deviceName + QString("Status %1").arg(controllerThread->isRunning()));

	m_controllersMap.insert(deviceName, controller);

	controller->registerReceiver(this);
	if(type == RDS_TCP_DEVICE) {
		m_rdsController->registerReceiver(m_coordinatesCounter);
	} else if(type == SOLVER_CLIENT_DEVICE) {
		m_clientSolver->getSolverEncoder()->registerReceiver(m_coordinatesCounter);
		m_clientSolver->registerReceiver(this);
	} else if (type == FLAKON_TCP_DEVICE) {
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
    } else if(deviceName == RDS_NAME ) {
        sender = (IRpcListener*)m_controllersMap.value("rds", NULL);
    }
	else {
		sender = (IRpcListener*)m_controllersMap.value(deviceName,NULL);
	}

	if(sender == NULL && deviceType != ARMR_TCP_SERVER){
		return;
	}

	switch(deviceType) {
		case RDS_TCP_DEVICE:
			if (messageType == TCP_FLAKON_ANSWER_FFT) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_POINTS, data, sender );

				//log_debug("SEND RDS POINTS!");
			}
			else if(messageType == TCP_RDS_ANSWER_SYSTEM) {
				if(m_rpcConfigReader) {
					m_rpcConfigReader->inStationsList(data);
					log_debug("SEND RDS CONFIGURATION!");
				}
			} else if (messageType == TCP_FLAKON_ANSWER_CORRELATION) {
					/// send data to FlakonCoordinatesCounter
					//m_rpcServer->call( FLAKON_COORDINATE_COUNTER, data, sender );
				IRpcListener *sender1 = NULL;
				sender1 =(IRpcListener*)m_controllersMap.value("Флакон", NULL);
					m_rpcServer->call( FLAKON_COORDINATE_COUNTER, data, sender1 );
					m_rpcServer->call( RPC_SLOT_SERVER_SEND_CORRELATION, data, sender );

				//log_debug("to RPC FLAKON_COORDINATE_COUNTER and RPC_SLOT_SERVER_SEND_CORRELATION");
			} else if( messageType == TCP_RDS_ANSWER_LOCSYSTEM ) {
				m_rpcServer->call(RPC_METHOD_CONFIG_RDS_ANSWER, data);
//				IRpcListener *sender1 = NULL;
//				sender1 =(IRpcListener*)m_controllersMap.value("Флакон", NULL);
//					m_rpcServer->call( FLAKON_COORDINATE_COUNTER, data, sender1 );
			} else if (messageType == TCP_FLAKON_STATUS) {
				m_rpcServer->call( RPC_SLOT_FLAKON_STATUS, data, sender );
				m_rpcServer->call( RPC_SLOT_FLAKON_DEV_STATE, data, sender );
			} else if (messageType == TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, data, sender );
			}
			else if( messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_1 ) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF_1, data);
			}
			else if( messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT_1 ) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_RESULT_1, data);
			}
			else if( messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_SOLVER_SETUP_1 ) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ERRORS) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_SOLVER_ERRORS, data, sender );
			}
			break;
		case SOLVER_CLIENT_DEVICE:
			{
				if( messageType != TCP_SOLVER_MESSAGE ) {
					break;
				}

				QByteArray inData = data.toByteArray();

				if( inData == QByteArray() ) {
					break;
				} else {
					m_clientSolver->getSolverEncoder()->readProtobuf( inData );
				}
			}
			break;
		case FLAKON_TCP_DEVICE:
			if (messageType == TCP_FLAKON_ANSWER_FFT) {

				m_rpcServer->call( RPC_SLOT_SERVER_SEND_POINTS, data, sender );

				log_debug("to RPC RPC_SLOT_SERVER_SEND_POINTS   >>>  %1");
			}
			else if (messageType == TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, data, sender );
			}
			else if (messageType == TCP_FLAKON_ANSWER_CORRELATION) {
				/// send data to FlakonCoordinatesCounter
				m_rpcServer->call( FLAKON_COORDINATE_COUNTER, data, sender );
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_CORRELATION, data, sender );

			//log_debug("to RPC FLAKON_COORDINATE_COUNTER and RPC_SLOT_SERVER_SEND_CORRELATION");
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF_AUTO, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_SINGLE) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF_SINGLE, data);
			}
			else if( TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_1 ) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_DEF_1, data);
			}
			else if( TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT_1 ) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_RESULT_1, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_BPLA_RESULT, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ANSWER_HYPERBOLA) {
				//FROM COORDINATES COUNTER
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_HYPERBOLA, data);
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_QUALITY_STATUS) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_QUALITY_STATUS, data, sender );
			}
			else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_ERRORS) {
				m_rpcServer->call( RPC_SLOT_SERVER_SEND_SOLVER_ERRORS, data, sender );
			}
			else if (messageType == TCP_FLAKON_STATUS) {
				m_rpcServer->call( RPC_SLOT_FLAKON_STATUS, data, sender );
			} else if(messageType == RPC_CORRELATION_CONTROL) {
				addSolver(data.toByteArray());
			}
			break;
		case PRM300_TCP_DEVICE:
			if (messageType == TCP_PRM300_ANSWER_STATUS) {
				m_rpcServer->call( RPC_SLOT_SERVER_PRM_STATUS, data, sender );
			}else if(messageType == TCP_PRM300_STATUS) {
				m_rpcServer->call( RPC_SLOT_SERVER_STATUS, data, sender );
			}else if(messageType == TCP_PRM300_FREQUENCY_CHANGED)
			{
				m_rpcServer->call( RPC_SLOT_PRM300_FREQUENCY_CHANGED, data, sender );
			}
			break;
		case ARMR_TCP_SERVER:
			if (messageType == TCP_ARMR_SEND_SOLVER_DATA) {
				CoordinateCounter* currentCoordinateCounter;
				if(m_mapCoordinateCounter.contains(m_currentFrequencyCorrelation))
				{
					currentCoordinateCounter = m_mapCoordinateCounter.value(m_currentFrequencyCorrelation);
				}
				else
				{
					currentCoordinateCounter = m_coordinatesCounter;
				}
				if(currentCoordinateCounter == NULL)
				{
					return;
				}
				currentCoordinateCounter->sendData(
							MessageSP(new Message<QByteArray>(
										  TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER,
										  argument->data())));
			} else if (messageType == TCP_ARMR_SEND_SOLVER_CLEAR) {
				CoordinateCounter* currentCoordinateCounter;
				if(m_mapCoordinateCounter.contains(m_currentFrequencyCorrelation))
				{
					currentCoordinateCounter = m_mapCoordinateCounter.value(m_currentFrequencyCorrelation);
				}
				else
				{
					currentCoordinateCounter = m_coordinatesCounter;
				}
				if(currentCoordinateCounter == NULL)
				{
					return;
				}
				currentCoordinateCounter->sendData(
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

void TcpManager::setConfigReader(RpcConfigReader* reader)
{
	m_rpcConfigReader = reader;

	if(m_rdsController) {
		connect(m_rpcConfigReader, SIGNAL(getStationList()), m_rdsController, SLOT(onGetStations()));
		connect(m_rdsController, SIGNAL(outStationsList(QList<StationConfiguration>)),
				m_rpcConfigReader, SLOT(inStationsList(QList<StationConfiguration>)));
	}
}

void TcpManager::onMethodCalledInternalSlot(const QString& method, const QVariant& argument)
{
	if(method == RPC_METHOD_SS_CORRELATION)
	{
		addSolver(argument.toByteArray());
		/// TODO Create solver om frequency
	}
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
