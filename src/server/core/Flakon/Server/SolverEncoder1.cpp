#include "Logger/Logger.h"
#include "SolverEncoder1.h"

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

	if( solverPacket.has_solution_manual_altitude() ) {
		int size = solverPacket.solution_manual_altitude().trajectory().size();
		bool anyPackage = false;
		anyPackage |= solverPacket.solution_manual_altitude().has_particles();
		anyPackage |= solverPacket.solution_manual_altitude().has_singlemarks();
		anyPackage |= solverPacket.solution_manual_altitude().has_positionlines();

		if( size > 0 ) {
			SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory trajectory = solverPacket.solution_manual_altitude().trajectory().Get( size-1 );
			int motionSize = trajectory.motionestimate().size();

			if(motionSize > 0) {
				SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate motionPacket =
						trajectory.motionestimate().Get( motionSize - 1 );

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSolver1ProtoData( (int)motionPacket.quality(), inputData );
				}
			}
		} else if( anyPackage ) {
			foreach (ISolverListener* listener, m_receiversList) {
				listener->onSolver1ProtoData( (int)SolverProtocol::BAD_QUALITY, inputData );
			}
		}
	}

	if( solverPacket.has_solution_automatic_altitude() ) {
		int size = solverPacket.solution_automatic_altitude().trajectory_size();

		bool anyPackage = false;
		anyPackage |= solverPacket.solution_automatic_altitude().has_particles();
		anyPackage |= solverPacket.solution_automatic_altitude().has_singlemarks();
		anyPackage |= solverPacket.solution_automatic_altitude().has_positionlines();

		if( size > 0 ) {
			SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory trajectory = solverPacket.solution_automatic_altitude().trajectory().Get( size-1 );
			int motionSize = trajectory.motionestimate().size();

			if(motionSize > 0) {
				SolverProtocol::Packet_DataFromSolver_SolverSolution_Trajectory_MotionEstimate motionPacket =
						trajectory.motionestimate().Get( motionSize - 1 );

				foreach (ISolverListener* listener, m_receiversList) {
					listener->onSolver1ProtoData( motionPacket.quality(), inputData );
				}
			}
		} else if( anyPackage ) {
			foreach (ISolverListener* listener, m_receiversList) {
				listener->onSolver1ProtoData( (int)SolverProtocol::BAD_QUALITY, inputData );
			}
		}
	}

	if( solverPacket.has_data_associations_manual_altitude() ) {
		int k = 0;
	}

	if( solverPacket.has_data_associations_automatic_altitude() ) {
		int k = 0;
	}

	if( solverPacket.has_solverresponse() ) {
		int k = 0;
		foreach (ISolverListener* listener, m_receiversList) {
			listener->onSolver1SetupAnswer( inputData );
		}
	}

	if( solverPacket.has_message() ) {
		int k = 0;

		foreach (ISolverListener* listener, m_receiversList) {
			listener->onErrorOccured( INTERNAL_ERROR, QString::fromStdString(solverPacket.message().message()) );
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

	//Zaviruha::Packet packet;
//	SolverClient::Packet packet;

//	SolverClient::Packet::Command* packetCommand = new SolverClient::Packet::Command();
//	packet.set_allocated_command(packetCommand);

//	if( message->type() == CLIENT_TCP_SERVER_SOLVER_DATA ) {
//		packetCommand->set_action(SolverClient::sendSolverClientData);

//		SolverClient::Packet::ArgumentVariant* packetArgs = new SolverClient::Packet::ArgumentVariant();
//		SolverClient::Packet::ArgumentVariant::SolverInput* arg = new SolverClient::Packet::ArgumentVariant::SolverInput();
//		packetArgs->set_allocated_solverinput(arg);
//		packetCommand->set_allocated_arguments(packetArgs);
//		toProtobufSolverData( arg, message->data() );
//	} else if( message->type() == CLIENT_TCP_SERVER_BPLA_DATA ) {
//		packetCommand->set_action(SolverClient::sendSolverClientBla);
//	} else {
//		return QByteArray();
//	}

//	unsigned int size = packet.ByteSize();
//	dataToSend.resize(size);
//	packet.SerializeToArray(dataToSend.data(), size);

//	return dataToSend;

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
