#include "TcpServerCoder.h"

TcpServerCoder::TcpServerCoder(QObject *parent) :
	BaseTcpDeviceCoder(parent)
{
}

TcpServerCoder::~TcpServerCoder()
{
}

MessageSP TcpServerCoder::encode(const QByteArray &data)
{
	log_debug(" tcpServer encode ");
	m_dataFromTcpSocket.append(data);

	if (m_dataFromTcpSocket.size() < TCP_ZAVIRUHA_PREAMBULA_LEN){
		return MessageSP();
	}

	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_ZAVIRUHA_PREAMBULA);
	if (preambulaIndex < 0){
		return MessageSP();
	}

	memcpy((char*)&m_residueLength, m_dataFromTcpSocket.data() + 3, sizeof(uint));

	MessageSP dataToSend;
	int packetLen = m_residueLength + TCP_ZAVIRUHA_PREAMBULA_LEN;

	if (m_dataFromTcpSocket.length() >= packetLen) {
		QByteArray data = m_dataFromTcpSocket.mid(preambulaIndex + TCP_ZAVIRUHA_PREAMBULA_LEN, m_residueLength);
		dataToSend = fromProtoBuf(data);

		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - packetLen);
		m_residueLength = 0;
	}

	return dataToSend;
}

QByteArray TcpServerCoder::decode(const MessageSP message)
{
	QString messageType = message->type();
	QByteArray messageData = message->data();

	Zaviruha::Packet packet;
	Zaviruha::Packet::Command* packetCommand = new Zaviruha::Packet::Command();
	packet.set_allocated_command(packetCommand);

	QByteArray dataToSend;

	if (messageType == QString(ARM_R_SERVER_BPLA_COORDS)) {
		packetCommand->set_action(Zaviruha::sendBplaPoints);
	} else if (messageType == QString(ARM_R_SERVER_BPLA_COORDS_AUTO)) {
		packetCommand->set_action(Zaviruha::sendBplaPointsAuto);
	} else if (messageType == QString(ARM_R_SERVER_ATLANT_DIRECTION)) {
		dataToSend = messageData;
		addPreambula(dataToSend);
		return messageData;
	} else if (messageType == QString(ARM_R_SERVER_ATLANT_POSITION)) {
		dataToSend = messageData;
		addPreambula(dataToSend);
		return messageData;
	}

	if (messageType == QString(ARM_R_SERVER_BPLA_COORDS)
		|| messageType == QString(ARM_R_SERVER_BPLA_COORDS_AUTO)) {
		Zaviruha::Packet::ArgumentVariant* packetArgs = new Zaviruha::Packet::ArgumentVariant();
		Zaviruha::Packet::ArgumentVariant::BplaPoints* arg = packetArgs->add_bplapoints();
		packetCommand->set_allocated_arguments(packetArgs);
		toProtoBufBplaCoords(arg, messageData);
	}

	unsigned int size = packet.ByteSize();
	dataToSend.resize(size);
	packet.SerializeToArray(dataToSend.data(), size);

	addPreambula(dataToSend);

	return dataToSend;
}

QObject *TcpServerCoder::asQObject()
{
	return this;
}

MessageSP TcpServerCoder::fromProtoBuf(const QByteArray& data)
{
	Zaviruha::Packet packet;
	if (!packet.ParseFromArray(data, data.size())){
		return MessageSP();
	}

	QString method = "";

	switch (packet.command().action()){
		case Zaviruha::sendSolverClear:
			method = TCP_ARMR_SEND_SOLVER_CLEAR;
			break;
		case Zaviruha::sendSolverData:
			method = TCP_ARMR_SEND_SOLVER_DATA;
			break;
		default:
			break;
	}

	return MessageSP(new Message<QByteArray>(method, data));
}

QByteArray TcpServerCoder::toProtoBuf(const MessageSP& message)
{
	Q_UNUSED(message);

	return QByteArray();
}

void TcpServerCoder::toProtoBufBplaCoords(Zaviruha::Packet::ArgumentVariant::BplaPoints* arg, QByteArray &data)
{
	QDataStream ds(&data, QIODevice::ReadOnly);
	QTime t;
	int state;
	QPointF pointStdDev;
	double speed;
	double alt;
	double course;
	QVector<QPointF> track;
	ds >> t >> state >> pointStdDev >> track >> speed >> alt >> course;

	arg->set_course(course);
	arg->set_speed(speed);
	arg->set_altitude(alt);
	arg->set_state(state);

	QDateTime timestamp;
	timestamp.setTime(t);
	arg->set_time(timestamp.toMSecsSinceEpoch());

	Zaviruha::Packet_ArgumentVariant_BplaPoints_Point* pointStdDevProto = new Zaviruha::Packet_ArgumentVariant_BplaPoints_Point();
	pointStdDevProto->set_x(pointStdDev.x());
	pointStdDevProto->set_y(pointStdDev.y());
	arg->set_allocated_pointstddev(pointStdDevProto);

	foreach (QPointF point, track){
		Zaviruha::Packet_ArgumentVariant_BplaPoints_Point* pointProto = arg->add_track();
		pointProto->set_x(point.x());
		pointProto->set_y(point.y());
	}
}

void TcpServerCoder::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend(TCP_ZAVIRUHA_PREAMBULA);
}
