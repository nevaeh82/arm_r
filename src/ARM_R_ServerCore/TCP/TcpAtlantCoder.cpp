#include "TcpAtlantCoder.h"

TcpAtlantCoder::TcpAtlantCoder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_residueLength = 0;
}

TcpAtlantCoder::~TcpAtlantCoder()
{
}

MessageSP TcpAtlantCoder::encode(const QByteArray& data)
{
	m_dataFromTcpSocket.append(data);

	/// TODO: recheck
	MessageSP dataToSend;

	/// WTF??
	if (m_residueLength == 0) {
		QByteArray len = m_dataFromTcpSocket.left(sizeof(int));
		memcpy(&m_residueLength, len.data(), 4);
		m_dataFromTcpSocket.remove(0, sizeof(int));
	}

	if (m_dataFromTcpSocket.length() >= m_residueLength) {
		QByteArray rest_msg = m_dataFromTcpSocket.right(m_dataFromTcpSocket.size() - m_residueLength);
		m_dataFromTcpSocket.truncate(m_residueLength);
		dataToSend = messageFromPreparedData();
		m_dataFromTcpSocket.clear();
		m_dataFromTcpSocket = rest_msg;
		m_residueLength = 0;
	}

	return dataToSend;
}

QByteArray TcpAtlantCoder::decode(const MessageSP message)
{
	QByteArray dataToSend;
	if (message->type() == TCP_ATLANT_REQUEST_SET_FREQUENCY) {
		dataToSend = atlantSetFrequency(message->data());
	}
	return dataToSend;
}

QObject* TcpAtlantCoder::asQObject()
{
	return this;
}

MessageSP TcpAtlantCoder::messageFromPreparedData()
{
	EMS::EagleMessage e_msg;
	e_msg.ParseFromArray(m_dataFromTcpSocket.constData(), m_dataFromTcpSocket.size());
	QString type = QString::fromStdString(e_msg.type());
	QByteArray ba = QByteArray::fromRawData(e_msg.innermessage().c_str(), e_msg.innermessage().size());

	if(type == Atlant_Direction_MsgA) {
		Storm::DirectionAnswerMessage d_msg;
		d_msg.ParseFromArray(ba.constData(), ba.size());

		QByteArray dataToSend = toProtobuf(d_msg);
		return MessageSP(new Message<QByteArray>(TCP_ATLANT_ANSWER_DIRECTION, dataToSend));
	}
	else if (type == Atlant_Position_MsgA) {
		Storm::PositionAnswerMessage d_msg;
		d_msg.ParseFromArray(ba.constData(), ba.size());

		QByteArray dataToSend = toProtobuf(d_msg);
		return MessageSP(new Message<QByteArray>(TCP_ATLANT_ANSWER_POSITION, dataToSend));
	}

	return MessageSP();
}

QByteArray TcpAtlantCoder::atlantSetFrequency(const QByteArray& data)
{
	/// WTF??
	QByteArray inputData = data;
	QDataStream dataStream(&inputData, QIODevice::ReadOnly);

	int id;
	int post;
	double freq;
	double width;

	dataStream >> id;
	dataStream >> post;
	dataStream >> freq;
	dataStream >> width;

	debug(QString("%1 %2 %3").arg(QString::number(id)).arg(QString::number(freq)).arg(QString::number(width)));

	EMS::EagleMessage e_msg;
	QString type = Atlant_Direction_Finding_Request;

	QDateTime dt(QDateTime::currentDateTime());
	qint64 d = 0; //datetime

	QString name = "ARM_R";
	QString label = "ARM_R1";

	e_msg.set_type(type.toStdString().c_str(), type.size());
	e_msg.set_sendername(name.toStdString().c_str(), name.size());
	e_msg.set_datetime(d);
	e_msg.add_label(label.toStdString().c_str(), label.size());

	Storm::DirectionFindingRequest d_msg;
	d_msg.set_id(id);
	d_msg.add_frequency(freq);
	d_msg.set_width(width);

	std::string inner = d_msg.SerializeAsString();

	e_msg.set_innermessage(inner);

	std::string message = e_msg.SerializeAsString();

	QByteArray dataToReturn;

	debug(QString("message size %1").arg(message.size()));

	unsigned int size = message.size();
	dataToReturn.append(reinterpret_cast<char *>(&size), sizeof(size));
	dataToReturn.append(message.c_str(), message.size());

	return dataToReturn;
}

