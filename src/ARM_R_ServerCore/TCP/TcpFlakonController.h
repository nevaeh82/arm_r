#ifndef TCPFLAKONCONTROLLER_H
#define TCPFLAKONCONTROLLER_H

#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpFlakonCoder.h"

class TcpFlakonController : public BaseTcpDeviceController
{
	Q_OBJECT

public:
	explicit TcpFlakonController(QObject* parent = NULL);
	virtual ~TcpFlakonController();

	// ITcpDeviceController interface
public:
	virtual QObject* asQObject();
};

#endif // TCPFLAKONCONTROLLER_H
