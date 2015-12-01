#include <Logger/Logger.h>

#include "TcpFlakonCoder.h"

#include <QDebug>

TcpFlakonCoder::TcpFlakonCoder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_residueLength = 0;
	cnt = 0;
}

TcpFlakonCoder::~TcpFlakonCoder()
{
}

MessageSP TcpFlakonCoder::encode(const QByteArray& data)
{
	MessageSP message;
	m_dataFromTcpSocket.append(data);
	if (m_residueLength == 0) {

		if(m_dataFromTcpSocket.length() < PAYLOAD_HEADER_LENGTH) {
			return message;
		}

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

		if(m_header.magic != 0xFFEECCFF) {

//			for(int i = 0; i<m_dataFromTcpSocket.length(); i++) {
//				if( m_dataFromTcpSocket.at(i) == 0xFF &&  m_dataFromTcpSocket.at(i))
//			}

			QByteArray tst;
			tst.append(0xFF);
			tst.append(0xEE);
			tst.append(0xCC);
			tst.append(0xFF);

			bool res = false;
			uint ind = 0;
			for(int i = 0; i<m_dataFromTcpSocket.length() - 3; i++) {
				uchar a1 = m_dataFromTcpSocket.at(i);
				uchar a2 = m_dataFromTcpSocket.at(i+1);
				uchar a3 = m_dataFromTcpSocket.at(i+2);
				uchar a4 = m_dataFromTcpSocket.at(i+3);

				if( m_dataFromTcpSocket.at(i) == tst.at(0) &&
					m_dataFromTcpSocket.at(i+1) == tst.at(1) &&
					m_dataFromTcpSocket.at(i+2) == tst.at(2) &&
					m_dataFromTcpSocket.at(i+3) == tst.at(3)) {
					ind = i;
					res = true;
				}
			}

			if( !res ) {
				m_dataFromTcpSocket = m_dataFromTcpSocket.right( 10 );
				m_residueLength = 0;
				//log_debug("err_no");
				return message;
			} else {
				m_dataFromTcpSocket = m_dataFromTcpSocket.right( m_dataFromTcpSocket.length() - ind );
				m_residueLength = 0;
				log_debug("err_Ok");
				return message;
			}
		} else {
			bool b;
			b = true;
		}

		m_dataFromTcpSocket.remove(0, PAYLOAD_HEADER_LENGTH);
	}

	//m_dataFromTcpSocket.append( QByteArray(m_residueLength, 'a') );

	float t1 = m_residueLength;
	float t2 = m_dataFromTcpSocket.length();

	float res = (t2 / t1) * 100;

//	log_debug(QString("Socket size:   %1").arg(m_dataFromTcpSocket.length()));
//	log_debug(QString("All size:   %1").arg(m_residueLength));

	cnt++;

	if(cnt > 100) {
		log_debug(QString("Receiving data   %1%").arg(res));
		cnt = 0;
	}

	if(m_dataFromTcpSocket.length() >= m_residueLength) {
		//log_debug(QString("Receiving data   %1% !!!!! all").arg(res));
		QByteArray rest_msg = m_dataFromTcpSocket.right(m_dataFromTcpSocket.size() - m_residueLength);
		m_dataFromTcpSocket.truncate(m_residueLength);


		message = messageFromPreparedData();
		m_dataFromTcpSocket.clear();
		m_dataFromTcpSocket = rest_msg;

		m_residueLength = 0;
	}

	return message;
}

