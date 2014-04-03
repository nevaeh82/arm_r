#ifndef TCPPRM300CONTROLLER_H
#define TCPPRM300CONTROLLER_H

#include <QSettings>
#include <QStringList>

#include <Interfaces/IRpcListener.h>
#include <TcpDevicesDefines.h>
#include <Info/Prm300Settings.h>

#include "TCP/TcpDeviceController.h"
#include "TCP/TcpPRM300Coder.h"
#include "TCP/TcpDefines.h"


class TcpPRM300Controller : public TcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpPRM300Controller(QObject* parent = NULL);
	explicit TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpPRM300Controller();

	virtual bool init();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

	virtual QByteArray getFullInfo();

	virtual RpcRoutedServer::RouteId getRouteId() const;

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

private slots:
	void slotTcpConnectionStatus(int status);

private:
	Prm300Settings m_prmSettings;
	RpcRoutedServer::RouteId m_routeId;
};

#endif // TCPPRM300CONTROLLER_H
