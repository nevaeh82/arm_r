#ifndef TCPSERVERCONTROLLER_H
#define TCPSERVERCONTROLLER_H

#include <QObject>
#include "Tcp/BaseTcpServerController.h"
#include "TcpServerCoder.h"

#include "TcpDevicesDefines.h"

#include <Logger/Logger.h>

#include "Info/BaseSettings.h"

class TcpServerController : public BaseTcpServerController
{
public:
	explicit TcpServerController(QObject *parent = 0);
	explicit TcpServerController(const QString& tcpServerName, QObject *parent = 0);
	virtual ~TcpServerController();

	// ITcpServerController interface
public:
	virtual void createTcpServerCoder();
	virtual QObject* asQObject();
};

#endif // TCPSERVERCONTROLLER_H
