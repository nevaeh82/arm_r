#ifndef TCPFLAKONCODER_H
#define TCPFLAKONCODER_H

#include "Tcp/BaseTcpDeviceCoder.h"

#include <QDataStream>
#include <QPointF>
#include <QVector>

#include "CRCs.h"

#include "TcpDefines.h"

class TcpFlakonCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	ZaviruhaPayloadPacketHeader m_header;
	int m_residueLength;
	QByteArray m_dataFromTcpSocket;

public:
	explicit TcpFlakonCoder(QObject* parent = NULL);
	virtual ~TcpFlakonCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();

private:
	MessageSP messageFromPreparedData();
	MessageSP pointers(QVector<QPointF> vec);
	MessageSP correlation(quint32 point1, quint32 point2, QVector<QPointF> points);
	MessageSP detectedBandwidth(QVector<QPointF> vec);
};

#endif // TCPFLAKONCODER_H
