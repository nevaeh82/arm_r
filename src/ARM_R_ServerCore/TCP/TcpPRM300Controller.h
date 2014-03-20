#ifndef TCPPRM300CONTROLLER_H
#define TCPPRM300CONTROLLER_H

#include "Tcp/BaseTcpDeviceController.h"
#include "TcpPRM300Coder.h"

#include "Logger.h"
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

signals:
	void createTcpPRM300CoderInternalSignal();

private slots:
	void createTcpPRM300CoderInternalSlot();
};

#endif // TCPPRM300CONTROLLER_H
