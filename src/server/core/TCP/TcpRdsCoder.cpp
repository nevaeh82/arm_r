#include <Logger/Logger.h>

#include "TcpRdsCoder.h"

#include "RdsPacket.pb.h"

#include <QDebug>

#define TO_KHZ 1000

TcpRdsCoder::TcpRdsCoder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_residueLength = 0;
	cnt = 0;
}

TcpRdsCoder::~TcpRdsCoder()
{
}

MessageSP TcpRdsCoder::encode(const QByteArray& data)
{
	MessageSP message;

	QByteArray inData = getMessage(data);

	if(inData.isEmpty()) {
		return message;
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
		QByteArray data = m_dataFromTcpSocket.right( m_residueLength );
		dataToSend = data;

		m_dataFromTcpSocket = m_dataFromTcpSocket.right(m_dataFromTcpSocket.length() - packetLen);
		m_residueLength = 0;
	}

	return dataToSend;
}

QByteArray TcpRdsCoder::decode(const MessageSP message)
{
	QString messageType = message->type();
	QByteArray dataToSend;
	RdsProtobuf::Packet msg;

	if (messageType == TCP_RDS_SET_STATUS) {
		msg.mutable_from_client()->mutable_set()->mutable_mode()->set_index(1);
	} else if(messageType == TCP_RDS_TURN_STATUS) {
		msg.mutable_from_client()->mutable_set()->mutable_mode()->set_status(true);
	} else if(messageType == TCP_RDS_GET_STATUS) {
		msg.mutable_from_client()->mutable_get()->mutable_mode()->set_index(1);
	} else if(messageType == TCP_RDS_GET_SYSTEM) {
		RdsProtobuf::System_SystemOptions* sMsg = msg.mutable_from_client()->mutable_get()->mutable_system()->mutable_options();
		sMsg->set_title("111", 3);
		sMsg->set_devices_num(1);
		sMsg->set_separate_receivers_num(1);

		RdsProtobuf::DeviceOptions* devMsg = sMsg->add_devices();
		devMsg->set_title("111");
		devMsg->set_status(true);
		devMsg->set_ip("111");
		devMsg->set_porthttp(0);
		devMsg->set_portsctp(0);
		devMsg->set_channelsnum(0);
		devMsg->set_sync(0);
		devMsg->set_board(0);

//		sMsg->mutable_devices(0)->is

		RdsProtobuf::ReceiverOptions* recMsg = sMsg->add_separate_receivers();
		recMsg->set_title("111");
		recMsg->set_status(true);
		recMsg->set_ip("111");
		recMsg->set_port(0);
		recMsg->set_type(0);

		recMsg->mutable_settingsprm300()->set_freq(0);
		recMsg->mutable_settingsprm300()->set_filter(0);
		recMsg->mutable_settingsprm300()->set_attenuator1(0);
		recMsg->mutable_settingsprm300()->set_attenuator2(0);
		recMsg->mutable_settingsprm300()->set_channum(0);
		recMsg->mutable_settingsprm300()->set_generator(0);
	} else {
		return QByteArray();
	}

	int size = msg.ByteSize();
	dataToSend.resize(size);
	msg.SerializeToArray(dataToSend.data(), dataToSend.size());

	addPreambula(dataToSend);

	return dataToSend;
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

MessageSP TcpRdsCoder::messageFromPreparedData(const QByteArray& data)
{
	MessageSP message;

	RdsProtobuf::Packet msg;
	msg.ParseFromArray(data.data(), data.size());

	if( !msg.has_from_server() ) {
		return message;
	}

	RdsProtobuf::ServerMessage sMsg = msg.from_server();

	if( sMsg.has_data() ) {
		if( sMsg.data().has_location_spectrum() ) {
			int ind = sMsg.data().location_spectrum().index();
			int len = sMsg.data().location_spectrum().length();
			int range = sMsg.data().location_spectrum().signalrange();

			double startFreq = sMsg.data().location_spectrum().startfreq();
			double stepFreq = sMsg.data().location_spectrum().stepfreq();
			int shiftFreq = sMsg.data().location_spectrum().freqshift();

			QVector<QPointF> vec;
			double freq = startFreq;

			for(int i = 0; i < sMsg.data().location_spectrum().data().size(); i++) {
				double val = sMsg.data().location_spectrum().data().Get(i);
				vec.append( QPointF(freq*TO_KHZ, val) );
				freq += stepFreq;
			}

			message = pointers(ind, vec);
		}
	} else if( sMsg.has_answer() ) {
		QString answer;
		if( sMsg.answer().has_error() ) {
			answer = QString::fromStdString( sMsg.answer().confirmation().str() );
		} else if( sMsg.answer().has_confirmation() ) {
			answer = QString::fromStdString( sMsg.answer().confirmation().str() );
		}
	} else if( sMsg.has_current() ) {
		if( sMsg.current().has_mode() ) {
			uint mode = sMsg.current().mode().index();
			bool stat = sMsg.current().mode().status();
			int i = 0;
			i = i+1;
		} else if( sMsg.current().has_system() ) {
			if( sMsg.current().system().has_options() ) {
				RdsProtobuf::System_SystemOptions sysMsg = sMsg.current().system().options();
				QString str = QString::fromStdString(sysMsg.title());
				int devNums = sysMsg.devices_num();
				int sDevNums = sysMsg.separate_receivers_num();


				for(int d = 0; d < sysMsg.devices().size(); d++) {
					RdsProtobuf::DeviceOptions optMsg = sysMsg.devices(d);
					QString optStr = QString::fromStdString(optMsg.title());
					bool stat = optMsg.status();
					QString ipStr = QString::fromStdString(optMsg.ip());
					for(int i = 0; i < optMsg.channels().size(); i++) {
						RdsProtobuf::ChannelOptions chMsg = optMsg.channels(i);
						QString str = QString::fromStdString(chMsg.title());
						bool inv = chMsg.inversion();
						RdsProtobuf::ReceiverOptions recMsg = chMsg.receiver();
						QString recStr = QString::fromStdString(recMsg.title());
						bool stat = recMsg.status();
						QString ip = QString::fromStdString(recMsg.ip());
						int port = recMsg.port();
						int type = recMsg.type();
						RdsProtobuf::Prm300 prmMsg = recMsg.settingsprm300();
						int freq = prmMsg.freq();
						int filter = prmMsg.filter();
						int att1 = prmMsg.attenuator1();
						int att2 = prmMsg.attenuator2();
						int chNum = prmMsg.channum();
						int gen = prmMsg.generator();
					}
				}
			}
		}
	}

	//message = pointers(vec);

	return message;
}

MessageSP TcpRdsCoder::pointers(int index, QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << index << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_FFT, ba));
}

MessageSP TcpRdsCoder::correlation(quint32 point1, quint32 point2, QVector<QPointF> points)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << point1 << point2 << points;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_CORRELATION, data));
}

MessageSP TcpRdsCoder::detectedBandwidth(QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << m_header.id << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH, ba));
}
