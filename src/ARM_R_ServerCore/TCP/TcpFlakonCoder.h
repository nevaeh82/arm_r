#ifndef TCPFLAKONCODER_H
#define TCPFLAKONCODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

class TcpFlakonCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT
public:
	explicit TcpFlakonCoder(QObject* parent = NULL);
	virtual ~TcpFlakonCoder();

	// ITcpDeviceCoder interface
public:
	virtual QByteArray encode(const QByteArray& data);
	virtual QByteArray decode(const QByteArray& data);
};

#endif // TCPFLAKONCODER_H
