#ifndef TCPFLAKONCONTROLLER_H
#define TCPFLAKONCONTROLLER_H

#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpFlakonCoder.h"

class TcpFlakonController : public BaseTcpDeviceController
{
	Q_OBJECT

public:
	explicit TcpFlakonController(QObject* parent = NULL);
	explicit TcpFlakonController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpFlakonController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

signals:
	void createTcpFlakonCoderInternalSignal();

private slots:
	void createTcpFlakonCoderInternalSlot();
};

#endif // TCPFLAKONCONTROLLER_H