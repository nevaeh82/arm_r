#include <Logger/Logger.h>

#include "RDSExchange.h"
#include "TcpRdsCoder.h"

#include <QDebug>
#include <QSettings>
#include <QTextCodec>

#define TO_KHZ 1000
#define TIME_DEL 1000

TcpRdsCoder::TcpRdsCoder(unsigned char zone, unsigned char typeRDS, QObject* parent) :
	BaseTcpDeviceCoder(parent),
	m_indexConv1(-1),
	m_indexConv2(-1),
    m_indexSpect(-1)
{

    m_header.zone = zone;
    m_header.typeRds = typeRDS;

	m_residueLength = 0;
	cnt = 0;

	m_locConf.duration = 3;

	m_specTime = QDateTime::currentDateTime();

	m_inputData = spectrum;

    QSettings settings("./ARM_R.ini", QSettings::IniFormat, this);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    m_upTime = settings.value("SpectrumUpTime/time", TIME_DEL).toInt();

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
        RdsProtobuf::Mode* sMsg = msg.mutable_from_client()->mutable_get()->mutable_mode();
        //sMsg->set_index(0);
        sMsg->set_status(true);
	}
	else if(messageType == TCP_RDS_GET_STATUS) {
		msg.mutable_from_client()->mutable_get()->mutable_mode()->set_index(1);
	} else if(messageType == TCP_RDS_GET_LOC_STATUS) {
		createGetLocationStatus(msg);
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
	} else if (message->type() == TCP_PRM300_REQUEST_SET_FREQUENCY) {
		QString name;
		unsigned short avalue;
		stream >> name;
		stream >> avalue;

		m_locConf.centralFreq = avalue;

		RdsProtobuf::Location_LocationOptions* lMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_options();
		lMsg->set_duration(m_locConf.duration);
		lMsg->set_central_frequency(m_locConf.centralFreq);
		lMsg->set_convolution(m_locConf.convolution);
		lMsg->set_doppler(m_locConf.doppler);
		lMsg->set_averaging_frequency_band( m_locConf.freqBand );
		lMsg->set_frequency_tuning_mode(m_locConf.tuningMode);

		lMsg->mutable_filter()->set_range(m_locConf.range);
		lMsg->mutable_filter()->set_shift(m_locConf.shift);
	}
	else if (messageType == TCP_FLAKON_REQUEST_SET_BANDWIDTH) {
		float bandwidth;
		stream >> bandwidth;
		m_locConf.range = bandwidth;
		return QByteArray();
	}
	else if (messageType == TCP_FLAKON_REQUEST_SET_SHIFT) {
		float shift;
		stream >> shift;
		m_locConf.shift = shift;
		return QByteArray();
	}
	else if(messageType == TCP_FLAKON_REQUEST_MAIN_STATION_CORRELATION) {
		QString mainSt;
		stream >> mainSt;
		m_locConf.baseIndex = m_mapPrm.key(mainSt, 0);

		RdsProtobuf::Location_LocationOptions* lMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_options();

		lMsg->set_duration(3);
		lMsg->set_central_frequency(m_locConf.centralFreq);
		lMsg->set_convolution(m_locConf.convolution);
		lMsg->set_doppler(m_locConf.doppler);
		lMsg->set_averaging_frequency_band(m_locConf.freqBand);
		lMsg->set_frequency_tuning_mode(m_locConf.tuningMode);

		lMsg->mutable_filter()->set_range(m_locConf.range);
		lMsg->mutable_filter()->set_shift(m_locConf.shift);

        if(m_coordCounter) {
            m_coordCounter->setShift( m_locConf.shift );
        }
	}
	else if (messageType == TCP_FLAKON_REQUEST_SS_CORRELATION) {
		//NOT usable more
		float frequency;
		bool enable;
		int id;
		stream >> id;
		stream >> frequency;
		stream >> enable;

		RdsProtobuf::Location_LocationOptions* lMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_options();
		lMsg->set_duration(m_locConf.duration);
        m_locConf.centralFreq = frequency;
		lMsg->set_central_frequency(m_locConf.centralFreq);
		m_locConf.convolution = enable;
		lMsg->set_convolution(m_locConf.convolution);
		lMsg->set_doppler(m_locConf.doppler);
		lMsg->set_averaging_frequency_band( m_locConf.freqBand );
		lMsg->set_frequency_tuning_mode(m_locConf.tuningMode);

		lMsg->mutable_filter()->set_range(m_locConf.range);
		lMsg->mutable_filter()->set_shift(m_locConf.shift);

        if(m_coordCounter) {
            m_coordCounter->setShift( m_locConf.shift );
        }
	}
	else if( message->type() == TCP_FLAKON_REQUEST_AVERAGE_SPECTRUM ) {
		int val;

		stream >> val;

		if(m_locConf.freqBand == val) {
			return QByteArray();
		}
		m_locConf.freqBand = val;

		RdsProtobuf::Location_LocationOptions* lMsg = msg.mutable_from_client()->mutable_set()->mutable_location()->mutable_options();
		lMsg->set_duration(m_locConf.duration);
		lMsg->set_central_frequency(m_locConf.centralFreq);
		lMsg->set_convolution(m_locConf.convolution);
		lMsg->set_doppler(m_locConf.doppler);
		lMsg->set_averaging_frequency_band( m_locConf.freqBand );
		lMsg->set_frequency_tuning_mode(m_locConf.tuningMode);

		lMsg->mutable_filter()->set_range(m_locConf.range);
		lMsg->mutable_filter()->set_shift(m_locConf.shift);
	}
	else if( message->type() == TCP_FLAKON_REQUEST_ENABLE_RECEIVER ) {
		int id;
		bool val;

		stream >> id;
		stream >> val;

		RdsProtobuf::System_Receiver* rMsg = msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver();

		rMsg->set_device_index(0);
		rMsg->set_channel_index( id );

		rMsg->set_status( val );
	}
	else if (message->type() == TCP_PRM300_REQUEST_SET_ATTENUER_ONE) {
		stream >> name;
		stream >> val;

		RdsProtobuf::System_Receiver* rMsg = msg.mutable_from_client()->mutable_set()->mutable_system()->mutable_receiver();

		rMsg->set_device_index(0);
		rMsg->set_channel_index(m_mapPrm.key(name));

		rMsg->mutable_settings()->set_frequency(m_locConf.centralFreq);
		rMsg->mutable_settings()->set_attenuator1(val.att1);
		rMsg->mutable_settings()->set_attenuator2(val.att2);

		//dataToSend = prmSetAttenuerOne(int_value);
	}
	else if (message->type() == TCP_PRM300_REQUEST_SET_ATTENUER_TWO) {
	}
	else if (message->type() == TCP_PRM300_REQUEST_SET_FILTER) {
	}
	else if( message->type() == TCP_RDS_SEND_PROTO ) {
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
		if( sMsg.data().has_data_status() ) {
			bool status;
			foreach (bool val, sMsg.data().data_status().status()) {
				status = val;
			}
		}
		if( sMsg.data().has_location_spectrum() ) {        //Spectrum			
			int ind = sMsg.data().location_spectrum().detector_index();

            QDateTime tmp = QDateTime::currentDateTime();
			m_specTime = m_mapSendSpectrumTime.value(ind, tmp);

            if( m_specTime.msecsTo(tmp) < m_upTime ) {

				if(!m_mapSendSpectrumTime.contains(ind)) {
					m_mapSendSpectrumTime.insert(ind, tmp);
				}

				return message;
			}

			m_mapSendSpectrumTime.insert(ind, tmp);

			double startFreq = sMsg.data().location_spectrum().plot().axis_x_start();
			double stepFreq = sMsg.data().location_spectrum().plot().axis_x_step();

			QVector<QPointF> vec;
			double freq = startFreq;

			for(int i = 0; i < sMsg.data().location_spectrum().plot().data().size(); i++) {
				double val = sMsg.data().location_spectrum().plot().data(i);
				vec.append( QPointF(freq*TO_KHZ, val) );
				freq += stepFreq;
			}

				message = pointers(ind, startFreq+10, vec);

                if(m_coordCounter) {
                    m_coordCounter->setCenterFrequency(startFreq+10);
                    m_coordCounter->setShift( m_locConf.shift );
                }

			m_indexSpect = ind;
		} else if( sMsg.data().has_location_convolution() ) {   //Single convolution
			float veracity = 0;
			int index1 =  sMsg.data().location_convolution().convolution().first_detector_index();
			int index2 =  sMsg.data().location_convolution().convolution().second_detector_index();

            QDateTime tmp = QDateTime::currentDateTime();
            int ind = (index1<<8)+index2;
            m_specTime = m_mapSendConvolutionTime.value(ind, tmp);

            if( m_specTime.msecsTo(tmp) < m_upTime ) {

                if(!m_mapSendConvolutionTime.contains(ind)) {
                    m_mapSendConvolutionTime.insert(ind, tmp);
                }

                return message;
            }

            m_mapSendConvolutionTime.insert(ind, tmp);


			float timediff = sMsg.data().location_convolution().convolution().delay();

			QVector<QPointF> vp;
			for(int i = 0; i < sMsg.data().location_convolution().plot().data_size(); i++) {
				double p = sMsg.data().location_convolution().plot().data(i);
				double x = sMsg.data().location_convolution().plot().axis_x_start() +
						   sMsg.data().location_convolution().plot().axis_x_step()*i;

				QPointF point( x, p );
				vp.append(point);
			}

			if(index1 != m_indexConv1 || index2 != m_indexConv2) {
				message = correlation(index1, index2, timediff, veracity, vp);
			}

			m_indexConv1 = index1;
			m_indexConv2 = index2;

		}
		else if( sMsg.data().has_location_data() ) {
			RdsProtobuf::LocationData lMsg = sMsg.data().location_data();

			qint64 dateTime =  lMsg.date_time();
			RdsProtobuf::Signal sigMsg = lMsg.signal();
			int csz = lMsg.convolution_size();

			double start = sigMsg.range().start();
			double end = sigMsg.range().end();
			QString info = QString::fromStdString( sigMsg.info() );

			foreach (RdsProtobuf::Convolution cnvMsg, lMsg.convolution()) {
				int firstInd = cnvMsg.first_detector_index();
				int secInd = cnvMsg.second_detector_index();

				bool stat = cnvMsg.has_delay();
				double delay = cnvMsg.delay();
				double delay_acc = cnvMsg.delay_accuracy();
			}

			message = correlationAll(data);
		}
		else if( sMsg.data().has_detector_spectrum() ) {
			int ind = sMsg.data().detector_spectrum().detector_index();

            QDateTime tmp = QDateTime::currentDateTime();
            m_specTime = m_mapSendDetectorTime.value(ind, tmp);

            if( m_specTime.msecsTo(tmp) < m_upTime ) {

                if(!m_mapSendDetectorTime.contains(ind)) {
                    m_mapSendDetectorTime.insert(ind, tmp);
                }

                return message;
            }

            m_mapSendDetectorTime.insert(ind, tmp);

			int cFreq = sMsg.data().detector_spectrum().central_frequency();

			QVector<QPointF> vec;
			double startFreq = sMsg.data().detector_spectrum().plot().axis_x_start();
			double stepFreq = sMsg.data().detector_spectrum().plot().axis_x_step();

			double freq = startFreq;

			for( int i = 0; i < sMsg.data().detector_spectrum().plot().data_size(); i++ ) {
				double val = sMsg.data().detector_spectrum(). plot().data(i);
				vec.append( QPointF(freq*TO_KHZ, val) );
				freq += stepFreq;
			}

			message = pointers(ind, cFreq, vec);

			QVector<QPointF> pointsList;
			int sz = sMsg.data().detector_spectrum().first_index_size();

			for(int i=0; i < sz; i++ ) {
				int start = sMsg.data().detector_spectrum().first_index(i);
				int end = sMsg.data().detector_spectrum().last_index(i);
				QPointF point( startFreq+stepFreq*start, startFreq+stepFreq*end );

				pointsList.append(point);
			}

            //Test
//            QPointF point( startFreq+stepFreq*10, startFreq+stepFreq*10000 );
//            pointsList.append(point);
            // =========
			emit onDetectSignal(ind, pointsList);
		}
		else if( sMsg.data().has_detector_detected() ) {
			int k = 0;
		}
		else if( sMsg.data().has_analysis_spectrogram() ) {
			int k = 0;
		}
		else if( sMsg.data().has_analysis_spectrum() ) {
			int k = 0;
		}
		else if( sMsg.data().has_analysis_detected() ) {
			int k = 0;
		}
	} else if( sMsg.has_answer() ) {
		QString answer;
		if( sMsg.answer().has_error() ) {
			answer = QString::fromStdString( sMsg.answer().confirmation().str() );
            message = configureLoc(data);
		} else if( sMsg.answer().has_confirmation() ) {
			answer = QString::fromStdString( sMsg.answer().confirmation().str() );
            message = configureLoc(data);
		}
	} else if( sMsg.has_current() ) {
		if( sMsg.current().has_mode() ) {
            int mode = sMsg.current().mode().index();
//			bool stat = sMsg.current().mode().status();
//			int i = 0;
//			i = i+1;
            message = configureLoc(data);
		} else if( sMsg.current().has_system() ) {
			if( sMsg.current().system().has_options() ) {
				QList<StationConfiguration> resultList;

				RdsProtobuf::System_SystemOptions sysMsg = sMsg.current().system().options();
				QString str = QString::fromStdString(sysMsg.title());
				int devNums = sysMsg.devices_size();

				for(int d = 0; d < sysMsg.devices().size(); d++) {
					RdsProtobuf::DeviceOptions optMsg = sysMsg.devices(d);
					QString optStr = QString::fromStdString(optMsg.title());
					bool statDev = optMsg.status();
					QString ipDev = QString::fromStdString(optMsg.ip());

					m_mapDevState.insert(-1, statDev);

					for(int i = 0; i < optMsg.channels_size(); i++) {
						StationConfiguration configVal;
						RdsProtobuf::ChannelOptions chMsg = optMsg.channels(i);

						QString chStr = QString::fromStdString(chMsg.title());

						RdsProtobuf::ReceiverOptions recMsg = chMsg.receiver();
						QString recStr = QString::fromStdString(recMsg.title());

						bool stat = recMsg.status();
						QString ip = QString::fromStdString(recMsg.ip());

						RdsProtobuf::ReceiverSettings prmMsg = recMsg.settings();
						int freq = prmMsg.frequency();
						int att1 = prmMsg.attenuator1();
						int att2 = prmMsg.attenuator2();

						m_mapDevState.insert(i, stat);

						PrmSettings settings;
						settings.att1 = att1;
						settings.att2 = att2;
						settings.ip = ip;

						m_mapPrmSettings.insert(i, settings);

						//configVal.id			= chNum;
						configVal.id			= i;
						configVal.name			= chStr;
						configVal.nameChannel	= chStr;
						configVal.namePrm		= chStr; //recStr;

						m_mapPrm.insert(i, chStr);

						if(chMsg.has_coordinates()) {
							configVal.latitude		= chMsg.coordinates().latitude(); //to be done in proto
							configVal.longitude		= chMsg.coordinates().longitude();
							configVal.altitude		= chMsg.coordinates().altitude();
						} else {
							configVal.latitude		= 0; //to be done in proto
							configVal.longitude		= 0;
							configVal.altitude		= 0;
						}
						configVal.hostPrm300	= ip;
						configVal.statusPrm		= stat;
						configVal.freqPrm		= freq;
						configVal.statusAdc		= statDev;
						configVal.hostADC		= ipDev;

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
			m_locConf.duration = locMsg.duration();
			m_locConf.centralFreq = locMsg.central_frequency();
			m_locConf.convolution = locMsg.convolution();
			m_locConf.doppler = locMsg.doppler();
			m_locConf.freqBand = locMsg.averaging_frequency_band();
			m_locConf.tuningMode = locMsg.frequency_tuning_mode();

			m_locConf.chanNum = m_mapPrmSettings.size();

			m_locConf.range = locMsg.filter().range();
			m_locConf.shift = locMsg.filter().shift();

            if(m_coordCounter) {
                m_coordCounter->setShift( m_locConf.shift );
            }
			message = configureLoc(data);
		}
		else if( sMsg.current().has_detector() ) {
			message = configureLoc(data);
		}
		else if( sMsg.current().has_correction() ) {
			message = configureLoc(data);
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
    dataStream << m_header.zone << m_header.typeRds << index << cf << vec;

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

MessageSP TcpRdsCoder::configureLoc(const QByteArray& data)
{
	return MessageSP(new Message<QByteArray>(TCP_RDS_ANSWER_LOCSYSTEM, data));
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

MessageSP TcpRdsCoder::detectedBandwidth(int index, QVector<QPointF> vec)
{
	// We should send m_header.id to rpcclient
	QByteArray ba;
	QDataStream dataStream(&ba, QIODevice::WriteOnly);
	dataStream << index << vec;

	return MessageSP(new Message<QByteArray>(TCP_FLAKON_ANSWER_DETECTED_BANDWIDTH, ba));
}
