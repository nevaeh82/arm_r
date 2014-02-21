#ifndef TCPPRM300CODER_H
#define TCPPRM300CODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

#include <QIODevice>
#include <QDebug>

#include "../Common/CRCs.h"

class TcpPRM300Coder : public BaseTcpDeviceCoder
{
	Q_OBJECT

private:
	int			m_residueLength;
	QByteArray	m_dataFromTcpSocket;
	bool		m_needBytes;

	quint8		m_aAddr;
	quint8		m_aSize;
	quint8		m_aPacketType;

public:
	explicit TcpPRM300Coder(QObject* parent = NULL);
	virtual ~TcpPRM300Coder();

	// ITcpDeviceCoder interface
public:
	virtual MessageSP encode(const QByteArray& data);
	virtual QByteArray decode(const IMessage<QByteArray>* message);
	virtual QObject* asQObject();

private:
	bool findPreamble();
	MessageSP sendPRMStatus(quint16 freq, quint8 filter, quint8 att1, quint8 att2);

	QByteArray prmSetFrequency(unsigned short aFreq);
	bool specialSymb(quint8 &aByteA, quint8 &aByteB);
	QByteArray prmGetFrequency();
	QByteArray prmSetAttenuerOne(int value);
	QByteArray prmSetAttenuerTwo(int value);
	QByteArray prmSetFilter(int value);
};

#endif // TCPPRM300CODER_H
