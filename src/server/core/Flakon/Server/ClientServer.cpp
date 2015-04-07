#include "ClientServer.h"

ClientTcpServer::ClientTcpServer(QObject* parent) :
	BaseTcpServer(parent)
{
	m_encoder = new SolverEncoder(this);
	this->registerReceiver(m_encoder);
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
			QByteArray dataToSend = decode(argument);
			bool res = sendData( dataToSend );
			emit onDataSended(res);
		}
	}
	break;
	default:
	break;
	}
}

SolverEncoder*ClientTcpServer::getSolverEncoder()
{
	return m_encoder;
}

int ClientTcpServer::getClientTcpPortValue() {
	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	return settings.value("ClientTCPServer/Port", TCP_SERVER_PORT).toInt();
}

QByteArray ClientTcpServer::decode(const MessageSP message) {
	QByteArray dataToSend;

	//Zaviruha::Packet packet;
	SolverClient::Packet packet;

	SolverClient::Packet::Command* packetCommand = new SolverClient::Packet::Command();
	packet.set_allocated_command(packetCommand);

	if( message->type() == CLIENT_TCP_SERVER_SOLVER_DATA ) {
		packetCommand->set_action(SolverClient::sendSolverClientData);

		SolverClient::Packet::ArgumentVariant* packetArgs = new SolverClient::Packet::ArgumentVariant();
		SolverClient::Packet::ArgumentVariant::SolverInput* arg = new SolverClient::Packet::ArgumentVariant::SolverInput();
		packetArgs->set_allocated_solverinput(arg);
		packetCommand->set_allocated_arguments(packetArgs);
		toProtobufSolverData( arg, message->data() );
	} else if( message->type() == CLIENT_TCP_SERVER_BPLA_DATA ) {
		packetCommand->set_action(SolverClient::sendSolverClientBla);
	} else {
		return QByteArray();
	}

	unsigned int size = packet.ByteSize();
	dataToSend.resize(size);
	packet.SerializeToArray(dataToSend.data(), size);

	addPreambula(dataToSend);
	return dataToSend;
}

void ClientTcpServer::toProtobufSolverData(SolverClient::Packet::ArgumentVariant::SolverInput* arg, QByteArray &data) {
	QDataStream stream(&data, QIODevice::ReadOnly);

	DataFromFlacon receiveData;
	double centerFrequency;
	stream >> receiveData.numOfReferenceDetector_;
	stream >> receiveData.time_;
	stream >> receiveData.ranges_;
	stream >> centerFrequency;

	foreach (double range, receiveData.ranges_) {
		arg->add_delays(range);
	}

	QDateTime dateTime = QDateTime::currentDateTime();
	//Fill time form solver
	//TODO new solver has QDateTime
	dateTime.setTime(receiveData.time_);
	arg->set_datetime(dateTime.toMSecsSinceEpoch());
	arg->set_centerfrequency(centerFrequency);
}



void ClientTcpServer::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend(TCP_ZAVIRUHA_PREAMBULA);
}
