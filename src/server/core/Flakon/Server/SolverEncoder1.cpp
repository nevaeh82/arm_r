#include "Logger/Logger.h"
#include "SolverEncoder1.h"

#include "SolverExchange.h"

SolverEncoder1::SolverEncoder1(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
}

SolverEncoder1::~SolverEncoder1()
{
}

void SolverEncoder1::onDataReceived(const QVariant& argument)
{
//	MessageSP msg = encode( argument.toByteArray() );

//	if( msg->type() != TCP_SOLVER_MESSAGE ) {
//		return;
//	}

//	QByteArray inData = msg->data();

//	if( inData == QByteArray() ) {
//		return;
//	} else {
//		readProtobuf( inData );
//	}
}

void SolverEncoder1::readProtobuf(const QByteArray& inputData)
{
    SolverProtocol::Packet packet;
	packet.ParseFromArray( inputData, inputData.size() );

	if( !packet.has_datafromsolver() ) {
		return;
	}

	SolverProtocol::Packet_DataFromSolver solverPacket = packet.datafromsolver();

    if( solverPacket.has_solution_manual_altitude() ||
        solverPacket.has_solution_automatic_altitude() )
    {
        SolverProtocol::Packet solutionPacket;
        if(solverPacket.has_solution_manual_altitude()) {
            createSolverPacketSolutionManual(solutionPacket,
                                             solverPacket.solution_manual_altitude());
        }

        if(solverPacket.has_solution_automatic_altitude()) {
            createSolverPacketSolutionAuto(solutionPacket,
                                             solverPacket.solution_automatic_altitude());
        }

        foreach (ISolverListener* listener, m_receiversList) {
            QByteArray blaRawData;
            blaRawData.resize(solutionPacket.ByteSize());
            solutionPacket.SerializeToArray( blaRawData.data(), blaRawData.size() );
            listener->onSolverBlaData(blaRawData);
        }

		int size = solverPacket.solution_manual_altitude().trajectory().size();
		bool anyPackage = false;
		anyPackage |= solverPacket.solution_manual_altitude().has_particles();
		anyPackage |= solverPacket.solution_manual_altitude().has_singlemarks();
		anyPackage |= solverPacket.solution_manual_altitude().has_positionlines();
        if(solverPacket.solution_manual_altitude().has_positionlines()) {
            int sz = solverPacket.solution_manual_altitude().positionlines().positionline_size();
            int k = 0;
            k++;
        }

		if( size > 0 ) {
			SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory trajectory = solverPacket.solution_manual_altitude().trajectory().Get( size-1 );
			int motionSize = trajectory.motionestimate().size();

			if(motionSize > 0) {
				SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate motionPacket =
						trajectory.motionestimate().Get( motionSize - 1 );

				foreach (ISolverListener* listener, m_receiversList) {
                    listener->onSolver1ProtoData( (int)motionPacket.quality(), QByteArray() );
				}
			}
		} else if( anyPackage ) {
			foreach (ISolverListener* listener, m_receiversList) {
                listener->onSolver1ProtoData( (int)SolverProtocol::BAD_QUALITY, QByteArray() );
			}
		}
	}

	if( solverPacket.has_solverresponse() ) {
        SolverProtocol::Packet responsePacket;
		foreach (ISolverListener* listener, m_receiversList) {
			listener->onSolver1SetupAnswer( inputData );
		}

        createSolverResponseMessage(responsePacket, solverPacket.solverresponse());

        bool test = isSolverMessageSolverResponse(responsePacket);

        QByteArray messageRawData;
        messageRawData.resize(responsePacket.ByteSize());
        responsePacket.SerializeToArray(messageRawData.data(), messageRawData.size());

		foreach (ISolverListener* listener, m_receiversList) {
            listener->onSolver1ProtoData( (int)SolverProtocol::UNKNOWN_QUALITY, messageRawData );
		}

        foreach (ISolverListener* listener, m_receiversList) {
            listener->onSolverWorkData( messageRawData );
        }
	}

	if( solverPacket.has_message() ) {
        SolverProtocol::Packet messagePacket;

		foreach (ISolverListener* listener, m_receiversList) {
			listener->onErrorOccured( INTERNAL_ERROR, QString::fromStdString(solverPacket.message().message()) );
		}

        createSolverPacketMessage(messagePacket, solverPacket.message());

        QByteArray messageRawData;
        messageRawData.resize(messagePacket.ByteSize());
        messagePacket.SerializeToArray(messageRawData.data(), messageRawData.size());

		foreach (ISolverListener* listener, m_receiversList) {
            listener->onSolver1ProtoData( (int)SolverProtocol::UNKNOWN_QUALITY, messageRawData );
		}

        foreach (ISolverListener* listener, m_receiversList) {
            listener->onSolverWorkData( messageRawData );
        }
	}
}

MessageSP SolverEncoder1::encode(const QByteArray &data)
{
	m_dataFromTcpSocket.append(data);

	if (m_dataFromTcpSocket.size() < TCP_ZAVIRUHA_PREAMBULA_LEN1){
		return MessageSP();
	}

	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_ZAVIRUHA_PREAMBULA_SOLVER1);
	m_dataFromTcpSocket = m_dataFromTcpSocket.right( m_dataFromTcpSocket.length() - preambulaIndex );
	if (preambulaIndex < 0) {
		return MessageSP();
	}

	memcpy( (char*)&m_residueLength, m_dataFromTcpSocket.data() + (TCP_ZAVIRUHA_PREAMBULA_LEN1-sizeof(uint)),
			sizeof(uint) );

	QByteArray dataToSend;
	int packetLen = m_residueLength + TCP_ZAVIRUHA_PREAMBULA_LEN1;

	if (m_dataFromTcpSocket.length() >= packetLen) {
		QByteArray data = m_dataFromTcpSocket.mid(preambulaIndex + TCP_ZAVIRUHA_PREAMBULA_LEN1, m_residueLength);
		dataToSend = data;

		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - packetLen);
		m_residueLength = 0;
	}

	return MessageSP(new Message<QByteArray>(TCP_SOLVER_MESSAGE, dataToSend));
}

QByteArray SolverEncoder1::decode(const MessageSP message) {
	QByteArray dataToSend;

	if( message->type() == CLIENT_TCP_SERVER_SOLVER_DATA_1 ) {
		dataToSend = message->data();
	}

	addPreambula(dataToSend);
	return dataToSend;
}

void SolverEncoder1::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend(TCP_ZAVIRUHA_PREAMBULA_SOLVER1);
}

void SolverEncoder1::sendSolverAnswerToGui(const QByteArray &data)
{

}
