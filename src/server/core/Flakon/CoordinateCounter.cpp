#include <Logger/Logger.h>

#include "CoordinateCounter.h"

#define GOOD_LIMIT 0.7

CoordinateCounter::CoordinateCounter(const QString& deviceName, QObject* parent) :
	QObject(parent)
{
	QDir dir;
	dir.mkdir("./logs/SpecialLogs");

	m_logManager = new LogManager("./logs/SpecialLogs/logDistances.log");
	if(!m_logManager->isFileOpened()) {
		log_debug("error");
	}

	m_logManager1 = new LogManager("./logs/SpecialLogs/logTrajMan.log");
	if(!m_logManager1->isFileOpened()) {
		log_debug("error");
	}

	m_logManager2 = new LogManager("./logs/SpecialLogs/logTrajAuto.log");
	if(!m_logManager2->isFileOpened()) {
		log_debug("error");
	}

	m_logManager3 = new LogManager("./logs/SpecialLogs/logTrajOne.log");
	if(!m_logManager3->isFileOpened()) {
		log_debug("error");
	}

	m_solver = NULL;

	m_corr_threshold = 3;
	m_prevStation = 0;
	m_centerFrequency = -1;

	qRegisterMetaType<DataFromFlacon> ("DataFromFlacon");
	qRegisterMetaType<DataFromRadioLocation> ("DataFromRadioLocation");
	qRegisterMetaType<OneDataFromRadioLocation> ("OneDataFromRadioLocation");
	qRegisterMetaType<ErrorType> ("ErrorType");
	qRegisterMetaType<SolveResult> ("SolveResult");
	qRegisterMetaType<HyperbolesFromRadioLocation> ("HyperbolesFromRadioLocation");

	m_likeADeviceName = deviceName;
	log_debug(QString("Created %1").arg(m_likeADeviceName));

	connect(this, SIGNAL(signalSetCenterFrequency(double)), this, SLOT(slotSetCenterFrequency(double)));
}

CoordinateCounter::~CoordinateCounter()
{
	if(m_solver != NULL) {
		delete m_solver;
	}
	emit signalFinished();
	delete m_logManager;
	delete m_logManager1;
	delete m_logManager2;
	delete m_logManager3;
}

void CoordinateCounter::onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument)
{
	Q_UNUSED( device );

	if (deviceType != FLAKON_TCP_DEVICE) {
		return;
	}

	if (argument->type() != TCP_FLAKON_ANSWER_CORRELATION) {
		return;
	}

	QByteArray inputData = argument->data();
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

	QVector<QPointF> points;
	int point1, point2;

	inputDataStream >> point1 >> point2 >> points;

	m_main_point = point1;
	m_map_vec_corr.insert(point2, points);

	if((m_prevStation > point2) && (m_map_vec_corr.count() > 4)) {
		QVector< QVector<QPointF> > vec_p;
		QMap<int, QVector<QPointF> >::iterator it;

		for(it = m_map_vec_corr.begin(); it != m_map_vec_corr.end(); ++it) {
			vec_p.append(it.value());
		}

		QVector<double> aDR;
		aDR.clear();
		bool isSolverServer = getIsSolverServer();

		ZDR mZDR;
		mZDR.getDataFromFlackon(m_main_point, vec_p, m_corr_threshold, aDR);
		m_map_vec_corr.clear();

		if (aDR.size() == 5) {
			// form data and calculate coordinates
			m_aData.numOfReferenceDetector_= m_main_point; // reference detector number
			m_aData.time_ = QTime::currentTime();	//Time
			m_aData.ranges_ = aDR;					//Adjusted difference in distance (maxima correlation graphs)

			if( !isSolverServer ) {
				m_solver->GetData(m_aData);
			} else {
				m_aData.ranges_.insert(m_main_point, 0);
				sendDataToClientTcpServer();
			}

			QString dataToFile = QTime::currentTime().toString("hh:mm:ss:zzz") + " " + QString::number(aDR.at(0)) + " " + QString::number(aDR.at(1)) + " " + QString::number(aDR.at(2)) + " " + QString::number(aDR.at(3)) + " " + QString::number(aDR.at(4)) + " " + QString::number(m_main_point) + "\n";

			m_logManager->writeToFile(dataToFile);
		}
	}

	m_prevStation = point2;
}

void CoordinateCounter::onSendDataFromRadioLocation(const SolveResult &result, const DataFromRadioLocation &allData)
{
	emit signalGetDataFromRadioLocation(result, allData);
}

void CoordinateCounter::onSendDataFromRadioLocationManualHeigh(const SolveResult &result, const DataFromRadioLocation &allData)
{
	emit signalGetDataFromRadioLocationManualHeight(result, allData);

}

