#ifndef TCPFLAKONCONTROLLER_H
#define TCPFLAKONCONTROLLER_H

#include <QSettings>
#include <QStringList>

#include <Interfaces/IRpcListener.h>
#include <TcpDevicesDefines.h>
#include <Info/FlakonSettings.h>

#include "Flakon/CoordinateCounter.h"

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
	void setCoordinateCounter(CoordinateCounter* obj);

	// ITcpDeviceController interface
	virtual void createTcpDeviceCoder();
	virtual void createTcpClient();
	virtual QObject* asQObject();

	virtual bool init();

	virtual QByteArray getFullInfo();

	virtual RpcRoutedServer::RouteId getRouteId() const;

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

private:
	FlakonSettings m_flakonSettingStruct;
	CoordinateCounter* m_coordinateCounter;

	void requestStationCorellation(QString stationName);

private slots:
	void slotTcpConnectionStatus(int status);
};

#endif // TCPFLAKONCONTROLLER_H
