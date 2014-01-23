#ifndef PARSERFLAKON_H
#define PARSERFLAKON_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QPointF>

#include <QTime>
#include "../Common/CRCs.h"
#include "ITCPParser.h"
#include "../Common/IRouter.h"
#include "TCPPacketStruct.h"

#include "../Common/ISubscriber.h"
#include "../RPC/Message.h"

//#include "../Common/MessageCom.h"

#include "../Flakon/StationProperty.h"



///Parser for flakon messages
class ParserFlakon : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    ParserFlakon(IRouter *router);
    ~ParserFlakon();

public:
    virtual void distruct();
    virtual void parsing(QByteArray data);

private:
    void _completeMsg();
    void _send_pointers(QVector<QPointF> vec);
    void _send_correlation(quint32 point1, quint32 point2, QVector<QPointF> vec);
    void _send_signal_type(int sig_type);
    QString _get_signal_disc(int signal);

private:
    int                         _type;
    ZaviruhaPayloadPacketHeader _header;
    int                         _restLength;
    QByteArray                  _msg;
    QByteArray                  _msg_all;
    IRouter*                    _router;
    ISubscriber*                _subscriber;
    QTime                       _cur_time;

    double                      _corr_threshold;

signals:
    void signalFFT(  QVector<QPointF>, bool);
    void signalFFT2( QVector<QPointF>, bool);
    void signalFFT3( QVector<QPointF>, bool);

private:
//    const char *RecognModulation[];

};

#endif // PARSERFLAKON_H
