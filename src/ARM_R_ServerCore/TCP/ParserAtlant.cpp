#include "ParserAtlant.h"
#include <QDebug>

ParserAtlant::ParserAtlant(IRouter* router):
    _type(0),
    _restLength(0)
{
    _router = router;
    _subscriber = _router->get_subscriber();
    connect(this, SIGNAL(signalParsing(QByteArray)), this, SLOT(_slotParsing(QByteArray)));
}

ParserAtlant::~ParserAtlant()
{
    QTextStream(stdout) << "Terminated parser" << endl;
}

/// self distroy
void ParserAtlant::distruct()
{
    emit signalFinished();
    delete this;
}

void ParserAtlant::parsing(QByteArray data)
{
    emit signalParsing(data);
}


void ParserAtlant::_completeMsg()
{
    EMS::EagleMessage e_msg;
    e_msg.ParseFromArray(_msg.constData(), _msg.size());
    QString type = QString::fromStdString(e_msg.type());
    QByteArray ba = QByteArray::fromRawData(e_msg.innermessage().c_str(), e_msg.innermessage().size());

    if(type == Atlant_Direction_MsgA)
    {
        Storm::DirectionAnswerMessage d_msg;
        d_msg.ParseFromArray(ba.constData(), ba.size());

        A_Dir_Ans_msg ad_struct;
        ad_struct.requestId         = d_msg.requestid();
        ad_struct.sourceId          = d_msg.sourceid();
        ad_struct.dateTime          = d_msg.datetime();
        ad_struct.post              = QString::fromStdString(d_msg.post());
        ad_struct.postLatitude      = d_msg.postlatitude();
        ad_struct.postLongitude     = d_msg.postlongitude();
        ad_struct.postHeight        = d_msg.postheight();
        ad_struct.frequency         = d_msg.frequency();
        ad_struct.widht             = d_msg.widht();
        ad_struct.direction         = d_msg.direction();
        ad_struct.angle             = d_msg.angle();
        ad_struct.level             = d_msg.level();
        ad_struct.quality           = d_msg.quality();
        ad_struct.motionType        = d_msg.motiontype();
        ad_struct.motionConfidence  = d_msg.motionconfidence();

        QByteArray *ba = new QByteArray();
        QDataStream ds(ba, QIODevice::ReadWrite);
        ds << ad_struct.requestId;
        ds << ad_struct.sourceId;
        ds << ad_struct.dateTime;
        ds << ad_struct.post;
        ds << ad_struct.postLatitude;
        ds << ad_struct.postLongitude;
        ds << ad_struct.postHeight;
        ds << ad_struct.frequency;
        ds << ad_struct.widht;
        ds << ad_struct.direction;
        ds << ad_struct.angle;
        ds << ad_struct.level;
        ds << ad_struct.quality;
        ds << ad_struct.motionType;
        ds << ad_struct.motionConfidence;

        ///costil in id of message
        QSharedPointer<IMessageOld> msg(new MessageOld(6, ATLANT_DIRECTION, ba));
        _subscriber->data_ready(ATLANT_DIRECTION, msg);
    }
    else if (type == Atlant_Position_MsgA)
    {
        Storm::PositionAnswerMessage d_msg;
        d_msg.ParseFromArray(ba.constData(), ba.size());

        A_Pos_Ans_msg ad_struct;
        ad_struct.requestId         = d_msg.requestid();
        ad_struct.sourceId          = d_msg.sourceid();
        ad_struct.dateTime          = d_msg.datetime();
        ad_struct.longitude         = d_msg.longitude();
        ad_struct.latitude          = d_msg.latitude();
        ad_struct.quality           = d_msg.quality();


        QByteArray *ba = new QByteArray();
        QDataStream ds(ba, QIODevice::ReadWrite);
        ds << ad_struct.requestId;
        ds << ad_struct.sourceId;
        ds << ad_struct.dateTime;
        ds << ad_struct.longitude;
        ds << ad_struct.latitude;
        ds << ad_struct.quality;

        ///costil in id of message
        QSharedPointer<IMessageOld> msg(new MessageOld(6, ATLANT_POSITION, ba));
        _subscriber->data_ready(ATLANT_POSITION, msg);
    }

}

void ParserAtlant::_slotParsing(QByteArray data)
{
    _cur_time = QTime::currentTime();
    _msg.append(data);
    if(_restLength == 0)
    {
//        qDebug() << "FIRST = " << (int)_msg.at(0);
        QByteArray len = _msg.left(sizeof(int));
        memcpy(&_restLength, len.data(), 4);
//        qDebug() << "rest = " << _restLength;
//        if(_restLength != 162)
//        {
//            qDebug() << "jjj";
////            _msg.clear();
////            return;
//        }
//        _restLength = len.toUInt();

        _msg.remove(0, sizeof(int));
    }

    if(_msg.length() >= _restLength)
    {
        QByteArray rest_msg = _msg.right(_msg.size() - _restLength);
        _msg.truncate(_restLength);
        //        _msg = data.right(_restLength);
        _completeMsg();
        _msg.clear();
//        data.remove(0, _restLength);
        _msg = rest_msg;
        _restLength = 0;
    }

}
