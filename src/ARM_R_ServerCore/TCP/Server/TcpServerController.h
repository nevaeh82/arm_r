#ifndef TCPSERVERCONTROLLER_H
#define TCPSERVERCONTROLLER_H

#include <QObject>
#include "Tcp/BaseTcpServerController.h"
#include "TcpServerCoder.h"

#include "Logger.h"

class TcpServerController : public BaseTcpServerController
{
public:
	explicit TcpServerController(const QString& tcpServerName = BASE_TCP_SERVER, QObject *parent = 0);
	virtual ~TcpServerController();

	// ITcpServerController interface
public:
	virtual void createTcpServerCoder();
	virtual QObject* asQObject();
};

#endif // TCPSERVERCONTROLLER_H
