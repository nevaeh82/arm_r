#include "ClientServer.h"

ClientTcpServer::ClientTcpServer(QObject* parent) :
	BaseTcpServer(parent)
{
}

ClientTcpServer::~ClientTcpServer()
{
	stop();
}

void ClientTcpServer::startServer()
{
	start( QHostAddress::Any, getClientTcpPortValue() );
}

bool ClientTcpServer::sendClientSolverData(DataFromFlacon clientData)
{
	QByteArray dataToSend;

	QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
	dataStream << QString(CLIENT_SOLVER_DATA);
	dataStream << clientData;

	return sendData(dataToSend);
}

int ClientTcpServer::getClientTcpPortValue() {
	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	return settings.value("ClientTCPServer/Port", TCP_SERVER_PORT).toInt();
}
