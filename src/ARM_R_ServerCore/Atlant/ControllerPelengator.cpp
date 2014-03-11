#include "ControllerPelengator.h"

ControllerPelengator *ControllerPelengator::_instance;



ControllerPelengator::ControllerPelengator(QString host, unsigned short port, QString name, IRouter* router) :
    _id_requestion(1),
    _id_diap_cur(1)
{
    _router = router;
    _subscriber = _router->get_subscriber();
    _tcp_controller = _router->get_tcp_controller();
}

ControllerPelengator::~ControllerPelengator()
{
}

void ControllerPelengator::setCon(QString host, unsigned short port, QString name)
{
    _host = host;
    _port = port;
    _name = name;
}

/// choose working mode
void ControllerPelengator::slotSetMode(int mode)
{
//    switch(mode)
//    {
//    case 0:
//        slotAdaptation(0, 0);
//        break;
//    case 1:
//        slotDuty(0, 0);
//        break;
//    case 2:
//        break;
//    default:
//        break;
//    }
}

/// adaptation mode
void ControllerPelengator::slotAdaptation(double freq, double brandwidth)
{    
    emsClient.ScanRequest(_id_requestion, freq, brandwidth);
    _id_diap_cur = _id_requestion++;
}

/// duty mode
void ControllerPelengator::slotDuty(double freq, double brandwidth)
{
    emsClient.ScanRequest(_id_requestion, freq, brandwidth);
    _id_diap_cur = _id_requestion++;
}

/// check mode
void ControllerPelengator::slotCheck(QVector<double> freqs, double brandwidth)
{
     emsClient.DirectionFindingRequest(_id_requestion++, freqs.toStdVector(), brandwidth);
     _id_diap_cur = _id_requestion++;
}

/// work mode
void ControllerPelengator::slotWork(QVector<double> freqs, double brandwidth)
{
    emsClient.DirectionFindingRequest(_id_requestion++, freqs.toStdVector(), brandwidth);
    _id_diap_cur = _id_requestion++;
}

/// stop scan
void ControllerPelengator::slotStopScan()
{
    emsClient.ScanStopRequest(_id_diap_cur);
}

/// stop df
void ControllerPelengator::slotStopDF()
{
    emsClient.DFStopRequest(_id_diap_cur);
}

void ControllerPelengator::addLog(QString msg)
{
    emit signalLog(msg);
}

void ControllerPelengator::DirectionAnswerMessageHandler(::Storm::DirectionAnswerMessage& message)
{
    QString msg;
    msg.append("DirectionAnswerMessageHandler received");
    msg.append("requestid = ");
    msg.append(QString::number(message.requestid()));
    msg.append("\n");
    msg.append("sourceid = ");
    msg.append(QString::number(message.sourceid()));
    msg.append("\n");
    msg.append("datetime = ");
    msg.append(QString::number(message.datetime()));
    msg.append("\n");
    msg.append("post = ");
    msg.append(QString::fromStdString(message.post()));
    msg.append("\n");
    msg.append("postlatitude = ");
    msg.append(QString::number(message.postlatitude()));
    msg.append("\n");
    msg.append("postlongitude = ");
    msg.append(QString::number(message.postlongitude()));
    msg.append("\n");
    msg.append("postheight = ");
    msg.append(QString::number(message.postheight()));
    msg.append("\n");
    msg.append("frequency = ");
    msg.append(QString::number(message.frequency()));
    msg.append("\n");
    msg.append("widht = ");
    msg.append(QString::number(message.widht()));
    msg.append("\n");
    msg.append("direction = ");
    msg.append(QString::number(message.direction()));
    msg.append("\n");
    msg.append("angle = ");
    msg.append(QString::number(message.angle()));
    msg.append("\n");
    msg.append("level = ");
    msg.append(QString::number(message.level()));
    msg.append("\n");
    msg.append("quality = ");
    msg.append(QString::number(message.quality()));
    msg.append("\n");
    msg.append("motiontype = ");
    msg.append(message.motiontype());
    msg.append("\n");
    msg.append("motionconfidence = ");
    msg.append(QString::number(message.motionconfidence()));
    msg.append("\n");

    ControllerPelengator::instance()->addLog(msg);

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << message.sourceid();
    ds << message.requestid();
    ds << message.frequency();
    ds << message.widht();
    ds << message.direction();
    ds << message.angle();
    ds << message.level();
    ds << message.quality();

    QSharedPointer<IMessage> msg(new Message(id, ATLANT_DIRECTION, data));
    _subscriber->data_ready(ATLANT_DIRECTION, msg);
}

void ControllerPelengator::PositionAnswerMessageHandler(::Storm::PositionAnswerMessage& message)
{
    qDebug() << "PositionAnswerMessageHandler received";
}
void ControllerPelengator::StatusMessageHandler(::Storm::StatusMessage& message)
{
    qDebug() << "StatusMessageHandler received";
}
