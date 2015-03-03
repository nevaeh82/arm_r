#ifndef TCPATLANTCODER_H
#define TCPATLANTCODER_H

#include "Tcp/BaseTcpDeviceCoder.h"

#include <QDataStream>
#include <QDateTime>

#include "../Atlant/EMS/proto/EagleMessageProto.pb.h"
#include "storm.pb.h"

#include "TcpDevicesDefines.h"
#include "TcpDefines.h"

#include "RPC/RpcDefines.h"

class TcpAtlantCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	int m_residueLength;
	QByteArray m_dataFromTcpSocket;

public:
	explicit TcpAtlantCoder(QObject* parent = NULL);
	virtual ~TcpAtlantCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();

private:
	MessageSP messageFromPreparedData();
	QByteArray atlantSetFrequency(const QByteArray& data);
};

#endif // TCPATLANTCODER_H
