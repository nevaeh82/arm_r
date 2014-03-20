#ifndef TCPSERVERCODER_H
#define TCPSERVERCODER_H

#include <QObject>

#include "Tcp/BaseTcpDeviceCoder.h"
#include "TCP/TcpDefines.h"

#include "Protobuf/ARMR_OD/ZVPacket.pb.h"

class TcpServerCoder : public BaseTcpDeviceCoder
{
private:
	QByteArray m_dataFromTcpSocket;
	int m_residueLength;
public:
	explicit TcpServerCoder(QObject *parent = 0);
	virtual ~TcpServerCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();
	
private:
	MessageSP fromProtoBuf(const QByteArray& data);
	QByteArray toProtoBuf(const MessageSP& message);
};

#endif // TCPSERVERCODER_H
