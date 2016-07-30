#pragma once

#include <QSettings>
#include <QStringList>

#include <Interfaces/IRpcListener.h>
#include <TcpDevicesDefines.h>
#include <Info/FlakonSettings.h>

#include "Flakon/CoordinateCounter.h"

#include "TCP/TcpDeviceController.h"
#include "TCP/TcpRdsCoder.h"

#include "Info/StationConfiguration.h"

class TcpRDSController : public TcpDeviceController
{
	Q_OBJECT

private:
	QMap<QString, BaseTcpDeviceController*> m_stations;

public:
	explicit TcpRDSController(QObject* parent = NULL);
	explicit TcpRDSController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpRDSController();

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

    uint m_stationShift;

private:
	FlakonSettings m_flakonSettingStruct;
	CoordinateCounter* m_coordinateCounter;

	void requestStationCorellation(QString stationName);

	void requestTest();

private slots:
	void slotTcpConnectionStatus(QMap<int, bool>);
	void slotDetectSignal(int index, QVector<QPointF> vec);

	void onGetStations();

signals:
	void outStationsList(QList<StationConfiguration> list);
};
