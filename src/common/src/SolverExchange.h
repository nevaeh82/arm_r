#pragma once

#include <QByteArray>

#include "SolverPacket1.pb.h"

void createSolverPacketSolutionManual(SolverProtocol::Packet& pkt, const SolverProtocol::Packet_DataFromSolver_SolverSolution& solution)
{
	pkt.mutable_datafromsolver()->mutable_solution_manual_altitude()->CopyFrom(solution);
}

void createSolverPacketSolutionAuto(SolverProtocol::Packet& pkt, const SolverProtocol::Packet_DataFromSolver_SolverSolution& solution)
{
	pkt.mutable_datafromsolver()->mutable_solution_automatic_altitude()->CopyFrom(solution);
}

void createSolverPacketMessage(SolverProtocol::Packet& pkt, const SolverProtocol::Packet_DataFromSolver_SolverMessage& message)
{
	pkt.mutable_datafromsolver()->mutable_message()->CopyFrom(message);
}

void createSolverResponseMessage(SolverProtocol::Packet& pkt, const SolverProtocol::Packet_DataFromSolver_SolverResponse& message)
{
	pkt.mutable_datafromsolver()->mutable_solverresponse()->CopyFrom(message);
}

//Check has commands
inline bool isSolverMessageCommandSolver( SolverProtocol::Packet& pkt )
{
	if( !pkt.has_datafromsolver() ) {
		return false;
	}

	if( !pkt.datafromsolver().has_message() ) {
		return false;
	}

	return true;
}

inline bool isSolverMessageSolverMessage( SolverProtocol::Packet& pkt )
{
	if( !pkt.has_datafromsolver() ) {
		return false;
	}

	if( !pkt.datafromsolver().has_message() ) {
		return false;
	}

	return true;
}

inline bool isSolverMessageSolverResponse( SolverProtocol::Packet& pkt )
{
	if( !pkt.has_datafromsolver() ) {
		return false;
	}

	if( !pkt.datafromsolver().has_solverresponse() ) {
		return false;
	}

	return true;
}

