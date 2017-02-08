#ifndef TCPRDSSETTINGSCONTROLLER_H
#define TCPRDSSETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QStringList>
#include <TcpDevicesDefines.h>

#include <Interfaces/IRpcListener.h>

#include "TCP/TcpDeviceController.h"

#include "Tcp/TcpRdsSettingsCoder.h"

class TcpRDSSettingsController : public TcpDeviceController
{
    Q_OBJECT
public:
    TcpRDSSettingsController(QObject* parent = NULL);
    ~TcpRDSSettingsController();

public:
    // ITcpDeviceController interface
    virtual void createTcpDeviceCoder();
    virtual void createTcpClient();
    virtual QObject* asQObject();

    virtual bool init();

    virtual QByteArray getFullInfo();

    virtual RpcRoutedServer::RouteId getRouteId() const;

    virtual void onMethodCalled(const QString& method, const QVariant& argument);

private:
    TcpRdsSettingsCoder* m_coder;
};

#endif // TCPRDSSETTINGSCONTROLLER_H