QByteArray TcpAtlantCoder::toProtobuf(const Storm::DirectionAnswerMessage& d_msg)
{
	Zaviruha::Packet packet;
	Zaviruha::Packet::Command* packetCommand = new Zaviruha::Packet::Command();
	Zaviruha::Packet::ArgumentVariant* packetArgs = new Zaviruha::Packet::ArgumentVariant();
	Zaviruha::Packet::ArgumentVariant::Header* packetHeader = new Zaviruha::Packet::ArgumentVariant::Header();
	Zaviruha::Packet::ArgumentVariant::AtlantDirection* packetAtlDir = packetArgs->add_atlantdirection();

	packet.set_allocated_command(packetCommand);
	packetCommand->set_allocated_arguments(packetArgs);
	packetCommand->set_action(Zaviruha::sendAtlantDirection);
	packetAtlDir->set_allocated_header(packetHeader);

	packetHeader->set_requestid(d_msg.requestid());
	packetHeader->set_sourceid(d_msg.sourceid());
	packetHeader->set_datetime(d_msg.datetime());

	packetAtlDir->set_postname(d_msg.post());
	packetAtlDir->set_postlatitude(d_msg.postlatitude());
	packetAtlDir->set_postlongitude(d_msg.postlongitude());
	packetAtlDir->set_postheight(d_msg.postheight());
	packetAtlDir->set_frequency(d_msg.frequency());
	packetAtlDir->set_width(d_msg.widht());
	packetAtlDir->set_direction(d_msg.direction());
	packetAtlDir->set_angle(d_msg.angle());
	packetAtlDir->set_level(d_msg.level());
	packetAtlDir->set_quality(d_msg.quality());
	packetAtlDir->set_motiontype(d_msg.motiontype());
	packetAtlDir->set_motionconfidence(d_msg.motionconfidence());

	QByteArray dataToSend;
	unsigned int size = packet.ByteSize();
	dataToSend.resize(size);
	packet.SerializeToArray(dataToSend.data(), size);

	return dataToSend;

	/*A_Dir_Ans_msg ad_struct;
	ad_struct.requestId			= d_msg.requestid();
	ad_struct.sourceId			= d_msg.sourceid();
	ad_struct.dateTime			= d_msg.datetime();
	ad_struct.post				= QString::fromStdString(d_msg.post());
	ad_struct.postLatitude		= d_msg.postlatitude();
	ad_struct.postLongitude		= d_msg.postlongitude();
	ad_struct.postHeight		= d_msg.postheight();
	ad_struct.frequency			= d_msg.frequency();
	ad_struct.widht				= d_msg.widht();
	ad_struct.direction			= d_msg.direction();
	ad_struct.angle				= d_msg.angle();
	ad_struct.level				= d_msg.level();
	ad_struct.quality			= d_msg.quality();
	ad_struct.motionType		= d_msg.motiontype();
	ad_struct.motionConfidence	= d_msg.motionconfidence();*/

	/*QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::ReadWrite);
	dataStream << ad_struct.requestId;
	dataStream << ad_struct.sourceId;
	dataStream << ad_struct.dateTime;
	dataStream << ad_struct.post;
	dataStream << ad_struct.postLatitude;
	dataStream << ad_struct.postLongitude;
	dataStream << ad_struct.postHeight;
	dataStream << ad_struct.frequency;
	dataStream << ad_struct.widht;
	dataStream << ad_struct.direction;
	dataStream << ad_struct.angle;
	dataStream << ad_struct.level;
	dataStream << ad_struct.quality;
	dataStream << ad_struct.motionType;
	dataStream << ad_struct.motionConfidence;*/
}

QByteArray TcpAtlantCoder::toProtobuf(const Storm::PositionAnswerMessage& d_msg)
{
	Zaviruha::Packet packet;
	Zaviruha::Packet::Command* packetCommand = new Zaviruha::Packet::Command();
	Zaviruha::Packet::ArgumentVariant* packetArgs = new Zaviruha::Packet::ArgumentVariant();
	Zaviruha::Packet::ArgumentVariant::Header* packetHeader = new Zaviruha::Packet::ArgumentVariant::Header();
	Zaviruha::Packet::ArgumentVariant::AtlantPosition* packetAtlPos = packetArgs->add_atlantposition();

	packet.set_allocated_command(packetCommand);
	packetCommand->set_allocated_arguments(packetArgs);
	packetCommand->set_action(Zaviruha::sendAtlantPosition);
	packetAtlPos->set_allocated_header(packetHeader);

	packetHeader->set_requestid(d_msg.requestid());
	packetHeader->set_sourceid(d_msg.sourceid());
	packetHeader->set_datetime(d_msg.datetime());

	packetAtlPos->set_latitude(d_msg.latitude());
	packetAtlPos->set_longitude(d_msg.longitude());
	packetAtlPos->set_quality(d_msg.quality());

	QByteArray dataToSend;
	unsigned int size = packet.ByteSize();
	dataToSend.resize(size);
	packet.SerializeToArray(dataToSend.data(), size);

	return dataToSend;
	/*A_Pos_Ans_msg ad_struct;
	ad_struct.requestId			= d_msg.requestid();
	ad_struct.sourceId			= d_msg.sourceid();
	ad_struct.dateTime			= d_msg.datetime();
	ad_struct.longitude			= d_msg.longitude();
	ad_struct.latitude			= d_msg.latitude();
	ad_struct.quality			= d_msg.quality();*/

	/*QByteArray dataToSend;
	QDataStream dataStream(&dataToSend, QIODevice::ReadWrite);
	dataStream << ad_struct.requestId;
	dataStream << ad_struct.sourceId;
	dataStream << ad_struct.dateTime;
	dataStream << ad_struct.longitude;
	dataStream << ad_struct.latitude;
	dataStream << ad_struct.quality;*/
}
