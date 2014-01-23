#ifndef PARSERPRM300_H
#define PARSERPRM300_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QTextCodec>
#include "../Common/CRCs.h"
#include "ITCPParser.h"
#include "../Common/IRouter.h"
#include "TCPPacketStruct.h"
#include "../Common/ISubscriber.h"
#include "../RPC/Message.h"

//#include "../Common/MessageCom.h"

///Parser for flakon messages
class ParserPRM300 : public QObject, public ITCPParser
{
    Q_OBJECT
public:
    ParserPRM300(IRouter *router, int id);
    ~ParserPRM300();

public:
    virtual void distruct();
    virtual void parsing(QByteArray data);

private:
//    void _completeMsg();
    bool _find_magic();

    void _send_prm_status(quint16 freq, quint8 filter, quint8 att1, quint8 att2);

private:
    int                         _id;
    ISubscriber*                _subscriber;
    int                         _type;
//    ZaviruhaPayloadPacketHeader _header;
    int                         _restLength;
    QByteArray                  _msg;
    IRouter*                    _router;
    bool                        _needBytes;

    quint8                      _aAddr;
    quint8                      _aSize;
    quint8                      _aPacketType;

signals:

};

#endif // PARSERPRM300_H
