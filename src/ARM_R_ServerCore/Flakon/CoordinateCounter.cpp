#include <Logger.h>

#include "CoordinateCounter.h"

CoordinateCounter::CoordinateCounter(const QString& deviceName, QObject* parent) :
	QObject(parent)
{
	m_solver = NULL;

	m_corr_threshold = 3;
	m_prevStation = 0;

	qRegisterMetaType<DataFromFlacon> ("DataFromFlacon");
	qRegisterMetaType<DataFromRadioLocation> ("DataFromRadioLocation");
	qRegisterMetaType<OneDataFromRadioLocation> ("OneDataFromRadioLocation");
	qRegisterMetaType<ErrorType> ("ErrorType");
	qRegisterMetaType<SolveResult> ("SolveResult");
	qRegisterMetaType<HyperbolesFromRadioLocation> ("HyperbolesFromRadioLocation");

	m_likeADeviceName = deviceName;
	log_debug(QString("Created %1").arg(m_likeADeviceName));
}

CoordinateCounter::~CoordinateCounter()
{
	if(m_solver != NULL)
		delete m_solver;
	emit signalFinished();
}

void CoordinateCounter::onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument)
{
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
		ZDR mZDR;
		mZDR.getDataFromFlackon(m_main_point, vec_p, m_corr_threshold, aDR);
		m_map_vec_corr.clear();

		if (aDR.size() == 5) {
			// form data and calculate coordinates
			m_aData.numOfReferenceDetector_= m_main_point; // reference detector number
			m_aData.time_ = QTime::currentTime();	//Time
			m_aData.ranges_ = aDR;					//Adjusted difference in distance (maxima correlation graphs)

			m_solver->GetData(m_aData);
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
	emit signalError(error_type, str);
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

void CoordinateCounter::slotCatchDataFromRadioLocationAuto(const SolveResult &result, const DataFromRadioLocation &aData)
{
	if(aData.timeHMSMs.size()==0) {
		return;
	}


	int aLastItem = aData.timeHMSMs.size() - 1;

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	int state = 1;

	dataStream << aData.timeHMSMs.at(aLastItem);
	dataStream << state;								/*aData.StateMassive_.at(aLastItem)*/
	dataStream << aData.latLonStdDev.at(aLastItem);
	dataStream << aData.coordLatLon;
	dataStream << aData.airspeed.at(aLastItem);
	dataStream << aData.heigh.at(aLastItem);
	dataStream << aData.relativeBearing.at(aLastItem);

	MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO, dataToSend));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, message);
	}
}

void CoordinateCounter::slotCatchDataFromRadioLocationManual(const SolveResult &result, const DataFromRadioLocation &aData)
{
	if(aData.timeHMSMs.size()==0) {
		return;
	}

	int aLastItem = aData.timeHMSMs.size() - 1;

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	int state = 1;

	dataStream << aData.timeHMSMs.at(aLastItem);
	dataStream << state;									/*aData.StateMassive_.at(aLastItem)*/
	dataStream << aData.latLonStdDev.at(aLastItem);
	dataStream << aData.coordLatLon;
	dataStream << aData.airspeed.at(aLastItem);
	dataStream << m_alt;									//aData.heigh.at(aLastItem);
	dataStream << aData.relativeBearing.at(aLastItem);

	MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA, dataToSend));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(FLAKON_TCP_DEVICE, m_likeADeviceName, message);
	}
}

void CoordinateCounter::slotOneCatchDataFromRadioLocationManual(const SolveResult &result, const OneDataFromRadioLocation &aData_1, const OneDataFromRadioLocation &aData_2)
{
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::ReadWrite);

	double alt = 100;
	double speed = 0;
	double course = 0;
	int state = 1;

	QVector<QPointF>vec;
	vec.append(aData_1.coordLatLon);
	ds<<aData_1.timeHMSMs;
	ds<<state/*aData.StateMassive_.at(aLastItem)*/;
	ds<<aData_1.latLonStdDev;
	ds<<vec;
	ds<<speed;
	ds<<alt;
	ds<<course;

	MessageSP message(new Message<QByteArray>(TCP_FLAKON_COORDINATES_COUNTER_ANSWER_BPLA_AUTO, ba));

	foreach (ITcpListener* receiver, m_receiversList) {
		receiver->onMessageReceived(DeviceTypesEnum::FLAKON_TCP_DEVICE, m_likeADeviceName, message);
	}
}

void CoordinateCounter::slotCatchDataHyperbolesFromRadioLocation(const SolveResult &result, const HyperbolesFromRadioLocation &hyperb)
{
	log_debug("Hyperboles");
}

void CoordinateCounter::slotErrorOccured(const ErrorType &error_type, const QString &str)
{
	log_debug(QString("ERROR = ").arg(error_type));
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
	connect(this, SIGNAL(signalError(ErrorType,QString)), this, SLOT(slotErrorOccured(ErrorType,QString)));

}
