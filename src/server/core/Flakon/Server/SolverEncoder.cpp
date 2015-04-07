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

//	foreach (ISolverListener* receiver, m_receiversList) {
//		DataFromRadioLocation data;
//		receiver->onSendDataFromRadioLocation(SOLVED, data);
//	}
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

///   Auto Trajectory   ///
			if( packet.command().arguments().solveranswer().has_auto_trajectory() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::Trajectory traj =
						packet.command().arguments().solveranswer().auto_trajectory();

				SolveResult res = (SolveResult)traj.result_of_calculation();
				DataFromRadioLocation data;

				for( int i = 0; i < traj.motionestimate_size(); i++ ) {
					SolverClient::Packet::ArgumentVariant::SolverAnswer::MotionEstimate motionEst = traj.motionestimate(i);
					data.coordLatLon.append( QPointF(motionEst.coordinates().lon(), motionEst.coordinates().lat()) );
					data.heigh.append( motionEst.coordinates().alt() );
					data.latLonStdDev.append( QPointF(motionEst.coordinates_acc().lon_acc(), motionEst.coordinates_acc().lat_acc()) );
					data.heighStdDev.append( motionEst.coordinates_acc().alt_acc() );
					data.timeHMSMs.append( QTime::currentTime() );
					data.airspeed.append( motionEst.targetspeed() );
					data.airSpeedStdDev.append( motionEst.targetspeed_acc() );
					data.relativeBearing.append( motionEst.relativebearing() );
					data.StateMassive_.append( (State)motionEst.state() );
					data.qualityMassive_.append( (Quality)motionEst.quality() );
				}

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSendDataFromRadioLocation(res, data);
				}
			}

///   Manual Trajectory   ///
			if( packet.command().arguments().solveranswer().has_manual_trajectory() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::Trajectory traj =
						packet.command().arguments().solveranswer().manual_trajectory();

				SolveResult res = (SolveResult)traj.result_of_calculation();
				DataFromRadioLocation data;

				for( int i = 0; i < traj.motionestimate_size(); i++ ) {
					SolverClient::Packet::ArgumentVariant::SolverAnswer::MotionEstimate motionEst = traj.motionestimate(i);
					data.coordLatLon.append( QPointF(motionEst.coordinates().lon(), motionEst.coordinates().lat()) );
					data.heigh.append( motionEst.coordinates().alt() );
					data.latLonStdDev.append( QPointF(motionEst.coordinates_acc().lon_acc(), motionEst.coordinates_acc().lat_acc()) );
					data.heighStdDev.append( motionEst.coordinates_acc().alt_acc() );
					data.timeHMSMs.append( QTime::currentTime() );
					data.airspeed.append( motionEst.targetspeed() );
					data.airSpeedStdDev.append( motionEst.targetspeed_acc() );
					data.relativeBearing.append( motionEst.relativebearing() );
					data.StateMassive_.append( (State)motionEst.state() );
					data.qualityMassive_.append( (Quality)motionEst.quality() );
				}

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSendDataFromRadioLocationManualHeigh(res, data);
				}
			}

///   Single Marks   ///
			if( packet.command().arguments().solveranswer().has_singlemarks() ) {
				SolverClient::Packet::ArgumentVariant::SolverAnswer::SingleMarks singleMarks = packet.command().arguments().solveranswer().singlemarks();

				SolveResult res = (SolveResult)singleMarks.result_of_calculation();
				QList<OneDataFromRadioLocation> oneDataList;
				int coordsEstimateCount = singleMarks.coordsestimate_size();

				for(int i = 0; i < coordsEstimateCount; i++) {
					OneDataFromRadioLocation oneData;
					SolverClient::Packet::ArgumentVariant::SolverAnswer::CoordsEstimate coordsEstimate = singleMarks.coordsestimate(i);
					oneData.coordLatLon = QPointF( coordsEstimate.coordinates().lon(), coordsEstimate.coordinates().lat() );
					oneData.heigh = coordsEstimate.coordinates().alt();
					oneData.latLonStdDev = QPointF( coordsEstimate.coordinates_acc().lon_acc(), coordsEstimate.coordinates_acc().lat_acc() );
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
				SolveResult res = (SolveResult)stateLines.result_of_calculation();
				HyperbolesFromRadioLocation hyperb;
				hyperb.timeHMSMs = QDateTime::fromMSecsSinceEpoch( stateLines.datetime() ).time();

				for(int i = 0; i < stateLines.stateline_size(); i++) {
					QVector<QPointF> pointsVector;
					SolverClient::Packet::ArgumentVariant::SolverAnswer::StateLines::StateLine stateLine =
							packet.command().arguments().solveranswer().statelines().stateline(i);

					for(int j = 0; j < stateLine.point_size(); j++) {
						pointsVector.append(QPointF( stateLine.point(j).lon(), stateLine.point(j).lat() ));
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
				ErrorType errType = (ErrorType)error.errortype();
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

	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_ZAVIRUHA_PREAMBULA);
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
