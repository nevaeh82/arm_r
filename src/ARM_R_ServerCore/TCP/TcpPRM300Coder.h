#ifndef TCPPRM300CODER_H
#define TCPPRM300CODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

class TcpPRM300Coder : public BaseTcpDeviceCoder
{
	Q_OBJECT
public:
	explicit TcpPRM300Coder(QObject* parent = NULL);
	virtual ~TcpPRM300Coder();

	// ITcpDeviceCoder interface
public:
	virtual QByteArray encode(const QByteArray& data);
	virtual QByteArray decode(const QByteArray& data);
	virtual QObject* asQObject();
};

#endif // TCPPRM300CODER_H
