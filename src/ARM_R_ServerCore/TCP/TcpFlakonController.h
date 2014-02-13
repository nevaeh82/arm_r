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
};

#endif // TCPFLAKONCONTROLLER_H
