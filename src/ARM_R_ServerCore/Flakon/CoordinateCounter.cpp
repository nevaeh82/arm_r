#include "CoordinateCounter.h"

CoordinateCounter::CoordinateCounter(const QString& deviceName, QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(CoordinateCounter));
	m_corr_threshold = 3;
	m_prevStation = 0;

//	initSolver();
	m_likeADeviceName = deviceName;
	m_logger->debug(QString("Created %1").arg(m_likeADeviceName));
}

CoordinateCounter::~CoordinateCounter()
{
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
			//Заполнение данных и отправка на вычисление координат
			m_aData.numOfReferenceDetector_= m_main_point; //Номер опорного
			m_aData.time_ = QTime::currentTime();	//Время
			m_aData.ranges_ = aDR;					//Откорректированные разности расстояний (максимумы графиков корреляции)

			m_solver->GetData(m_aData);
		}
	}

	m_prevStation = point2;
}

void CoordinateCounter::sendData(const MessageSP message)
{
	QString messageType = message->type();

	if (messageType == TCP_FLAKON_COORDINATES_COUNTER_REQUEST_SET_SOLVER) {
		QByteArray messageData = message->data();
		QDataStream dataStream(&messageData, QIODevice::ReadOnly);

		int id;
		double alt;
		int track_length = 0;

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

void CoordinateCounter::slotCatchDataFromRadioLocationAuto(const DataFromRadioLocation &aData)
{

	int aLastItem = aData.timeHMSMs.size() - 1;

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	dataStream << aData.timeHMSMs.at(aLastItem);
	dataStream << 1;								/*aData.StateMassive_.at(aLastItem)*/
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

void CoordinateCounter::slotCatchDataFromRadioLocationManual(const DataFromRadioLocation &aData)
{
	int aLastItem = aData.timeHMSMs.size() - 1;

	QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);

	dataStream << aData.timeHMSMs.at(aLastItem);
	dataStream << 1;									/*aData.StateMassive_.at(aLastItem)*/
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

void CoordinateCounter::setSolverDataSize(int aSize)
{
	if ((aSize>10) && (aSize<2000)) {
		m_solver->SetOutDataLength(aSize);
	}
}

void CoordinateCounter::setSolverAnalyzeSize(int aSize)
{
	if ((aSize>10) && (aSize<200)) {
		m_solver->SetStateAnalizeCount(aSize);
	}
}

void CoordinateCounter::initSolver()
{
	//Solver
	m_solver = new Solver;

	//� азмер приходящих данных
	setSolverDataSize(100);

	//Кол-во данных для определения движения
	setSolverAnalyzeSize(60);

	connect(m_solver,SIGNAL(signal_sendDataFromRadioLocation(const DataFromRadioLocation&)), this, SLOT(slotCatchDataFromRadioLocationAuto(const DataFromRadioLocation&)));
	connect(m_solver,SIGNAL(signal_sendDataFromRadioLocationManualHeigh(const DataFromRadioLocation&)), this, SLOT(slotCatchDataFromRadioLocationManual(const DataFromRadioLocation&)));
}
