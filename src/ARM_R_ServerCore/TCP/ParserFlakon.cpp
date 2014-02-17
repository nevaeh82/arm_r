#include "ParserFlakon.h"
#include <QDebug>

const char *RecognModulation[] =
{
    "rmUnknown",		//= 0,	//тип не определен
    "rmFM",			//= 1,	//аналоговая ЧМ
    "rmFSK",//			= 2,
    "rmBPSK",//			= 3,
    "rmQPSK",//			= 4,
    "rmPSK8",//			= 5,
    "rmPI_4_DQPSK",//	= 6,	//PI/4 DQPSK
    "rmOQPSK",//			= 7,
    "rmQAM",//			= 8,
    "rmGMSK_0_25",//		= 9,	//GMSK 0,25
    "rmGMSK_0_3",//		= 10,	//GMSK 0,3
    "rmGMSK_0_5",//		= 11,	//GMSK 0,5
    "rmPI_4_CQPSK",//	= 12,	//PI/4 CQPSK
    "rmPI_4_CBPSK",//	= 13,	//PI/4 CBPSK
    "rmDQPSK",//			= 14,	//DQPSK ???
    "rmSDPSK",//			= 15,
    "rmAM",//			= 16,
    "rmQAM16"//			= 17	//QAM16, непонятно чем отличается от QAM ?
};

ParserFlakon::ParserFlakon(IRouter *router)
{
    _router = router;
    _subscriber = router->get_subscriber();
}

ParserFlakon::~ParserFlakon()
{
    QTextStream(stdout) << "Terminated parser" << endl;
}

/// self distroy
void ParserFlakon::distruct()
{
    delete this;
}

void ParserFlakon::parsing(QByteArray data)
{
    _cur_time = QTime::currentTime();
    _msg.append(data);
    if(_restLength == 0)
    {
        QDataStream stream(_msg);
        stream.setVersion(QDataStream::Qt_4_7);

        stream >> _header.magic;
        stream >> _header.number;
        stream >> _header.id;
        stream >> _header.flags;
        stream >> _header.timestamp;
        stream >> _header.type;
        stream >> _header.length;
        stream >> _header.reserved;
        stream >> _header.messageCRC;
        stream >> _header.headerCRC;

        _restLength = _header.length;
        _msg.remove(0, PAYLOAD_HEADER_LENGTH);
    }

    if(_msg.length() >= _restLength)
    {
//        qDebug() << "message length = " << _msg.length();
        QByteArray rest_msg = _msg.right(_msg.size() - _restLength);
        _msg.truncate(_restLength);
        //        _msg = data.right(_restLength);
        _completeMsg();
        _msg.clear();
//        data.remove(0, _restLength);
        _msg = rest_msg;
        _restLength = 0;
    }
    //  _header = reinterpret_cast<ZaviruhaPayloadPacketHeader *>(data.data());
//    qDebug() << "parse data";
//    MessageCom *msg = new MessageCom(_type, data);
//    _queue->add_data(msg);
}

void ParserFlakon::_completeMsg()
{
    QDataStream stream(_msg);
    stream.setVersion(QDataStream::Qt_4_7);

    qreal x,y;
    QVector<QPointF> vec;
    QPointF point;
    int id = 0;
    int id_swap = 0;

    quint32 id1 = 0;
    quint32 id2 = 0;
    bool isComplex = false;

    QString str = "";
    int signal_type = 0;
    switch(_header.type)
    {
    case 1:
        for(int i = 0; i < _header.length; i += sizeof(QPointF))
        {
            stream >> x >> y;// spectr.append(QPointF(x,y)); emit signalSendText(QString("QVector<QPointF> (%1,%2)\n").arg(spectr.at(0).x()).arg(spectr.at(0).y()));
            point.setX(x*1000);
            point.setY(y);
            vec.append(point);

        }

        _send_pointers(vec);

//        QByteArray *ba = new QByteArray();
//        QDataStream ds(ba, QIODevice::ReadWrite);
//        ds << vec;

//        QSharedPointer<IMessage> msg(new Message(_header.id, FLAKON_FFT, ba));
//        _subscriber->data_ready(FLAKON_FFT, msg);

//        isComplex = false;

//        if(_header.id == 1)
//            emit signalFFT(vec,isComplex);
//        if(_header.id == 2)
//            emit signalFFT2(vec, isComplex);
//        if(_header.id == 3)
//            emit signalFFT3(vec, isComplex);
        break;
	case 2:
	//2 – обнаруженный сигнал на текущем id пункте (QVector<QPointF>, например (-1.0,5.2) - сигнал с шириной полосы от -1 МГц до 5.2 МГц ),
//		QVector<QPointF> vec;
//		QPointF point;
		for(int i = 0; i < _header.length; i += sizeof(QPointF))
		{
			stream >> point;
			vec.append(point);
			qDebug() << _header.id << "detected signal = " << point;
		}
		m_sendDetectedBandwidth(vec);
		break;
    case 3:
        QTextStream(stdout) << "Received type" << endl;
        stream >> str;
        qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! = " << str;
        QTextStream(stdout) << str.toLocal8Bit() << endl;
        break;
    case 4:
        stream >> id1;
        stream >> id2;
        for(int i = 0; i < _header.length; i += sizeof(QPointF))
        {
            stream >> x >> y;// spectr.append(QPointF(x,y)); emit signalSendText(QString("QVector<QPointF> (%1,%2)\n").arg(spectr.at(0).x()).arg(spectr.at(0).y()));
            point.setX(x);
            point.setY(y);
            vec.append(point);
        }

        _send_correlation(id1, id2, vec);
        break;
    case 5:
        stream >> signal_type;
        QTextStream(stdout) << "Signal is = " << signal_type << endl;
        _send_signal_type(signal_type);
        break;

    default:
        break;
    }
}

