#ifndef TCPFLAKONCODER_H
#define TCPFLAKONCODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

#include <QDataStream>
#include <QPointF>
#include <QVector>

#include "CRCs.h"

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
	virtual IMessage<QByteArray>* encode(const QByteArray& data);
	virtual QByteArray decode(const IMessage<QByteArray>* message);
	virtual QObject* asQObject();

private:
	IMessage<QByteArray>* messageFromPreparedData();
	IMessage<QByteArray>* pointers(QVector<QPointF> vec);
	IMessage<QByteArray>* correlation(quint32 point1, quint32 point2, QVector<QPointF> vec);
	IMessage<QByteArray>* detectedBandwidth(QVector<QPointF> vec);
};

#endif // TCPFLAKONCODER_H
