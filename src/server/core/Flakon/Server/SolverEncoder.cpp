#include "Logger/Logger.h"
#include "SolverEncoder.h"

SolverEncoder::SolverEncoder(QObject* parent)
{
}

SolverEncoder::~SolverEncoder()
{
}

void SolverEncoder::onDataReceived(const QVariant& argument)
{
	QByteArray inData = encode( argument.toByteArray() );
	if( inData == QByteArray() ) {
		return;
	} else {
		readProtobuf( inData );
	}
}

void SolverEncoder::readProtobuf(const QByteArray& inputData)
{
	SolverClient::Packet packet;
	packet.ParseFromArray( inputData.constData(), inputData.size() );

	if( packet.command().action() == SolverClient::sendSolverClientResult ) {
		//qDebug() << " >>>>>>>>>>> SolverAnswerPacket in server ...";
		if( !packet.command().arguments().has_solveranswer() ) {
			//qDebug() << "Bad Message";
		} else {
			int targetID = packet.command().arguments().solveranswer().targetid();
			Q_UNUSED(targetID); // For future using

			SolveResult res = SOLVED;

///   Auto Trajectory   ///
			if( packet.command().arguments().solveranswer().has_auto_trajectory() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::Trajectory traj =
						packet.command().arguments().solveranswer().auto_trajectory();

				res = (SolveResult)(traj.result_of_calculation() - 1);
				DataFromRadioLocation data;

				for( int i = 0; i < traj.motionestimate_size(); i++ ) {
					SolverClient::Packet::ArgumentVariant::SolverAnswer::MotionEstimate motionEst = traj.motionestimate(i);
					data.coordLatLon.append( QPointF(motionEst.coordinates().lat(), motionEst.coordinates().lon()) );
					data.heigh.append( motionEst.coordinates().alt() );
					data.latLonStdDev.append( QPointF(motionEst.coordinates_acc().lat_acc(), motionEst.coordinates_acc().lon_acc()) );
					data.heighStdDev.append( motionEst.coordinates_acc().alt_acc() );
					data.timeHMSMs.append( QTime::currentTime() );
					data.airspeed.append( motionEst.targetspeed() );
					data.airSpeedStdDev.append( motionEst.targetspeed_acc() );
					data.relativeBearing.append( motionEst.relativebearing() );
					data.StateMassive_.append( (State)(motionEst.state() - 1) );
					data.qualityMassive_.append( (Quality)(motionEst.quality() - 1) );
				}

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSendDataFromRadioLocation(res, data);
				}
			}

///   Manual Trajectory   ///
			if( packet.command().arguments().solveranswer().has_manual_trajectory() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::Trajectory traj =
						packet.command().arguments().solveranswer().manual_trajectory();

				res = (SolveResult)(traj.result_of_calculation() - 1);

				DataFromRadioLocation data;

				for( int i = 0; i < traj.motionestimate_size(); i++ ) {
					SolverClient::Packet::ArgumentVariant::SolverAnswer::MotionEstimate motionEst = traj.motionestimate(i);
					data.coordLatLon.append( QPointF(motionEst.coordinates().lat(), motionEst.coordinates().lon()) );
					data.heigh.append( motionEst.coordinates().alt() );
					data.latLonStdDev.append( QPointF(motionEst.coordinates_acc().lat_acc(), motionEst.coordinates_acc().lon_acc()) );
					data.heighStdDev.append( motionEst.coordinates_acc().alt_acc() );
					data.timeHMSMs.append( QTime::currentTime() );
					data.airspeed.append( motionEst.targetspeed() );
					data.airSpeedStdDev.append( motionEst.targetspeed_acc() );
					data.relativeBearing.append( motionEst.relativebearing() );
					data.StateMassive_.append( (State)(motionEst.state() - 1) );
					data.qualityMassive_.append( (Quality)(motionEst.quality() - 1) );
				}

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSendDataFromRadioLocationManualHeigh(res, data);
				}
			}

///   Single Marks   ///
			if( packet.command().arguments().solveranswer().has_singlemarks() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::SingleMarks singleMarks = packet.command().arguments().solveranswer().singlemarks();

				res = (SolveResult)(singleMarks.result_of_calculation() - 1);

				QList<OneDataFromRadioLocation> oneDataList;
				int coordsEstimateCount = singleMarks.coordsestimate_size();

				for(int i = 0; i < coordsEstimateCount; i++) {
					OneDataFromRadioLocation oneData;
					SolverClient::Packet::ArgumentVariant::SolverAnswer::CoordsEstimate coordsEstimate = singleMarks.coordsestimate(i);
					oneData.coordLatLon = QPointF( coordsEstimate.coordinates().lat(), coordsEstimate.coordinates().lon() );
					oneData.heigh = coordsEstimate.coordinates().alt();
					oneData.latLonStdDev = QPointF( coordsEstimate.coordinates_acc().lat_acc(), coordsEstimate.coordinates_acc().lon_acc() );
					oneData.heighStdDev = coordsEstimate.coordinates_acc().alt_acc();
					oneData.timeHMSMs = QTime::currentTime();
					oneDataList.append(oneData);
				}

				if( oneDataList.length() < 2 ) {
					oneDataList.append(oneDataList.at(0));
				}

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSendOneDataFromRadioLocation( res, oneDataList.at(0), oneDataList.at(1) );
				}
			}

