#ifndef TCPPRM300CONTROLLER_H
#define TCPPRM300CONTROLLER_H

#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpPRM300Coder.h"

class TcpPRM300Controller : public BaseTcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpPRM300Controller(QObject* parent = NULL);
	explicit TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpPRM300Controller();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();
};

#endif // TCPPRM300CONTROLLER_H
