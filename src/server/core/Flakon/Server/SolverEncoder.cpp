#include "Logger/Logger.h"
#include "SolverEncoder.h"

#include "zavCommon/UAVDefines.h"

#include "ProtoTypes1.pb.h"

SolverEncoder::SolverEncoder(QObject* parent)
{
}

SolverEncoder::~SolverEncoder()
{
}

void SolverEncoder::onDataReceived(const QVariant& argument)
{
	QByteArray inData = argument.toByteArray();
	m_dataFromTcpSocket.append(inData);

	if( m_dataFromTcpSocket.indexOf("SPP") < 0 ) {
		return;
	} else if( m_dataFromTcpSocket.indexOf("\n", m_dataFromTcpSocket.indexOf("SPP")) < 0 ) {
		return;
	}

	int from = m_dataFromTcpSocket.lastIndexOf("SPP");
	int lenRight = m_dataFromTcpSocket.size() - from;
	if(from < 0) {
		return;
	}

	QByteArray test = m_dataFromTcpSocket.mid(from, m_dataFromTcpSocket.size() - from );

	QList<QByteArray> dataList = test.split(',');

	if(dataList.size() < 5 ) {
		m_dataFromTcpSocket.clear();
		return;
	} else if( dataList.at(4).size() < 4 ) {
		return;
	}

	QTextCodec *codec = QTextCodec::codecForName("KOI8-R" );
	QTextCodec *codec1 = QTextCodec::codecForName("KOI8-R");
	QString devName;
	QByteArray devNameRaw = dataList.at(1);
	QByteArray res;
	devName = codec1->toUnicode( devNameRaw );
	res = codec->fromUnicode( devName );

	//devName = QString(devNameRaw);
	int inTst = QString(dataList.at(4).left(4)).toInt();

	m_dataFromTcpSocket.clear();

	emit nippDataIncome(QString(devName), bool(inTst));
}

QByteArray SolverEncoder::encode(const QByteArray &data)
{
	//	m_dataFromTcpSocket.append(data);

	//    if (m_dataFromTcpSocket.size() < TCP_ZAVIRUHA_PREAMBULA_LEN) {
	//		return QByteArray();
	//	}

	//	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_ZAVIRUHA_PREAMBULA_SOLVER);
	//	m_dataFromTcpSocket = m_dataFromTcpSocket.right( m_dataFromTcpSocket.length() - preambulaIndex );
	//	if (preambulaIndex < 0) {
	//		return QByteArray();
	//	}

	//	memcpy( (char*)&m_residueLength, m_dataFromTcpSocket.data() + 3, sizeof(uint) );

	//	QByteArray dataToSend;
	//	int packetLen = m_residueLength + TCP_ZAVIRUHA_PREAMBULA_LEN;

	//	if (m_dataFromTcpSocket.length() >= packetLen) {
	//		QByteArray data = m_dataFromTcpSocket.mid(preambulaIndex + TCP_ZAVIRUHA_PREAMBULA_LEN, m_residueLength);
	//		dataToSend = data;

	//		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - packetLen);
	//		m_residueLength = 0;
	//	}

	return QByteArray();
}