///   State Lines   ///
			if( packet.command().arguments().solveranswer().has_statelines() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::StateLines stateLines = packet.command().arguments().solveranswer().statelines();
				res = (SolveResult)(stateLines.result_of_calculation() - 1);

				HyperbolesFromRadioLocation hyperb;
				hyperb.timeHMSMs = QDateTime::fromMSecsSinceEpoch( stateLines.datetime() ).time();

				for(int i = 0; i < stateLines.stateline_size(); i++) {
					QVector<QPointF> pointsVector;
					SolverClient::Packet::ArgumentVariant::SolverAnswer::StateLines::StateLine stateLine =
							packet.command().arguments().solveranswer().statelines().stateline(i);

					for(int j = 0; j < stateLine.point_size(); j++) {
						pointsVector.append(QPointF( stateLine.point(j).lat(), stateLine.point(j).lon() ));
					}

					hyperb.hyperboles_list.append(pointsVector);
				}

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSendHyperbolesFromRadioLocation( res, hyperb );
				}
			}

///   Error Message   ///
			if( packet.command().arguments().solveranswer().has_errormessage() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::ErrorMessage error = packet.command().arguments().solveranswer().errormessage();
				ErrorType errType = (ErrorType)(error.errortype() - 1);
				QString errString = QString::fromStdString( error.message() );

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onErrorOccured(errType, errString);
				}
			}
		}
	}
}

QByteArray SolverEncoder::encode(const QByteArray &data)
{
	m_dataFromTcpSocket.append(data);

	if (m_dataFromTcpSocket.size() < TCP_ZAVIRUHA_PREAMBULA_LEN){
		return QByteArray();
	}

	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_ZAVIRUHA_PREAMBULA_SOLVER);
	m_dataFromTcpSocket = m_dataFromTcpSocket.right( m_dataFromTcpSocket.length() - preambulaIndex );
	if (preambulaIndex < 0) {
		return QByteArray();
	}

	memcpy( (char*)&m_residueLength, m_dataFromTcpSocket.data() + 3, sizeof(uint) );

	QByteArray dataToSend;
	int packetLen = m_residueLength + TCP_ZAVIRUHA_PREAMBULA_LEN;

	if (m_dataFromTcpSocket.length() >= packetLen) {
		QByteArray data = m_dataFromTcpSocket.mid(preambulaIndex + TCP_ZAVIRUHA_PREAMBULA_LEN, m_residueLength);
		dataToSend = data;

		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - packetLen);
		m_residueLength = 0;
	}

	return dataToSend;
}

QByteArray SolverEncoder::decode(const MessageSP message) {
	QByteArray dataToSend;

	//Zaviruha::Packet packet;
	SolverClient::Packet packet;

	SolverClient::Packet::Command* packetCommand = new SolverClient::Packet::Command();
	packet.set_allocated_command(packetCommand);

	if( message->type() == CLIENT_TCP_SERVER_SOLVER_DATA ) {
		packetCommand->set_action(SolverClient::sendSolverClientData);

		SolverClient::Packet::ArgumentVariant* packetArgs = new SolverClient::Packet::ArgumentVariant();
		SolverClient::Packet::ArgumentVariant::SolverInput* arg = new SolverClient::Packet::ArgumentVariant::SolverInput();
		packetArgs->set_allocated_solverinput(arg);
		packetCommand->set_allocated_arguments(packetArgs);
		toProtobufSolverData( arg, message->data() );
	} else if( message->type() == CLIENT_TCP_SERVER_BPLA_DATA ) {
		packetCommand->set_action(SolverClient::sendSolverClientBla);
	} else {
		return QByteArray();
	}

	unsigned int size = packet.ByteSize();
	dataToSend.resize(size);
	packet.SerializeToArray(dataToSend.data(), size);

	addPreambula(dataToSend);
	return dataToSend;
}

void SolverEncoder::toProtobufSolverData(SolverClient::Packet::ArgumentVariant::SolverInput* arg, QByteArray &data) {
	QDataStream stream(&data, QIODevice::ReadOnly);

	DataFromFlaconInt receiveData;
	double centerFrequency;
	stream >> receiveData.numOfReferenceDetector_;
	stream >> receiveData.time_;
	stream >> receiveData.ranges_;
	stream >> centerFrequency;

	foreach (double range, receiveData.ranges_) {
		arg->add_delays(range);
	}

//	QDateTime dateTime = QDateTime::currentDateTime();
//	//Fill time form solver
//	//TODO new solver has QDateTime
//	dateTime.setTime(receiveData.time_);
	arg->set_datetime( receiveData.time_ );
	arg->set_centerfrequency(centerFrequency);
}

void SolverEncoder::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend(TCP_ZAVIRUHA_PREAMBULA_SOLVER);
}
