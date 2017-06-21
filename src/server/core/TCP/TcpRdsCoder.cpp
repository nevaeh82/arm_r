#include <Logger/Logger.h>

#include "RDSExchange.h"
#include "TcpRdsCoder.h"

#include <QDebug>
#include <QSettings>
#include <QTextCodec>

#define TO_KHZ 1000
#define TIME_DEL 1000
#define PUSH_TIMEOUT 350

#define SERVER_ID_LOG_ENABLE 3

TcpRdsCoder::TcpRdsCoder(unsigned int zone, unsigned int typeRDS, unsigned int serverId, QObject* parent) :
	BaseTcpDeviceCoder(parent),
	m_indexConv1(-1),
	m_indexConv2(-1),
	m_indexSpect(-1),
	m_serverId(serverId)
{
	m_readyToSend = true;
	m_header.zone = zone;
	m_header.typeRds = typeRDS;

	m_readyToPushSpectrum = true;

	m_residueLength = 0;
	cnt = 0;

	m_specTime = QDateTime::currentDateTime();

	m_inputData = spectrum;

	QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	m_upTime = settings.value("SpectrumUpTime/time", TIME_DEL).toInt();

	m_dropFlagTimer = new QTimer(this);
	connect(m_dropFlagTimer, SIGNAL(timeout()), this, SLOT(onDropFlag()));
	m_dropFlagTimer->setInterval(5000);
	m_dropFlagTimer->start();

	m_pushTime = QTime::currentTime();
}

TcpRdsCoder::~TcpRdsCoder()
{
}

MessageSP TcpRdsCoder::encode(const QByteArray& data)
{
	MessageSP message;

	QByteArray inData = getMessage(data);

	if(inData.isEmpty()) {
		return MessageSP(new Message<QByteArray>(TCP_EMPTY_MESSAGE, inData));
	}

	if(m_dataFromTcpSocket.length() >= m_residueLength) {
		message = messageFromPreparedData(inData);
	}

	return message;
}

QByteArray TcpRdsCoder::getMessage(const QByteArray& input)
{
	m_dataFromTcpSocket.append(input);

	if ( m_dataFromTcpSocket.size() < 13 ) {
		return QByteArray();
	}

	int preambulaIndex = m_dataFromTcpSocket.indexOf(TCP_RDS_PREAMBULA);

	if ( preambulaIndex < 0 ) {
		return QByteArray();
	}

	m_dataFromTcpSocket = m_dataFromTcpSocket.right( m_dataFromTcpSocket.length() - preambulaIndex );

	memcpy( (char*)&m_residueLength, m_dataFromTcpSocket.data() + 9, sizeof(uint) );

	QByteArray dataToSend;
	int packetLen = m_residueLength + 13;

	if (m_dataFromTcpSocket.length() >= packetLen) {
		QByteArray data = m_dataFromTcpSocket.mid(13, m_residueLength );
		dataToSend = data;

		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - packetLen);
		m_residueLength = 0;
	}

	return dataToSend;
}

//Decode to send Message to RDS
QByteArray TcpRdsCoder::decode(const MessageSP message)
{
	QByteArray dataToSend;

	QByteArray msgData = message->data();

	RdsProtobuf::Packet msg;

	if( message->type() == TCP_RDS_SEND_PROTO ) {
		RdsProtobuf::Packet pkt = unpack(msgData);
		return msgData;
	}
	else {
		return QByteArray();
	}

	int size = msg.ByteSize();
	dataToSend.resize(size);
	msg.SerializeToArray(dataToSend.data(), dataToSend.size());

	addPreambula(dataToSend);
	return dataToSend;
}

//Decode to send Message to RDS
QByteArray TcpRdsCoder::decodeWithCheckLocation(const MessageSP message, bool &isLocation, bool &isNonStop)
{
	QByteArray dataToSend;
	isLocation = false;
	isNonStop = true;

	QByteArray msgData = message->data();

	RdsProtobuf::Packet msg;

	if( message->type() == TCP_RDS_SEND_PROTO ) {
		RdsProtobuf::Packet pkt = unpack(msgData);

		if( pkt.has_from_client() &&
			pkt.from_client().has_one_shot() ) {
			isNonStop = false;
		}

		if( pkt.has_from_client() &&
			pkt.from_client().has_one_shot() &&
			pkt.from_client().one_shot().has_location() ) {

			RdsProtobuf::ClientMessage_OneShot_Location loc = pkt.from_client().one_shot().location();

			isLocation = true;
		}

		return msgData;
	}
	else {
		return QByteArray();
	}

	int size = msg.ByteSize();
	dataToSend.resize(size);
	msg.SerializeToArray(dataToSend.data(), dataToSend.size());

	addPreambula(dataToSend);
	return dataToSend;
}