void CoordinateCounter::onSendOneDataFromRadioLocation(const SolveResult &result, const OneDataFromRadioLocation &oneData_1, const OneDataFromRadioLocation &oneData_2)
{
	emit signalGetOneDataFromRadioLocation(result, oneData_1, oneData_2);
}

void CoordinateCounter::onSendHyperbolesFromRadioLocation(const SolveResult &result, const HyperbolesFromRadioLocation &hyperb)
{
	emit signalGetHyperbolesDataFromRadioLocation(result, hyperb);
}

void CoordinateCounter::onErrorOccured(const ErrorType &error_type, const QString &str)
{
	emit signalError((int)error_type, str);
}

void CoordinateCounter::sendData(const MessageSP message)
{
	QString messageType = message->type();

	if (messageType == TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER) {
		QByteArray messageData = message->data();
		/*TODO: REPLACE DATASTREAM WITH PROTOBUF WHEN TCPSERVER WILL BE TESTED*/
		/*
		Zaviruha::Packet packet;
		if (!packet.ParseFromArray(messageData, messageData.size())){
			return;
		}

		double alt = packet.command().arguments().solverdata(0).altitude();*/

		int id;
		int track_length;
		double alt;

		QDataStream dataStream(&messageData, QIODevice::ReadOnly);
		dataStream >> id >> track_length >> alt;

		m_solver->SetHeighApriori(alt);
		m_alt = alt;
	}
	else if (messageType == TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER_CLEAR) {
		m_solver->Clear();
	}
}

QObject* CoordinateCounter::asQObject()
{
	return this;
}

void CoordinateCounter::setCenterFrequency(const double& frequency)
{
	emit signalSetCenterFrequency(frequency);
}

void CoordinateCounter::slotCatchDataFromRadioLocationAuto(const SolveResult &result, const DataFromRadioLocation &aData)
{
	if( aData.timeHMSMs.size() == 0 ) {
		return;
	}

	int sourceType = AUTO_HEIGH;
	int aLastItem = aData.timeHMSMs.size() - 1;

	if( result == SOLVED ) {
		QByteArray dataToSend;
		QDataStream ds(&dataToSend, QIODevice::WriteOnly);

		ds << encodeSolverData(aData);

		/// @deprecated Now source type determinates by TCP/RPC message type
		ds << sourceType;

		MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO, dataToSend));

		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, message);
		}

		//Send status to ARM-R UI

		QByteArray statusBA;
		QDataStream dataStreamStatus(&statusBA, QIODevice::WriteOnly);

		int status = 0;
		if (aData.qualityMassive_.count(GOOD)/GOOD_LIMIT >= aData.qualityMassive_.size()) {
			status = 1;
		}
		dataStreamStatus << status;

		MessageSP messageStatus(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_QUALITY_STATUS, statusBA));

		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageStatus);
		}
	}

	QByteArray dataResult;
	QDataStream dataResultStream(&dataResult, QIODevice::WriteOnly);


	dataResultStream << sourceType;
	dataResultStream << (int)result;;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT, dataResult));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageResult);
	}

	QString dataToFile = aData.timeHMSMs.at(aLastItem).toString("hh:mm:ss:zzz") + " " + QString::number(aData.coordLatLon.at(aLastItem).x()) + " " + QString::number(aData.coordLatLon.at(aLastItem).y()) + " " + QString::number(aData.heigh.at(aLastItem)) + "\n";

	m_logManager2->writeToFile(dataToFile);
}

void CoordinateCounter::slotCatchDataFromRadioLocationManual(const SolveResult &result, const DataFromRadioLocation &aData)
{
	int sourceType = MANUAL_HEIGH;

	if(aData.timeHMSMs.size()==0) {
		return;
	}

	int aLastItem = aData.timeHMSMs.size() - 1;

	if( result == SOLVED ) {
		QByteArray dataToSend;
		QDataStream ds(&dataToSend, QIODevice::WriteOnly);

		ds << encodeSolverData(aData, true);

		/// @deprecated Now source type determinates by TCP/RPC message type
		ds << sourceType;

		MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA, dataToSend));

		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, message);
		}

		//Send status to ARM-R UI

		QByteArray statusBA;
		QDataStream dataStreamStatus(&statusBA, QIODevice::WriteOnly);

		int status = 0;
		if (aData.qualityMassive_.count( GOOD )/GOOD_LIMIT >= aData.qualityMassive_.size()) {
			status = 1;
		}
		dataStreamStatus << status;

		MessageSP messageStatus(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_QUALITY_STATUS, statusBA));

		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageStatus);
		}
	}

	QByteArray dataResult;
	QDataStream dataResultStream(&dataResult, QIODevice::WriteOnly);


	dataResultStream << sourceType;
	dataResultStream << (int)result;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT, dataResult));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageResult);
	}

	QString dataToFile = aData.timeHMSMs.at(aLastItem).toString("hh:mm:ss:zzz") + " " + QString::number(aData.coordLatLon.at(aLastItem).x()) + " " + QString::number(aData.coordLatLon.at(aLastItem).y()) + " " + QString::number(aData.heigh.at(aLastItem)) + "\n";

	m_logManager1->writeToFile(dataToFile);
}

