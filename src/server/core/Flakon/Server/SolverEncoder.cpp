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
			//qDebug() << QString("TargetID = %1").arg(targetID);

			//Parse Trajectory Message
			SolverClient::Packet::ArgumentVariant::SolverAnswer::Trajectory traj = packet.command().arguments().solveranswer().trajectory();
			//qDebug() << QString("Trajectory Type = %1").arg( QString(traj.type().data()) );
			//Unpack MotionEstimate list in trajectory
			for(int i = 0; i < traj.motionestimate_size(); i++) {
//				qDebug() << QString("> MotionINDEX = %1").arg(i);

//				qDebug() << QString("	CoordEstDate = %1").arg( QDateTime::fromTime_t(traj.motionestimate(i).datetime()).toString() );
//				qDebug() << QString("	Coordinates lat = %1 lon = %2")
//							.arg(traj.motionestimate(i).coordinates().lat())
//							.arg(traj.motionestimate(i).coordinates().lon());

//				qDebug() << QString("	Coordinates_acc lat = %1 lon = %2 alt = %3")
//							.arg(traj.motionestimate(i).coordinates_acc().lat_acc())
//							.arg(traj.motionestimate(i).coordinates_acc().lon_acc())
//							.arg(traj.motionestimate(i).coordinates_acc().alt_acc());

//				qDebug() << QString("	target speed = %1").arg(traj.motionestimate(i).targetspeed());
//				qDebug() << QString("	target speed acc = %1").arg(traj.motionestimate(i).targetspeed_acc());
//				qDebug() << QString("	bearing = %1").arg(traj.motionestimate(i).relativebearing());
			}

			//Parse SingleMarks Message
			SolverClient::Packet::ArgumentVariant::SolverAnswer::SingleMarks singleMarks = packet.command().arguments().solveranswer().singlemarks();
			//Unpack MotionEstimate list in trajectory
			for(int i = 0; i < singleMarks.coordsestimate_size(); i++) {
				//qDebug() << QString("> CoordINDEX = %1").arg(i);

				//qDebug() << QString("	CoordEstDate = %1").arg( QDateTime::fromTime_t(singleMarks.coordsestimate(i).datetime()).toString() );
/*				qDebug() << QString("	Coordinates lat = %1 lon = %2")
							.arg(singleMarks.coordsestimate(i).coordinates().lat())
							.arg(singleMarks.coordsestimate(i).coordinates().lon());

				qDebug() << QString("	Coordinates_acc lat = %1 lon = %2 alt = %3")
							.arg(singleMarks.coordsestimate(i).coordinates_acc().lat_acc())
							.arg(singleMarks.coordsestimate(i).coordinates_acc().lon_acc())
							.arg(singleMarks.coordsestimate(i).coordinates_acc().alt_acc())*/;
			}


			//qDebug() << " >>>>>>>>>>> \r\n\r\n";
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