void TcpRdsCoder::setReadyToPushSpectrums(bool val)
{
	m_readyToPushSpectrum = val;
}

void TcpRdsCoder::addPreambula(QByteArray& data)
{
	uint size = data.size();
	data.prepend((char*)&size, sizeof(unsigned int));
	data.prepend(TCP_RDS_PREAMBULA);
}

QObject* TcpRdsCoder::asQObject()
{
	return this;
}

void TcpRdsCoder::setCoordinatesCounter(CoordinateCounter *obj)
{
	m_coordCounter = obj;
}

//Read message from RDS
MessageSP TcpRdsCoder::messageFromPreparedData(const QByteArray& data)
{
	MessageSP message;

	RdsProtobuf::Packet msg;
	msg.ParseFromArray(data.data(), data.size());

	if( !msg.has_from_server() ) {
		return message;
	}

	RdsProtobuf::ServerMessage sMsg = msg.from_server();

//	if(sMsg.has_current()) {
//		if(m_serverId == SERVER_ID_LOG_ENABLE)
//			log_debug(QString(">>>>>>>> Current received  >> %1").arg(data.size()));
//		m_dropFlagTimer->start();
//		m_readyToSend = true;
//	}

	if( sMsg.has_one_shot_data()) {
		if(m_serverId == SERVER_ID_LOG_ENABLE)
//			log_debug(QString(">>>>>>>> One shot received  >> %1 %2 %3").arg(data.size()).arg(sMsg.one_shot_data().location_data().success())\
//					  .arg(QString::fromStdString(sMsg.one_shot_data().location_data().error())));
			m_dropFlagTimer->start();
			m_readyToSend = true;
	}

	if(sMsg.has_current()) {
		return configureSys( data );
	} else if(sMsg.has_answer()) {
		return configureLoc( data, false );
	} else {
		if(sMsg.has_one_shot_data()) {
			if( !(sMsg.one_shot_data().has_analysis_data()) &&
				!(sMsg.one_shot_data().has_record_data()) &&
				m_readyToPushSpectrum &&
				m_pushTime.elapsed() > m_upTime
			) {
				if(sMsg.one_shot_data().has_location_data()) {
					if(!sMsg.one_shot_data().location_data().success()) {
						return message; // Unsuccess message (
					}
				}

				m_pushTime.restart();
				setReadyToPushSpectrums(false);
				return configureLoc(data, false);
			} else {
				bool b = true;

				if( sMsg.one_shot_data().has_analysis_data() ||
					sMsg.one_shot_data().has_record_data() ) {
					b = false;
				}

//				if(m_serverId == SERVER_ID_LOG_ENABLE)
//					log_debug("spectToALL >>>>>>>>>>>>>>");
				return configureLoc(data, b);
			}

			return message;
		}

		return configureLoc( data, false );
	}

	return message;
}

MessageSP TcpRdsCoder::pointers(int index, float cf, QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_header.zone << m_header.typeRds << index << cf << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_FFT, ba));
}

MessageSP TcpRdsCoder::configureLoc(const QByteArray& data, bool timeout)
{
	if(timeout) {
		return MessageSP(new Message<QByteArray>(TCP_RDS_ANSWER_LOCSYSTEM_TO, data));
	}

	return MessageSP(new Message<QByteArray>(TCP_RDS_ANSWER_LOCSYSTEM, data));
}

MessageSP TcpRdsCoder::configureSys(const QByteArray &data)
{
	return MessageSP(new Message<QByteArray>(TCP_RDS_ANSWER_SYSTEM, data));
}

void TcpRdsCoder::onDropFlag()
{
	m_readyToSend = true;
}

void TcpRdsCoder::onSetFlag()
{
	m_readyToSend = false;
}

MessageSP TcpRdsCoder::correlation(quint32 point1, quint32 point2, float timediff, float veracity, QVector<QPointF> points)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << point1 << point2 << timediff << veracity << points;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_CORRELATION, data));
}

MessageSP TcpRdsCoder::correlationAll(const QByteArray& data)
{
	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_CORRELATION_ALL, data));
}
