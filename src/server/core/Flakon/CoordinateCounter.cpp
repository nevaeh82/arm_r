#include <Logger/Logger.h>

#include "RDSExchange.h"

#include "CoordinateCounter.h"
#include "Info/StationConfiguration.h"

#define GOOD_LIMIT 0.7
#define VERACITY 1

#define DELAY 500

CoordinateCounter::CoordinateCounter(const QString& deviceName, QObject* parent) :
	QObject(parent),
	isInit(false),
    m_main_point(0),
    m_stationsShift(0)
{
	QDir dir;
	dir.mkdir("./logs/SpecialLogs");
    m_resTime.start();

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

    connect(this, SIGNAL(signalSetShift(double)), this, SLOT(slotSetShift(double)));
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

	if( argument->type() == TCP_RDS_ANSWER_LOCSYSTEM ) {
		QByteArray protoData = argument->data();

		RdsProtobuf::Packet msg;
		msg.ParseFromArray(protoData.data(), protoData.size());

		if( !msg.has_from_server()) {
			return;
		}

		RdsProtobuf::ServerMessage sMsg = msg.from_server();

		if(!isServerLocationShot(sMsg)) {
			return;
		}

		RdsProtobuf::ServerMessage_OneShotData_LocationData location = getServerLocationShot(sMsg);


		SolverProtocol::Packet solverPkt;

		SolverProtocol::MeasurementsData* solverData =
				solverPkt.mutable_datafromclient()->mutable_measurementsdata(); ;

		foreach (RdsProtobuf::Convolution cnvMsg, location.convolution()) {
			int firstInd = cnvMsg.first_detector_index() + m_stationsShift;
			int secInd = cnvMsg.second_detector_index() + m_stationsShift;

			SolverProtocol::MeasurementsData_DataPacket* data = solverData->add_data_packet();
			data->set_first_detector_index( firstInd );
			data->set_second_detector_index( secInd );
			data->set_time_delay( cnvMsg.delay() );
			data->set_time_delay_sdv( cnvMsg.delay_accuracy() );

			if( cnvMsg.has_doppler() && cnvMsg.has_doppler_accuracy() ) {
				data->set_dopler( cnvMsg.doppler() );
				data->set_dopler_sdv( cnvMsg.doppler_accuracy() );
			}
		}

		solverData->set_central_frequency( (location.range().start() + location.range().end())/2 );
		solverData->set_datetime( location.date_time() );

		sendDataToClientTcpServer1(solverPkt);
	}
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

void CoordinateCounter::onSolver1ProtoData(const int &result, const QByteArray &data)
{
	emit signal1ProtoData( result, data );
}

void CoordinateCounter::onSolverBlaData(const QByteArray &data)
{

    if( m_resTime.elapsed() > DELAY ) {
        SolverProtocol::Packet pkt;
        pkt.ParseFromArray(data.data(), data.size());

        emit signal1BlaData( data );
        m_resTime.restart();
    }
}

void CoordinateCounter::onSolverWorkData(const QByteArray &data)
{
     emit signal1BlaData( data );
}

void CoordinateCounter::onSolver1SetupAnswer(const QByteArray &data)
{
	emit signal1SetupAnswer( data );
}

//Here Now parse solver protobuf
void CoordinateCounter::sendData(const MessageSP message)
{
	QString messageType = message->type();

	if (messageType == TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER) {
		QByteArray messageData = message->data();
		sendRawDataToClientTcpServer1(messageData);
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

void CoordinateCounter::setShift(const double shift)
{
    emit signalSetShift(shift);
}

void CoordinateCounter::setStationsShift(const uint val)
{
    m_stationsShift = val;
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
	dataResultStream << (int)result;

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

void CoordinateCounter::slotSolverBlaData( QByteArray data ) {

         //log_debug("ON SEND BALDATA >>>>>>>>>!");
         MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_1, data));
         foreach (ITcpListener* receiver, m_receiversList) {
             receiver->onMessageReceived(RDS_TCP_DEVICE, m_likeADeviceName, message);
         }

         //To NIIPP
         MessageSP messageNiipp(new Message<QByteArray>(CLIENT_TCP_SERVER_SOLVER_DATA, data));
         foreach (ITcpListener* receiver, m_receiversList) {
             receiver->onMessageReceived(CLIENT_TCP_SERVER, m_likeADeviceName, messageNiipp);
         }
}

void CoordinateCounter::slotSolver1ProtoData(int result, QByteArray data)
{
	QByteArray dataResult;
	QDataStream dataResultStream(&dataResult, QIODevice::WriteOnly);

	dataResultStream << result;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_RESULT_1, dataResult));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(RDS_TCP_DEVICE, m_likeADeviceName, messageResult);
	}
}

void CoordinateCounter::slotSolver1SetupAnswer(QByteArray data)
{
	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_SOLVER_SETUP_1, data));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(RDS_TCP_DEVICE, m_likeADeviceName, messageResult);
	}
}

void CoordinateCounter::slotErrorOccured(int error_type, QString str)
{
    //log_debug(QString("ERROR = %1").arg(error_type));

	QByteArray strBA;
	QDataStream dataStream(&strBA, QIODevice::WriteOnly);
	dataStream << str;

	MessageSP messageResult(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ERRORS, strBA));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(RDS_TCP_DEVICE, m_likeADeviceName, messageResult);
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

	connect(this, SIGNAL(signal1ProtoData(int,QByteArray)), this, SLOT(slotSolver1ProtoData(int,QByteArray)));

    connect(this, SIGNAL(signal1BlaData(QByteArray)),
            this, SLOT(slotSolverBlaData(QByteArray)));

	connect(this, SIGNAL(signal1SetupAnswer(QByteArray)), this, SLOT(slotSolver1SetupAnswer(QByteArray)));

}

void CoordinateCounter::slotSetCenterFrequency(const double& frequency)
{
	m_centerFrequency = frequency;
}

void CoordinateCounter::slotSetShift(const double val)
{
   m_shift = val;
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

void CoordinateCounter::sendDataToClientTcpServer1(const SolverProtocol::Packet& data)
{
	QByteArray outData;
	outData.resize( data.ByteSize() );
	data.SerializeToArray( outData.data(), outData.size() );

	MessageSP message(new Message<QByteArray>(CLIENT_TCP_SERVER_SOLVER_DATA_1, outData));
	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(CLIENT_TCP_SERVER, m_likeADeviceName, message);
	}
}

void CoordinateCounter::sendRawDataToClientTcpServer1(const QByteArray& data)
{
	MessageSP message(new Message<QByteArray>(CLIENT_TCP_SERVER_SOLVER_DATA_1, data));
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

uint CoordinateCounter::getSolverVersion()
{
	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	return settings.value("ClientTCPServer/SolverVersion", 0).toUInt();
}