QByteArray TcpFlakonCoder::decode(const MessageSP message)
{
	/// WTF magic??
	ZaviruhaPayloadPacketHeader header;

	header.magic = PAYLOAD_PREAMBULE;
	header.number = 0;
	header.id = 0;
	header.flags = 0;
	header.timestamp = 0;
	header.type = -1;

	QString messageType = message->type();

	if (messageType == TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION) {
		header.type = (quint32)FlakonExternal::TypeSetMainStationCorrelationRequest;
	}
	else if (messageType == TCP_FLAKON_REQUEST_SET_BANDWIDTH) {
		header.type = (quint32)FlakonExternal::TypeSetBandWidthRequest;
	}
	else if (messageType == TCP_FLAKON_REQUEST_SET_SHIFT) {
		header.type = (quint32)FlakonExternal::TypeSetShiftRequest;
	}
	else if (messageType == TCP_FLAKON_REQUEST_SS_CORRELATION) {
		header.type = (quint32)FlakonExternal::TypeStartCorrelationRequest;
	}
	else if (messageType == TCP_FLAKON_REQUEST_RECOGNIZE) {
		header.type = (quint32)FlakonExternal::TypeRecognizeSignalRequest;
	}
	else if (messageType == TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM) {
		header.type = (quint32)FlakonExternal::TypeSetSpectrumAverageParametrRequest;
	}

	/// TODO: recheck _header.type = type1;

	QByteArray inputData = message->data();
	QDataStream inputDataStream(&inputData, QIODevice::ReadOnly);

	header.length = inputData.size();
	log_debug(QString("length = %1").arg(header.length));

	CRCs crc;
	header.messageCRC = crc.crc16(reinterpret_cast<unsigned char *>(inputData.data()), inputData.length());
	header.headerCRC = crc.crc8(reinterpret_cast<unsigned char *>(&header), sizeof(ZaviruhaPayloadPacketHeader) - sizeof(short));

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
		int point;
		inputDataStream >> point;
		QString s(inputData);
		log_debug(QString("poin %1 %2").arg(QString::number(point)).arg(s));
		stream << point;
	}
	else if (message->type() == TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM) {
		int average;
		inputDataStream >> average;
		log_debug(QString("average %1").arg(QString::number(average)));
		stream << average;
	}
	else if (message->type() == TCP_FLAKON_REQUEST_SS_CORRELATION) {
		bool enable;
		inputDataStream >> enable;
		stream << enable;
	}
	else{
		// For what??
		dataToSend.append(inputData);
	}
	return dataToSend;
}

QObject* TcpFlakonCoder::asQObject()
{
	return this;
}

MessageSP TcpFlakonCoder::messageFromPreparedData()
{
	QDataStream stream(m_dataFromTcpSocket);
	stream.setVersion(QDataStream::Qt_4_7);

	qreal x,y;
	QVector<QPointF> vec;
	QPointF point;

	quint32 id1 = 0;
	quint32 id2 = 0;
	QString stringFromServer;

	/// TODO: reckeck
	MessageSP message;

	FlakonExternal::TypeFlakonExternalEnum type = (FlakonExternal::TypeFlakonExternalEnum)m_header.type;
	switch(type)
	{
		case FlakonExternal::TypePointsReceivedAnswer:
			for(quint32 i = 0; i < m_header.length; i += sizeof(QPointF)) {
				stream >> x >> y;
				point.setX(x*1000);
				point.setY(y);
				vec.append(point);
			}
			message = pointers(vec);
			break;
		case FlakonExternal::TypeDetectedBandWidthAnswer:
			//2 – обнаруженный сигнал на текущем id пункте (QVector<QPointF>, например (-1.0,5.2) - сигнал с шириной полосы от -1 МГц до 5.2 МГц ),
			for(quint32 i = 0; i < m_header.length; i += sizeof(QPointF)) {
				stream >> point;
				vec.append(point);
//				log_debug(QString("header.id = %1 detected signal = %2 %3").arg(QString::number(m_header.id)).arg(QString::number(point.x())).arg(QString::number(point.y())));
			}
			message = detectedBandwidth(vec);
			break;
		case FlakonExternal::TypeStringAnswer:
			stream >> stringFromServer;
//			log_info(QString("Flakon: %1").arg(stringFromServer));
			break;
		case FlakonExternal::TypeCorrelationReceivedAnswer:
			stream >> id1;
			stream >> id2;
			for(quint32 i = 0; i < m_header.length; i += sizeof(QPointF)) {
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

MessageSP TcpFlakonCoder::pointers(QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_header.id << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_FFT, ba));
}

MessageSP TcpFlakonCoder::correlation(quint32 point1, quint32 point2, QVector<QPointF> points)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << point1 << point2 << points;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_CORRELATION, data));
}

MessageSP TcpFlakonCoder::detectedBandwidth(QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_header.id << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH, ba));
}
