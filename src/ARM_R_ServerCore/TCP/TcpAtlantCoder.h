#ifndef TCPATLANTCODER_H
#define TCPATLANTCODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

#include <QDataStream>
#include <QDateTime>

#include "../Atlant/EMS/proto/EagleMessageProto.pb.h"
#include "../Atlant/EMS/storm.pb.h"

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
	virtual IMessage<QByteArray>* encode(const QByteArray& data);
	virtual QByteArray decode(const IMessage<QByteArray>* message);
	virtual QObject* asQObject();

private:
	IMessage<QByteArray>* messageFromPreparedData();
	QByteArray atlantSetFrequency(const QByteArray& data);
};

#endif // TCPATLANTCODER_H