void CoordinateCounter::slotOneCatchDataFromRadioLocationManual(const SolveResult &result, const OneDataFromRadioLocation &aData_1, const OneDataFromRadioLocation &aData_2)
{
	int sourceType = ONE_DATA;

	if( result == SOLVED ) {
		QByteArray ba;
		QDataStream ds(&ba, QIODevice::ReadWrite);

		QList<UAVPositionDataEnemy> list;
		list << encodeSolverData(aData_1);
		list << encodeSolverData(aData_2);

		ds << list;

		/// @deprecated Now source type determinates by TCP/RPC message type
		ds << (int) sourceType;

		MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_SINGLE, ba));

		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, message);
		}

	}

	QByteArray dataResult;
	QDataStream dataResultStream(&dataResult, QIODevice::WriteOnly);


	dataResultStream << sourceType;
	dataResultStream << (int)result;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT, dataResult));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageResult);
	}


	QString dataToFile = aData_1.timeHMSMs.toString("hh:mm:ss:zzz") + " " + QString::number(aData_1.coordLatLon.x()) + " " + QString::number(aData_1.coordLatLon.y()) + " " + QString::number(aData_1.heigh) + " " +QString::number(aData_2.coordLatLon.x()) + " " + QString::number(aData_2.coordLatLon.y()) + " " + QString::number(aData_2.heigh) + "\n";

	m_logManager3->writeToFile(dataToFile);
}

void CoordinateCounter::slotCatchDataHyperbolesFromRadioLocation(const SolveResult &result, const HyperbolesFromRadioLocation &hyperb)
{
	if( result == SOLVED ) {
		QByteArray dataToSend;

		QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
		dataStream << m_centerFrequency;
		dataStream << hyperb.timeHMSMs;
		dataStream << hyperb.hyperboles_list;

		MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_HYPERBOLA, dataToSend));
		foreach (ITcpListener* receiver, m_receiversList) {
			receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, message);
		}

		dataToSend.clear();
	}

	QByteArray dataResult;
	QDataStream dataResultStream(&dataResult, QIODevice::WriteOnly);

	int sourceType = HYPERBOLES;

	dataResultStream << sourceType;
	dataResultStream << (int)result;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT, dataResult));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageResult);
	}
}

void CoordinateCounter::slotErrorOccured(int error_type, QString str)
{
	log_debug(QString("ERROR = %1").arg(error_type));

	QByteArray strBA;
	QDataStream dataStream(&strBA, QIODevice::WriteOnly);
	dataStream << str;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ERRORS, strBA));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, messageResult);
	}
}


void CoordinateCounter::setSolverDataSize(int aSize)
{
	if ((aSize>10) && (aSize<2000)) {
		m_solver->SetOutDataLength(aSize);
	}
}

void CoordinateCounter::setSolverAnalyzeSize(int aSize)
{
	if ((aSize>10) && (aSize<200)) {
		//		m_solver->SetStateAnalizeCount(aSize);
	}
}

