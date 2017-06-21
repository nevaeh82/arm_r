#include "ClientServer.h"

//Now goto use for NIIPP server

ClientTcpServer::ClientTcpServer(int port, int type, QObject* parent) :
	BaseTcpServer(parent),
	m_port(port),
	m_type(type)
{
	if(type == 0) {
		m_solverEncoder = new SolverEncoder(this);
		m_encoder = m_solverEncoder;

		connect(m_solverEncoder, SIGNAL(nippDataIncome(QString, bool)), this, SIGNAL(signalNiippData(QString,bool)));
		connect(m_solverEncoder, SIGNAL(nippDataIncome(QString, bool)), this, SLOT(slotNiipDataIncome(QString,bool)));
	} else {
		m_solverEncoder = NULL;
		m_sprutEncoder = new SprutEncoder(this);
		m_encoder = m_sprutEncoder;

		connect(m_sprutEncoder, SIGNAL(signalSprutIncome(QByteArray)), this, SLOT(slotSprutIncome(QByteArray)));
	}

	this->registerReceiver(m_encoder);

	connect(this, SIGNAL(newClientSignal(uint,ITcpServerClient*)), this, SLOT(onRegisterNewConnection(uint,ITcpServerClient*)));
}

ClientTcpServer::~ClientTcpServer()
{
	stop();
}

void ClientTcpServer::startServer()
{
	start( QHostAddress::Any, m_port );
}

void ClientTcpServer::stopServer()
{
	stop();
}

void ClientTcpServer::onRegisterNewConnection(uint n, ITcpServerClient * client)
{
	Q_UNUSED(n)

//	QByteArray dataToSend = m_encoder->decodeRegister();

	//	client->writeData(dataToSend);
}

void ClientTcpServer::slotNiipDataIncome(QString, bool status)
{
	if(status) {
	//Todo maybe answer to NIIP
	}
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
			if(m_solverEncoder) {
				QByteArray dataToSend = m_solverEncoder->decode(argument);
				bool res = sendData( dataToSend );
				emit onDataSended(res);
			}
		} else if(messageType == CLIENT_TCP_SERVER_KTR_DATA) {
			if(m_solverEncoder) {
				QByteArray dataToSend = m_solverEncoder->decode(argument);
				bool res = sendData( dataToSend );
				emit onDataSended(res);
			}
		}
	}
	break;
	default:
	break;
	}
}

SolverEncoder* ClientTcpServer::getSolverEncoder()
{
	return m_solverEncoder;
}


void ClientTcpServer::slotSprutIncome(QByteArray data)
{
	m_sprutBuffer.append(data);
	QByteArray preambula;
	preambula.append(0xAA);
	preambula.append(0xBB);
	preambula.append(0xCC);
	preambula.append(0xDD);

	if(m_sprutBuffer.size() < 6) {
		return;
	}

	int index = m_sprutBuffer.indexOf(preambula);

	if(index < 0) {
		return;
	}

	if(m_sprutBuffer.size() > preambula.size() + 1) {
		if(m_sprutBuffer.size() < (preambula.size() + 1 + (int)m_sprutBuffer.at(preambula.size() + 1)) ) {
			return;
		}
	} else {
		return;
	}

	m_sprutBuffer = m_sprutBuffer.right( m_sprutBuffer.length() - index - preambula.size() );

	int size = (int)m_sprutBuffer.at(0);
	int value = (int)m_sprutBuffer.at(1);

	if(value == 1) {
		emit signalNiippData("SPROUT", true);
	} else {
		emit signalNiippData("SPROUT", false);
	}
}