void ParserFlakon::_send_pointers(QVector<QPointF> vec)
{
    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << vec;

    QSharedPointer<IMessage> msg(new Message(_header.id, FLAKON_FFT, ba));
    _subscriber->data_ready(FLAKON_FFT, msg);
}

void ParserFlakon::m_sendDetectedBandwidth(QVector<QPointF> vec)
{
	QByteArray *ba = new QByteArray();
	QDataStream ds(ba, QIODevice::ReadWrite);
	ds << vec;

	QSharedPointer<IMessage> msg(new Message(_header.id, FLAKON_DETECTED_BANDWIDTH, ba));
	_subscriber->data_ready(FLAKON_DETECTED_BANDWIDTH, msg);
}

void ParserFlakon::_send_correlation(quint32 point1, quint32 point2, QVector<QPointF> vec)
{
//    //Для поиска максимума и критерия корреляции
//    double aTempForMax=-9999;
//    double aCurrentDr;
//    double aTempBenchmark;
//    //Для нахождения МО
//    double aSum=0;
//    double aMean;
//    if(_router->get_station_property() == NULL)
//    {
//        return;
//    }
//    double dist1 = _router->get_station_property()->value(point1)->get_distance();
//    double dist2 = _router->get_station_property()->value(point2)->get_distance();
//    for (int p = 0; p < 30000; p++)
//    {
//       vec.replace(p, QPointF(vec.at(p).x() - (-dist2 + dist1), vec.at(p).y()));
//       if (aTempForMax < vec.at(p).y())
//       {
//           aTempForMax = vec.at(p).y();
//           aCurrentDr = vec.at(p).x();
//       }
//       aSum=aSum + vec.at(p).y();
//    }
//    aMean=aSum / 30000;

//    if (aTempForMax < 0.03)
//    {
//        aTempBenchmark = ((aTempForMax - aMean) / (aMean * 10));
//    }
//    else if ((aTempForMax < 0.1) && (aTempForMax > 0.03))
//    {
//        aTempBenchmark = ((aTempForMax - aMean) / (aMean));
//    }
//    else if (aTempForMax >= 0.1)
//    {
//        aTempBenchmark = ((aTempForMax - aMean) / (aMean)) * (aTempForMax * 10);
//    }

//    //Вычисленные по графику корреляции разности и достоверности
//    double aBenchmark = aTempBenchmark;
//    double aDR = aCurrentDr;

////    qDebug() << "aBenchmark" << aBenchmark;
////    qDebug() << "aDR" << aDR;

//    QByteArray *ba = new QByteArray();
//    QDataStream ds(ba, QIODevice::ReadWrite);
//    ds << _cur_time;
//    ds << point1;
//    ds << point2;
//    ds << aDR;
//    ds << aBenchmark;
//    ds << vec;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::WriteOnly);
    ds << point1;
    ds << point2;
    ds << vec;


    QSharedPointer<IMessage> msg(new Message(point1, FLAKON_CORRELATION, ba));
    _subscriber->data_ready(FLAKON_CORRELATION, msg);
}

void ParserFlakon::_send_signal_type(int sig_type)
{
//    QString modulation = _get_signal_disc(sig_type);
//    QByteArray *ba = new QByteArray();
//    QDataStream ds(ba, QIODevice::ReadWrite);
//    ds << modulation;

//    QSharedPointer<IMessage> msg(new Message(_header.id, FLAKON_SIGNAL_TYPE, ba));
//    _subscriber->data_ready(FLAKON_SIGNAL_TYPE, msg);
}

QString ParserFlakon::_get_signal_disc(int signal)
{
    QString str = "";//RecognModulation[signal];
    QTextStream(stdout) << str << endl;
    return str;
}
