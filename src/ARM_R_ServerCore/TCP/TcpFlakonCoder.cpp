#include "TcpFlakonCoder.h"

TcpFlakonCoder::TcpFlakonCoder(QObject* parent) :
	BaseTcpDeviceCoder(Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(TcpFlakonCoder)), parent)
{
	m_residueLength = 0;
}

TcpFlakonCoder::~TcpFlakonCoder()
{
}

IMessage<QByteArray>* TcpFlakonCoder::encode(const QByteArray& data)
{
	IMessage<QByteArray>* message = NULL;

	m_dataFromTcpSocket.append(data);
	if (m_residueLength == 0) {
		QDataStream stream(m_dataFromTcpSocket);
		stream.setVersion(QDataStream::Qt_4_7);

		stream >> m_header.magic;
		stream >> m_header.number;
		stream >> m_header.id;
		stream >> m_header.flags;
		stream >> m_header.timestamp;
		stream >> m_header.type;
		stream >> m_header.length;
		stream >> m_header.reserved;
		stream >> m_header.messageCRC;
		stream >> m_header.headerCRC;

		m_residueLength = m_header.length;
		m_dataFromTcpSocket.remove(0, PAYLOAD_HEADER_LENGTH);
	}

	if(m_dataFromTcpSocket.length() >= m_residueLength) {
		QByteArray rest_msg = m_dataFromTcpSocket.right(m_dataFromTcpSocket.size() - m_residueLength);
		m_dataFromTcpSocket.truncate(m_residueLength);
		message = messageFromPreparedData();
		m_dataFromTcpSocket.clear();
		m_dataFromTcpSocket = rest_msg;
		m_residueLength = 0;
	}

	return message;
}

QByteArray TcpFlakonCoder::decode(const IMessage<QByteArray>* message)
{
	/// WTF magic??
	ZaviruhaPayloadPacketHeader_redefenition header;

	header.magic = PAYLOAD_PREAMBULE;
	header.number = 0;
	header.id = 0;
	header.flags = 0;
	header.timestamp = 0;
	header.type = -1;

	/// TODO: recheck _header.type = type1;

	QByteArray inputData = message->data();
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

	header.length = inputData.size();
	m_logger->debug(QString("length = %1").arg(header.length));

	CRCs crc;
	header.messageCRC = crc.crc16(reinterpret_cast<unsigned char *>(inputData.data()), inputData.length());
	header.headerCRC = crc.crc8(reinterpret_cast<unsigned char *>(&header), sizeof(ZaviruhaPayloadPacketHeader_redefenition) - sizeof(short));

	QByteArray dataToSend;
	QDataStream stream(&dataToSend, QIODevice::WriteOnly);

	stream << header.magic;
	stream << header.number;
	stream << header.id;
	stream << header.flags;
	stream << header.timestamp;
	stream << header.type;
	stream << header.length;
	stream << header.reserved;
	stream << header.messageCRC;
	stream << header.headerCRC;

	if (message->type() == TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION) {
		int poin;
		inputDataStream >> poin;
		QString s(inputData);
		m_logger->debug(QString("poin %1 %2").arg(QString::number(poin)).arg(s));
		stream << poin;
	}
	else if (message->type() == TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM) {
		int average;
		inputDataStream >> average;
		m_logger->debug(QString("average %1").arg(QString::number(average)));
		stream << average;
	}
	else {
		// For what??
		dataToSend.append(inputData);
	}
	return dataToSend;
}

QObject* TcpFlakonCoder::asQObject()
{
	return this;
}

IMessage<QByteArray>* TcpFlakonCoder::messageFromPreparedData()
{
	QDataStream stream(m_dataFromTcpSocket);
	stream.setVersion(QDataStream::Qt_4_7);

	qreal x,y;
	QVector<QPointF> vec;
	QPointF point;

	quint32 id1 = 0;
	quint32 id2 = 0;

//	QString str = "";

	IMessage<QByteArray>* message = NULL;

	FlakonExternal::TypeFlakonExternalEnum type = (FlakonExternal::TypeFlakonExternalEnum)m_header.type;
	switch(type)
	{
		case FlakonExternal::TypePointsReceivedAnswer:
			for(int i = 0; i < m_header.length; i += sizeof(QPointF)) {
				stream >> x >> y;
				point.setX(x*1000);
				point.setY(y);
				vec.append(point);
			}
			message = pointers(vec);
			break;
		case FlakonExternal::TypeDetectedBandWidthAnswer:
			//2 – обнаруженный сигнал на текущем id пункте (QVector<QPointF>, например (-1.0,5.2) - сигнал с шириной полосы от -1 МГц до 5.2 МГц ),
			for(int i = 0; i < m_header.length; i += sizeof(QPointF)) {
				stream >> point;
				vec.append(point);
//				m_logger->debug(QString("header.id = %1 detected signal = %2 %3").arg(QString::number(m_header.id)).arg(QString::number(point.x())).arg(QString::number(point.y())));
			}
			message = detectedBandwidth(vec);
			break;
		case FlakonExternal::TypeUnknownForUs:
//			QTextStream(stdout) << "Received type" << endl;
//			stream >> str;
//			qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! = " << str;
//			QTextStream(stdout) << str.toLocal8Bit() << endl;
			break;
		case FlakonExternal::TypeCorrelationReceivedAnswer:
			stream >> id1;
			stream >> id2;
			for(int i = 0; i < m_header.length; i += sizeof(QPointF)) {
				stream >> x >> y;
				point.setX(x);
				point.setY(y);
				vec.append(point);
			}
			message = correlation(id1, id2, vec);
			break;
		default:
			break;
	}
	return message;
}

IMessage<QByteArray>* TcpFlakonCoder::pointers(QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_header.id << vec;

	return new Message<QByteArray>(TCP_FLAKON_ANSWER_FFT, ba);
}

IMessage<QByteArray>* TcpFlakonCoder::correlation(quint32 point1, quint32 point2, QVector<QPointF> vec)
{
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << point1 << point2 << vec;

	return new Message<QByteArray>(TCP_FLAKON_ANSWER_CORRELATION, ba);
}

IMessage<QByteArray>* TcpFlakonCoder::detectedBandwidth(QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_header.id << vec;

	return new Message<QByteArray>(TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH, ba);
}