void CoordinateCounter::initSolver()
{
	//Solver
	m_solver = new Solver(this);

	QString stationsSettingsFile = QCoreApplication::applicationDirPath();
	stationsSettingsFile.append("./TCP/coders.ini");
	QSettings stationSettings( stationsSettingsFile, QSettings::IniFormat );
	stationSettings.setIniCodec( QTextCodec::codecForName("UTF-8") );

	QStringList childKeys = stationSettings.childGroups();

	QMap<int, PositionLocal> settings;

	foreach(const QString& childKey, childKeys) {
		stationSettings.beginGroup(childKey);

		int type=stationSettings.value("type",0).toInt();
		if(type!=2)
		{
			stationSettings.endGroup();
			continue;
		}

		PositionLocal coordData;

		//double latitude = stationSettings.value("latitude",0).toDouble();
		//double longitude = stationSettings.value("longitude",0).toDouble();
		//int altitude = stationSettings.value("altitude",0).toInt();

		coordData.lat = stationSettings.value("latitude",0).toDouble();
		coordData.lon = stationSettings.value("longitude",0).toDouble();
		coordData.alt = stationSettings.value("altitude",0).toInt();

		int id = stationSettings.value("id",0).toInt();

		settings.insert(id, coordData);

		//int id2 = m_solver->AddStation(latitude,longitude,altitude);
		stationSettings.endGroup();
	}

	foreach (int id, settings.keys()) {

		double latitude = settings.value(id).lat;
		double longitude = settings.value(id).lon;
		int altitude = settings.value(id).alt;

		m_solver->AddStation(latitude,longitude,altitude);
	}


	m_solver->AddSolverType(ONE_DATA);
	m_solver->AddSolverType(HYPERBOLES);
	m_solver->AddSolverType(AUTO_HEIGH);
	m_solver->AddSolverType(MANUAL_HEIGH);

	m_solver->RegisterListener(this);

	connect(this, SIGNAL(signalGetDataFromRadioLocation(SolveResult,DataFromRadioLocation)), this, SLOT(slotCatchDataFromRadioLocationAuto(SolveResult,DataFromRadioLocation)));
	connect(this, SIGNAL(signalGetDataFromRadioLocationManualHeight(SolveResult,DataFromRadioLocation)), this, SLOT(slotCatchDataFromRadioLocationManual(SolveResult,DataFromRadioLocation)));
	connect(this, SIGNAL(signalGetHyperbolesDataFromRadioLocation(SolveResult,HyperbolesFromRadioLocation)), this, SLOT(slotCatchDataHyperbolesFromRadioLocation(SolveResult,HyperbolesFromRadioLocation)));
	connect(this, SIGNAL(signalGetOneDataFromRadioLocation(SolveResult,OneDataFromRadioLocation,OneDataFromRadioLocation)), this, SLOT(slotOneCatchDataFromRadioLocationManual(SolveResult,OneDataFromRadioLocation,OneDataFromRadioLocation)));
	connect(this, SIGNAL(signalError(int ,QString)), this, SLOT(slotErrorOccured(int ,QString)));

}

void CoordinateCounter::slotSetCenterFrequency(const double& frequency)
{
	m_centerFrequency = frequency;
}

QList<UAVPositionDataEnemy> CoordinateCounter::encodeSolverData(
		const DataFromRadioLocation& data, bool useCommonAlt)
{
	// get the shortest length of vectors
	// to prevent app crash fault of erroneous data content
	int len = data.timeHMSMs.size();
	len = qMin( len, data.airspeed.size() );
	len = qMin( len, data.airSpeedStdDev.size() );
	len = qMin( len, data.coordLatLon.size() );
	len = qMin( len, data.heigh.size() );
	len = qMin( len, data.heighStdDev.size() );
	len = qMin( len, data.latLonStdDev.size() );
	len = qMin( len, data.relativeBearing.size() );
	len = qMin( len, data.StateMassive_.size() );

	QList<UAVPositionDataEnemy> list;

	for ( int i=0; i < len; i++ ) {
		UAVPositionDataEnemy uav;

		uav.altitude = useCommonAlt ? m_alt : data.heigh[i];
		uav.altitudeStdDev = data.heighStdDev[i];
		uav.speed = data.airspeed[i];
		uav.course = data.relativeBearing[i];
		uav.state = 1; // data.StateMassive_[i]
		uav.frequency = m_centerFrequency;
		uav.time = data.timeHMSMs[i];
		uav.latLonStdDev = data.latLonStdDev[i];
		uav.latLon = data.coordLatLon[i];

		list << uav;
	}

	return list;
}

UAVPositionDataEnemy CoordinateCounter::encodeSolverData(const OneDataFromRadioLocation& data)
{
	UAVPositionDataEnemy uav;

	double alt = 100;
	double speed = 0;
	double course = 0;

	uav.altitude = alt;
	uav.altitudeStdDev = data.heighStdDev;
	uav.speed = speed;
	uav.course = course;
	uav.state = 1;
	uav.frequency = m_centerFrequency;
	uav.time = data.timeHMSMs;
	uav.latLonStdDev = data.latLonStdDev;
	uav.latLon = data.coordLatLon;

	return uav;
}

void CoordinateCounter::sendDataToClientTcpServer() {
	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << m_aData;
	dataStream << m_centerFrequency;

	MessageSP message(new Message<QByteArray>(CLIENT_TCP_SERVER_SOLVER_DATA, dataToSend));
	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(CLIENT_TCP_SERVER, m_likeADeviceName, message);
	}
}

bool CoordinateCounter::getIsSolverServer()
{
	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	return settings.value("ClientTCPServer/isSolverServer", false).toBool();
}