QByteArray SolverEncoder::decode(const MessageSP message) {
	QByteArray dataToSend;

	if( message->type() == CLIENT_TCP_SERVER_SOLVER_DATA ) {

		QByteArray data = message->data();

		SolverProtocol::Packet solutionPacket;

		if( !solutionPacket.ParseFromArray(data.data(), data.size()) ) {
			return QByteArray();
		}

		SolverProtocol::Packet_DataFromSolver_SolverSolution dPkt;
		if(solutionPacket.datafromsolver().has_solution_automatic_altitude()) {
			dPkt = solutionPacket.datafromsolver().solution_automatic_altitude();
		} else if(solutionPacket.datafromsolver().has_solution_manual_altitude()) {
			dPkt = solutionPacket.datafromsolver().solution_manual_altitude();
		} else {
			return QByteArray();
		}


		QString all;
		for(int i = 0; i<dPkt.trajectory_size(); i++) {

			QString message;

			SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory tPkt =  dPkt.trajectory( i );

			SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate mPkt =
					tPkt.motionestimate(tPkt.motionestimate_size()-1);

			QStringList niippLst;
			niippLst.append("BPLA");

			QString t_id = QString::number(i+1, 16);
			int t_id_size = t_id.size();

			if(t_id_size < 4) {
				for(int i = 0; i<4-t_id_size; i++) {
					t_id.prepend("0");
				}
			}

			niippLst.append(t_id);

			niippLst.append(QString::number( tPkt.central_frequency() ));

			quint64 t = mPkt.datetime();
			//QDateTime dt = QDateTime::fromMSecsSinceEpoch(t);
			QDateTime dt = QDateTime::currentDateTime();

			niippLst.append( dt.date().toString("ddMMyy") );
			niippLst.append( dt.time().toString("hhmmss") );

			niippLst.append( QString::number(mPkt.coordinates().alt()) );
			niippLst.append( QString::number(mPkt.targetspeed()) );
			niippLst.append( QString::number(mPkt.relativebearing()) );
			niippLst.append( QString("+") + QString::number(mPkt.coordinates().lat()) );
			niippLst.append( QString("+") + QString::number(mPkt.coordinates().lon()) );

			double acc = sqrt( mPkt.coordinates_acc().lon_acc()*mPkt.coordinates_acc().lon_acc() +
							   mPkt.coordinates_acc().lat_acc()*mPkt.coordinates_acc().lat_acc() );

			niippLst.append( QString::number(acc) );


			message = niippLst.join(",");

			quint8 n =  crc( message.toLocal8Bit() );

			message.append( "*" );
			QString t_src = QString::number(n, 16);
			if(t_src.length() < 2) {
				t_src.prepend("0");
			}

			message.append(t_src.toUpper());

			message.append(0x0A);
			message.append(0x0D);
			all.append(message);
		}

		return all.toLocal8Bit();

	} else if( message->type() == CLIENT_TCP_SERVER_BPLA_DATA ) {
		return QByteArray();
	} else if(message->type() == CLIENT_TCP_SERVER_KTR_DATA) {
		QByteArray msg = message->data();

		QVector<UAVPositionData> blaDataVector;

		QDataStream ds(&msg, QIODevice::ReadOnly);

		ds >> blaDataVector;

		QString all;
		for(int i = 0; i<blaDataVector.size(); i++) {

			QString message;
			UAVPositionData uavData = blaDataVector.at(i);

			if(uavData.sourceType != UAV_AUTOPILOT_SOURCE) {
				continue;
			}

			QStringList niippLst;
			niippLst.append("BLA");

			QString t_id = QString::number(uavData.boardID, 16);
			int t_id_size = t_id.size();

			if(t_id_size < 4) {
				for(int i = 0; i<4-t_id_size; i++) {
					t_id.prepend("0");
				}
			}

			if(t_id.length() < 4) {
				for(int i = 0; i<4-t_id.length(); i++) {
					t_id.prepend("0");
				}
			}

			niippLst.append(t_id);

			QDateTime dt = uavData.dateTime;

			niippLst.append( dt.date().toString("ddMMyy") );
			niippLst.append( dt.time().toString("hhmmss") );

			niippLst.append( QString::number(uavData.altitude) );
			niippLst.append( QString::number(uavData.speed) );
			niippLst.append( QString::number(uavData.course) );
			niippLst.append( QString("+") + QString::number(uavData.latitude) );
			niippLst.append( QString("+") + QString::number(uavData.longitude) );


			message = niippLst.join(",");

			quint8 n =  crc( message.toLocal8Bit() );

			message.append( "*" );

			QString t_src = QString::number(n, 16);
			if(t_src.length() < 2) {
				t_src.prepend("0");
			}

			message.append(t_src.toUpper());

			message.append(0x0A);
			message.append(0x0D);
			all.append(message);
		}

		return all.toLocal8Bit();

	} else {
		return QByteArray();
	}


	return dataToSend;
}

QByteArray SolverEncoder::decodeRegister()
{
	QString message;

	message.append("OD");
	message.append("REQ");

	quint8 n =  crc( message.toLocal8Bit() );

	message.append( "*" );
	message.append(QString::number(n));

	message.append("\\r\\n");

	return message.toLocal8Bit();
}

void SolverEncoder::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend(TCP_ZAVIRUHA_PREAMBULA_SOLVER);
}

quint8 SolverEncoder::crc(const QByteArray& data)
{
	quint8 crc = 0;
	for (int i = 0; i < data.length(); i++) {
		crc ^= data.at(i);
	}
	return crc;
}
