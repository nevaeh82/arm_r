#ifndef TCPATLANTCONTROLLER_H
#define TCPATLANTCONTROLLER_H

#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpAtlantCoder.h"

class TcpAtlantController : public BaseTcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpAtlantController(QObject* parent = NULL);
	explicit TcpAtlantController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpAtlantController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

signals:
	void createTcpAtlantCoderInternalSignal();

private slots:
	void createTcpAtlantCoderInternalSlot();
};

#endif // TCPATLANTCONTROLLER_H
