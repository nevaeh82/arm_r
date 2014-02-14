#ifndef TCPATLANTCODER_H
#define TCPATLANTCODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

class TcpAtlantCoder : public BaseTcpDeviceCoder
{
	Q_OBJECT
public:
	explicit TcpAtlantCoder(QObject* parent = NULL);

	// ITcpDeviceCoder interface
public:
	virtual QByteArray encode(const QByteArray& data);
	virtual QByteArray decode(const QByteArray& data);
	virtual QObject* asQObject();
};

#endif // TCPATLANTCODER_H
