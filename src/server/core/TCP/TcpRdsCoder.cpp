#include <Logger/Logger.h>

#include "TcpRdsCoder.h"

#include "RdsPacket.pb.h"

#include <QDebug>

#define TO_KHZ 1000
#define TIME_DEL 0

TcpRdsCoder::TcpRdsCoder(QObject* parent) :
	BaseTcpDeviceCoder(parent)
{
	m_residueLength = 0;
	cnt = 0;

	m_locConf.duration = 3;

	m_specTime = QDateTime::currentDateTime();
	m_corTime = QDateTime::currentDateTime();
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

QByteArray TcpRdsCoder::decode(const MessageSP message)
{
	QString messageType = message->type();
	QByteArray dataToSend;

	Prm val;
	QString name;

	QByteArray msgData = message->data();
	QDataStream stream(&msgData, QIODevice::ReadOnly);

	RdsProtobuf::Packet msg;

	if (messageType == TCP_RDS_SET_STATUS) {
		int ind;
		stream >> ind;
		msg.mutable_from_client()->mutable_set()->mutable_mode()->set_index(ind);
	} else if(messageType == TCP_RDS_TURN_STATUS) {
		bool b;
		stream >> b;
		msg.mutable_from_client()->mutable_set()->mutable_mode()->set_status(b);
	}
	else if(messageType == TCP_RDS_WORK_MODE) {
	}
	else if(messageType == TCP_RDS_GET_STATUS) {
		msg.mutable_from_client()->mutable_get()->mutable_mode()->set_index(1);
	} else if(messageType == TCP_RDS_GET_LOC_STATUS) {
		RdsProtobuf::Location_LocationOptions* oMsg = msg.mutable_from_client()->mutable_get()->mutable_location()->mutable_options();
		oMsg->set_baseindex(0);
		oMsg->set_centralfreq(0);
		oMsg->set_channum(0);
		oMsg->set_convolution(0);
		oMsg->set_duration(0);
		oMsg->set_mode(0);
		oMsg->set_spectrummean(0);
		oMsg->set_tuningmode(0);
	} else if( messageType == TCP_RDS_GET_PRM_STATUS ) {
		RdsProtobuf::System_Device* dMsg = msg.mutable_from_client()->mutable_get()->mutable_system()->mutable_device();

		dMsg->set_device_index(0);
		dMsg->set_status(true);
	} else if( messageType == TCP_RDS_SET_PRM_STATUS ) {
		int ind;
		stream >> ind;

		RdsProtobuf::System_Device* dMsg = msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_device();

		if(ind < 0) {
			dMsg->set_device_index(0);
			dMsg->set_status(true);
		} else {
			dMsg->set_device_index(ind);
			dMsg->set_status(true);
		}
	}
	 else if( messageType == TCP_RDS_GET_SYSTEM ) {
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
	} else if (message->type() == TCP_PRM300_REQUEST_SET_FREQUENCY) {
		QString name;
		unsigned short avalue;
		stream >> name;
		stream >> avalue;

//		RdsProtobuf::System_Receiver* rMsg = msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver();

//		rMsg->set_device_index(0);
//		rMsg->set_channel_index(name.toInt());

//		RdsProtobuf::Prm300 pMsg = RdsProtobuf::Prm300();
//		rMsg->mutable_settingsprm300()->set_freq(avalue);
//		rMsg->mutable_settingsprm300()->set_attenuator1(0);
//		rMsg->mutable_settingsprm300()->set_attenuator2(0);
//		rMsg->mutable_settingsprm300()->set_channum(0);
//		rMsg->mutable_settingsprm300()->set_generator(0);
//		rMsg->mutable_settingsprm300()->set_filter(0);

		m_locConf.centralFreq = avalue;

		RdsProtobuf::Location_LocationOptions* lMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_options();
		lMsg->set_baseindex(m_locConf.baseIndex);
		lMsg->set_centralfreq(m_locConf.centralFreq);
		lMsg->set_channum(m_locConf.chanNum);
		lMsg->set_convolution(m_locConf.convolution);
		lMsg->set_duration(3);
		lMsg->set_mode(m_locConf.mode);
		lMsg->set_spectrummean(m_locConf.spectrumMean);
		lMsg->set_tuningmode(m_locConf.tuningMode);
	}
	else if (messageType == TCP_FLAKON_REQUEST_SET_BANDWIDTH) {
		float bandwidth;
		stream >> bandwidth;
		m_bandwidth = bandwidth;
		return QByteArray();
	}
	else if (messageType == TCP_FLAKON_REQUEST_SET_SHIFT) {
		float shift;
		stream >> shift;
		m_shift = shift;
		return QByteArray();
	}
	else if(messageType == TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION) {
		QString mainSt;
		stream >> mainSt;
		m_locConf.baseIndex = m_mapPrm.key(mainSt, 0);

		RdsProtobuf::Location_LocationOptions* lMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_options();
		lMsg->set_baseindex(m_locConf.baseIndex);
		lMsg->set_centralfreq(m_locConf.centralFreq);
		lMsg->set_channum(m_locConf.chanNum);
		lMsg->set_convolution(m_locConf.convolution);
		lMsg->set_duration(3);
		lMsg->set_mode(m_locConf.mode);
		lMsg->set_spectrummean(m_locConf.spectrumMean);
		lMsg->set_tuningmode(m_locConf.tuningMode);

		//return QByteArray();
	}
	else if (messageType == TCP_FLAKON_REQUEST_SS_CORRELATION) {
		float frequency;
		bool enable;
		int id;
		stream >> id;
		stream >> frequency;
		stream >> enable;

		RdsProtobuf::Location_FilterOptions* fMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_filter();
		fMsg->set_range(m_bandwidth);
		fMsg->set_shift(m_shift);
	}
	else if (message->type() == TCP_PRM300_REQUEST_SET_ATTENUER_ONE) {
		stream >> name;
		stream >> val;

		RdsProtobuf::System_Receiver* rMsg = msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver();

		rMsg->set_device_index(0);
		rMsg->set_channel_index(m_mapPrm.key(name));

		RdsProtobuf::Prm300 pMsg = RdsProtobuf::Prm300();
		rMsg->mutable_settingsprm300()->set_freq(m_freq);
		rMsg->mutable_settingsprm300()->set_attenuator1(val.att1);
		rMsg->mutable_settingsprm300()->set_attenuator2(val.att2);
		rMsg->mutable_settingsprm300()->set_channum(0);
		rMsg->mutable_settingsprm300()->set_generator(0);
		rMsg->mutable_settingsprm300()->set_filter(val.filter);

		//dataToSend = prmSetAttenuerOne(int_value);
	}
	else if (message->type() == TCP_PRM300_REQUEST_SET_ATTENUER_TWO) {
		int val;
		QString name;
		stream >> name;
		stream >> val;

		m_test = val;
		m_sTest = name;
		//dataToSend = prmSetAttenuerTwo(int_value);
	}
	else if (message->type() == TCP_PRM300_REQUEST_SET_FILTER) {
		int val;
		QString name;
		stream >> name;
		stream >> val;
//		/dataToSend = prmSetFilter(int_value);
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
			QDateTime tmp = QDateTime::currentDateTime();
			if( m_specTime.msecsTo(tmp) < TIME_DEL ) {
				return message;
			}

			m_specTime = QDateTime::currentDateTime();

			int ind = sMsg.data().location_spectrum().index();
			int len = sMsg.data().location_spectrum().length();
			int range = sMsg.data().location_spectrum().signalrange();

				log_debug(QString("Spectrum ID: %1").arg(ind));

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

			message = pointers(ind, startFreq+10, vec);
		} else if( sMsg.data().has_location_xcov() ) {
			QDateTime tmp = QDateTime::currentDateTime();
			if( m_corTime.msecsTo(tmp) < TIME_DEL ) {
				return message;
			}

			m_corTime = QDateTime::currentDateTime();

			int index = sMsg.data().location_xcov().index();
			float timediff = sMsg.data().location_xcov().timediff();
			float veracity = sMsg.data().location_xcov().veracity();

			QVector<QPointF> vp;
			for(int i = 0; i < sMsg.data().location_xcov().data().size(); i++) {
				float p = sMsg.data().location_xcov().data(i);
				QPointF point(i, p);
				vp.append(point);
			}

			message = correlation(m_locConf.baseIndex, index, timediff, veracity, vp);
		} else if( sMsg.data().has_detector_spectrum() ) {

			QDateTime tmp = QDateTime::currentDateTime();
			if( m_specTime.msecsTo(tmp) < TIME_DEL ) {
				return message;
			}

			m_specTime = QDateTime::currentDateTime();


			int ind = sMsg.data().detector_spectrum().index();
			int len = sMsg.data().detector_spectrum().length();
			int range = sMsg.data().detector_spectrum().signalrange();

				log_debug(QString("Spectrum ID: %1").arg(ind));

			double startFreq = sMsg.data().detector_spectrum().startfreq();
			double stepFreq = sMsg.data().detector_spectrum().stepfreq();
			int shiftFreq = sMsg.data().detector_spectrum().freqshift();

			QVector<QPointF> vec;
			double freq = startFreq;

			for(int i = 0; i < sMsg.data().detector_spectrum().data().size(); i++) {
				double val = sMsg.data().detector_spectrum().data().Get(i);
				vec.append( QPointF(freq*TO_KHZ, val) );
				freq += stepFreq;
			}

			message = pointers(ind, startFreq+10, vec);

			QVector<QPointF> pointsList;
			int sz = sMsg.data().detector_spectrum().firstindex_size();

			for(int i=0; i < sMsg.data().detector_spectrum().firstindex_size(); i++ ) {
				int start = sMsg.data().detector_spectrum().firstindex(i);
				int end = sMsg.data().detector_spectrum().lastindex(i);
				QPointF point( startFreq+stepFreq*start, startFreq+stepFreq*end );

				pointsList.append(point);
			}

			emit onDetectSignal(ind, pointsList);
			//message = detectedBandwidth(ind, pointsList);
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
				QList<StationConfiguration> resultList;

				RdsProtobuf::System_SystemOptions sysMsg = sMsg.current().system().options();
				QString str = QString::fromStdString(sysMsg.title());
				int devNums = sysMsg.devices_num();
				int sDevNums = sysMsg.separate_receivers_num();

				for(int d = 0; d < sysMsg.devices().size(); d++) {
					RdsProtobuf::DeviceOptions optMsg = sysMsg.devices(d);
					QString optStr = QString::fromStdString(optMsg.title());
					bool statDev = optMsg.status();
					QString ipDev = QString::fromStdString(optMsg.ip());
					quint32 PortDev = optMsg.porthttp();

					m_mapDevState.insert(-1, statDev);

					for(int i = 0; i < optMsg.channels().size(); i++) {
						StationConfiguration configVal;
						RdsProtobuf::ChannelOptions chMsg = optMsg.channels(i);
						QString chStr = QString::fromStdString(chMsg.title());
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

						m_mapDevState.insert(i, stat);

						PrmSettings settings;
						settings.att1 = att1;
						settings.att2 = att2;
						settings.filter = filter;
						settings.gen = gen;
						m_mapPrmSettings.insert(chNum, settings);

						//configVal.id			= chNum;
						configVal.id			= i;
						configVal.name			= chStr;
						configVal.nameChannel	= chStr;
						configVal.namePrm		= chStr; //recStr;

						m_mapPrm.insert(i, chStr);

						configVal.latitude		= 0; //to be done in proto
						configVal.longitude		= 0;
						configVal.hostPrm300	= ip;
						configVal.portPrm		= port;
						configVal.typePrm		= type;
						configVal.inversionPrm	= inv;
						configVal.statusPrm		= stat;
						configVal.freqPrm = freq;
						configVal.statusAdc		= statDev;
						configVal.hostADC		= ipDev;
						configVal.portADC		= PortDev;

						resultList.append(configVal);
					}
				}
				message = configure(resultList);
				emit onChangeDevState(m_mapDevState);

			} else if( sMsg.current().system().has_device()) {
				RdsProtobuf::System_Device dMsg = sMsg.current().system().device();
				m_mapDevState.insert(-1, dMsg.status());
				emit onChangeDevState(m_mapDevState);
			} else if( sMsg.current().system().has_receiver()) {
				RdsProtobuf::System_Receiver rMsg = sMsg.current().system().receiver();
				if(rMsg.has_status()) {
					m_mapDevState.insert( rMsg.channel_index(), rMsg.status() );
					emit onChangeDevState(m_mapDevState);
				}
			}
		} else if( sMsg.current().has_location() ) {
			RdsProtobuf::Location_LocationOptions locMsg= sMsg.current().location().options();
			m_locConf.chanNum = locMsg.channum();
			m_locConf.mode = locMsg.mode();
			m_locConf.baseIndex = locMsg.baseindex();
			m_locConf.convolution = locMsg.convolution();
			m_locConf.spectrumMean = locMsg.spectrummean();
			m_locConf.tuningMode = locMsg.tuningmode();
			m_locConf.duration = locMsg.duration();
			m_locConf.centralFreq = locMsg.centralfreq();

			message = configureLoc(m_locConf);
		}
	}

	//message = pointers(vec);

	return message;
}

MessageSP TcpRdsCoder::pointers(int index, float cf, QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << index << cf << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_FFT, ba));
}

MessageSP TcpRdsCoder::configure(const QList<StationConfiguration>& lst)
{
	if(lst.isEmpty()) {
		return MessageSP();
	}

	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << lst;

	return MessageSP(new Message<QByteArray>(TCP_RDS_ANSWER_SYSTEM, ba));
}

MessageSP TcpRdsCoder::configureLoc(const LocSystemConfiguration& conf)
{

	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << conf;

	return MessageSP(new Message<QByteArray>(TCP_RDS_ANSWER_LOCSYSTEM, ba));
}

MessageSP TcpRdsCoder::correlation(quint32 point1, quint32 point2, float timediff, float veracity, QVector<QPointF> points)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << point1 << point2 << timediff << veracity << points;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_CORRELATION, data));
}

MessageSP TcpRdsCoder::detectedBandwidth(int index, QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << index << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH, ba));
}
