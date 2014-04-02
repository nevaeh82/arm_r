#ifndef TCPFLAKONCONTROLLER_H
#define TCPFLAKONCONTROLLER_H

#include <QSettings>
#include <QStringList>

#include <Interfaces/IRpcListener.h>
#include <TcpDevicesDefines.h>
#include <Info/FlakonSettings.h>

#include "TCP/TcpDeviceController.h"
#include "TCP/TcpFlakonCoder.h"

class TcpFlakonController : public TcpDeviceController
{
	Q_OBJECT

private:
	QMap<QString, BaseTcpDeviceController*> m_stations;

public:
	explicit TcpFlakonController(QObject* parent = NULL);
	explicit TcpFlakonController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpFlakonController();

	QMap<QString, BaseTcpDeviceController*>& stations();

	// ITcpDeviceController interface
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

	virtual bool init();

	virtual QByteArray getFullInfo();

	virtual RpcRoutedServer::RouteId getRouteId() const;

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

signals:
	void createTcpFlakonCoderInternalSignal();

private slots:
	void createTcpFlakonCoderInternalSlot();

private:
	FlakonSettings m_flakonSettingStruct;

	void requestStationCorellation(QString stationName);
};

#endif // TCPFLAKONCONTROLLER_H
