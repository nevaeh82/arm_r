#ifndef PARSERKTR_H
#define PARSERKTR_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QPointF>
#include "../Common/CRCs.h"
#include "ITCPParser.h"
#include "../Common/IRouter.h"
#include "TCPPacketStruct.h"

#include "../Common/ISubscriber.h"
#include "../RPC/Message.h"

///Parser for KTR messages
class ParserKTR : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    ParserKTR(int id, IRouter *router);
    ~ParserKTR();

public:
    virtual void distruct();
    virtual void parsing(QByteArray data);

private:
    void _send_bla_points(QPointF point, double alt);

private:
    int                         _type;
    int                         _restLength;
    QByteArray                  _msg;
    QByteArray                  _msg_all;
    IRouter*                    _router;
    ISubscriber*                _subscriber;

    double        _latitude;
    double        _longitude;
    double        _altitude;
    int           _id;

signals:
    void signalFFT(  QVector<QPointF>, bool);
    void signalFFT2( QVector<QPointF>, bool);
    void signalFFT3( QVector<QPointF>, bool);
};

#endif // PARSERKTR_H
