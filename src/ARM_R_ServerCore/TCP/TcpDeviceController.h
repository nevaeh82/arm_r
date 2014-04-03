#ifndef TCPDEVICECONTROLLER_H
#define TCPDEVICECONTROLLER_H

#include <Interfaces/IRpcListener.h>
#include <Rpc/RpcRoutedServer.h>
#include <TCP/BaseTcpDeviceController.h>

class TcpDeviceController : public BaseTcpDeviceController, public IRpcListener
{
	Q_OBJECT

protected:
	RpcRoutedServer::RouteId m_routeId;

public:
	explicit TcpDeviceController(QObject *parent = NULL)
		: BaseTcpDeviceController(BASE_TCP_DEVICE, parent) {}

	explicit TcpDeviceController(const QString& deviceName, QObject *parent = NULL)
		: BaseTcpDeviceController(deviceName, parent) {}

	virtual RpcRoutedServer::RouteId getRouteId() const = 0;
};

#endif // TCPDEVICECONTROLLER_H
