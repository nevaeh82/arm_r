#ifndef PARSERATLANT_H
#define PARSERATLANT_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QTextStream>
#include <QByteArray>
#include <QTime>
#include <QDataStream>

#include "ITCPParser.h"
#include "../Common/IRouter.h"

#include "../Common/ISubscriber.h"
#include "../RPC/Message.h"

#include "../Atlant/EMS/proto/EagleMessageProto.pb.h"
#include "../Atlant/EMS/storm.pb.h"

#include "Rpc/RpcDefines.h"

#define Atlant_Direction_MsgA    "DIRECTION_ANSWER_MESSAGE"
#define Atlant_Position_MsgA     "POSITION_ANSWER_MESSAGE"

//enum MotionType
//{
//    STATIONARY = 0,
//    UNDERWAY = 1,
//    UNCERTAIN = 2,
//    LACKOFDATA = 3
//}

///Parser for atlant messages
class ParserAtlant : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    ParserAtlant(IRouter *router);
    ~ParserAtlant();

public:
    virtual void distruct();
    virtual void parsing(QByteArray data);

private:
    void _completeMsg();
//    void _send_pointers(QVector<QPointF> vec);
//    void _send_correlation(quint32 point1, quint32 point2, QVector<QPointF> vec);
//    void _send_signal_type(int sig_type);
//    QString _get_signal_disc(int signal);

private:
    int                         _type;
    int                         _restLength;
    QByteArray                  _msg;
    QByteArray                  _msg_all;
    IRouter*                    _router;
    ISubscriber*                _subscriber;

    QTime                       _cur_time;

signals:
    void signalParsing(QByteArray);
    void signalFinished();

private slots:
    void _slotParsing(QByteArray);

//signals:

};


#endif // PARSERATLANT_H
