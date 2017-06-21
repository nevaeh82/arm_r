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

#include "TCP/TcpRdsSettingsController.h"

class TcpRDSController : public TcpDeviceController
{
	Q_OBJECT

private:
	QMap<QString, BaseTcpDeviceController*> m_stations;

public:
	explicit TcpRDSController(int serverId, QObject* parent = NULL);
	explicit TcpRDSController(int serverId, const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpRDSController();

	QMap<QString, BaseTcpDeviceController*>& stations();
	void setCoordinateCounter(CoordinateCounter* obj);

	void setTcpRdsSettingscontroller(TcpRDSSettingsController *controller);

	// ITcpDeviceController interface
	virtual void createTcpDeviceCoder();
	virtual void createTcpClient();
	virtual QObject* asQObject();

	virtual bool init();

	virtual QByteArray getFullInfo();

	virtual RpcRoutedServer::RouteId getRouteId() const;

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	int m_stationShift;

	void sendData(const MessageSP message) override;
private:
	int m_serverId;
	FlakonSettings m_flakonSettingStruct;
	CoordinateCounter* m_coordinateCounter;
	TcpRdsCoder* m_coder;

	TcpRDSSettingsController *m_tcpRdsSettingsController;

	void requestStationCorellation(QString stationName);

	void requestTest();

	QList<QByteArray> m_messageBuffer;
	QByteArray m_lastLocationMessage;
	QTime m_lastLocationMessageTime;
	QTimer* m_locationTimer;

	bool writeToRds(const QByteArray& data);

	QTimer* m_tcpSendTimer;
	QTimer* m_rdsTimeout;

	bool m_incoming;

private slots:
	void slotTcpConnectionStatus(int status);

	void onGetStations();
	void sendLocation();
	void onForceWriteToRds();

	void onSendDataInternal(QByteArray decodedData);
	void slotRdsTimeout();
signals:
	void outStationsList(QList<StationConfiguration> list);
	void signalSendDataInternal(QByteArray);
};
