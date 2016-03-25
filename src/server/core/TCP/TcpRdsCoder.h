#pragma once

#include "Tcp/BaseTcpDeviceCoder.h"
#include "Info/StationConfiguration.h"

#include <QDataStream>
#include <QPointF>
#include <QVector>

#include "CRCs.h"

#include "TcpDefines.h"

class TcpRdsCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	ZaviruhaPayloadPacketHeader m_header;
	int m_residueLength;
	QByteArray m_dataFromTcpSocket;
	int cnt;

public:
	explicit TcpRdsCoder(QObject* parent = NULL);
	virtual ~TcpRdsCoder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const MessageSP message);
	virtual QObject* asQObject();

private:
	MessageSP messageFromPreparedData(const QByteArray& data);
	MessageSP pointers(int index, QVector<QPointF> vec);
	MessageSP configure(const QList<StationConfiguration>& lst);
	MessageSP correlation(quint32 point1, quint32 point2, QVector<QPointF> points);
	MessageSP detectedBandwidth(QVector<QPointF> vec);

	void addPreambula(QByteArray& data);
	QByteArray getMessage(const QByteArray& input);
};
