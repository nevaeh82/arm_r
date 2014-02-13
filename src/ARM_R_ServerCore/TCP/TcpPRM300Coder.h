#ifndef TCPPRM300CODER_H
#define TCPPRM300CODER_H

#include "BaseClasses/BaseTcpDeviceCoder.h"

class TcpPRM300Coder : public BaseTcpDeviceCoder
{
	Q_OBJECT
public:
	explicit TcpPRM300Coder(QObject* parent = NULL);
	virtual ~TcpPRM300Coder();
};

#endif // TCPPRM300CODER_H
