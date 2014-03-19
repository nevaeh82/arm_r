#ifndef TCPKTRCONTROLLER_H
#define TCPKTRCONTROLLER_H

#include <Tcp/BaseTcpDeviceController.h>

#include "TcpDevicesDefines.h"
#include "TcpKTRCoder.h"

class TcpKTRController : public BaseTcpDeviceController
{
	Q_OBJECT

public:
	explicit TcpKTRController(QObject* parent = NULL);
	explicit TcpKTRController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpKTRController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();

signals:
	void createTcpKTRCoderInternalSignal();

private slots:
	void createTcpKTRCoderInternalSlot();
};

#endif // TCPKTRCONTROLLER_H
