#ifndef TCPPRM300CONTROLLER_H
#define TCPPRM300CONTROLLER_H

#include "BaseClasses/BaseTcpDeviceController.h"

class TcpPRM300Controller : public BaseTcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpPRM300Controller(QObject* parent = NULL);
	virtual ~TcpPRM300Controller();
};

#endif // TCPPRM300CONTROLLER_H
