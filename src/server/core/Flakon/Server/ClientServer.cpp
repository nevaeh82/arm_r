#include "ClientServer.h"

//Now goto use for NIIPP server

ClientTcpServer::ClientTcpServer(QObject* parent) :
	BaseTcpServer(parent)
{
	m_encoder = new SolverEncoder(this);

	this->registerReceiver(m_encoder);

	connect(this, SIGNAL(newClientSignal(uint,ITcpServerClient*)), this, SLOT(onRegisterNewConnection(uint,ITcpServerClient*)));
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

void ClientTcpServer::onRegisterNewConnection(uint n, ITcpServerClient * client)
{
	Q_UNUSED(n)

	QByteArray dataToSend = m_encoder->decodeRegister();

	client->writeData(dataToSend);
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
			QByteArray dataToSend = m_encoder->decode(argument);
			bool res = sendData( dataToSend );
			emit onDataSended(res);
		}
	}
	break;
	default:
	break;
	}
}

SolverEncoder* ClientTcpServer::getSolverEncoder()
{
	return m_encoder;
}

int ClientTcpServer::getClientTcpPortValue() {
	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	return settings.value("ClientTCPServer/Port", TCP_SERVER_PORT).toInt();
}
