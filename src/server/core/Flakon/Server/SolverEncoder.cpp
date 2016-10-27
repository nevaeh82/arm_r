#include "Logger/Logger.h"
#include "SolverEncoder.h"

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

    Q_UNUSED(inData);
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
            niippLst.append(QString::fromStdString(tPkt.targetid()));

            quint64 t = mPkt.datetime();
            QDateTime dt = QDateTime::fromMSecsSinceEpoch(t);

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
            message.append(QString::number(n));

            message.append("\\r\\n");
            all.append(message);
        }

        return all.toLocal8Bit();

	} else if( message->type() == CLIENT_TCP_SERVER_BPLA_DATA ) {
        return QByteArray();
	} else {
		return QByteArray();
	}


	return dataToSend;
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
