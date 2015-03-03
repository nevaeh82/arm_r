#ifndef TCPSERVERCODER_H
#define TCPSERVERCODER_H

#include <QObject>
#include <QPointF>
#include <QTime>
#include <QDateTime>
#include <QVector>

#include "Tcp/BaseTcpDeviceCoder.h"
#include "TCP/TcpDefines.h"

#include "ZVPacket.pb.h"

class TcpServerCoder : public BaseTcpDeviceCoder
{
private:
	QByteArray m_dataFromTcpSocket;
	uint m_residueLength;
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

	void toProtoBufBplaCoords(Zaviruha::Packet::ArgumentVariant::BplaPoints*, QByteArray &data);

	void addPreambula(QByteArray&);
};

#endif // TCPSERVERCODER_H
