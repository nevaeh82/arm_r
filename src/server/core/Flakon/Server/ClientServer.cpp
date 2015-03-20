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

void ClientTcpServer::stopServer()
{
	stop();
}

void ClientTcpServer::onMessageReceived(const quint32 deviceType, const QString& device, const MessageSP argument)
{
	Q_UNUSED(device);

	QString messageType = argument->type();
	QVariant data = QVariant( argument->data() );

	switch (deviceType) {
	case CLIENT_TCP_SERVER:
	{
		if(messageType == CLIENT_TCP_SERVER_SOLVER_DATA) {
			bool res = sendData(data.toByteArray());
			emit onDataSended(res);
		}
	}
	break;
	default:
	break;
	}
}

int ClientTcpServer::getClientTcpPortValue() {
	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	return settings.value("ClientTCPServer/Port", TCP_SERVER_PORT).toInt();
}
