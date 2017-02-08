#ifndef TCPRDSSETTINGSCODER_H
#define TCPRDSSETTINGSCODER_H

#include <QObject>
#include <QSettings>
#include <QTextCodec>
#include "TcpDefines.h"
#include "Tcp/BaseTcpDeviceCoder.h"
#include "CPPacket.pb.h"

#include "Rpc/RpcDefines.h"

class TcpRdsSettingsCoder : public BaseTcpDeviceCoder
{
    Q_OBJECT
public:
    TcpRdsSettingsCoder(QObject* parent = NULL);
    ~TcpRdsSettingsCoder();

public:
    virtual MessageSP encode(const QByteArray& data);
    virtual QByteArray decode(const MessageSP message);

//    virtual QObject* asQObject();
};

#endif // TCPRDSSETTINGSCODER_H
