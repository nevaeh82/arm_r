#ifndef TCPATLANTCONTROLLER_H
#define TCPATLANTCONTROLLER_H

#include <QSettings>
#include <QStringList>

#include <Interfaces/IRpcListener.h>
#include <Info/AtlantSettings.h>

#include "TCP/TcpDeviceController.h"
#include "TCP/TcpAtlantCoder.h"

class TcpAtlantController : public TcpDeviceController
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

	virtual bool init();

	virtual QByteArray getFullInfo();

	virtual RpcRoutedServer::RouteId getRouteId() const;

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

private:
	AtlantSettings m_atlantSettingStruct;
};

#endif // TCPATLANTCONTROLLER_H
