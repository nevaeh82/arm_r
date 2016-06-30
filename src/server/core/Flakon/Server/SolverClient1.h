#pragma once

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>

#include <Tcp/TcpDeviceController.h>
#include <Interfaces/Tcp/ITcpListener.h>

#include <RadiolocationInterface.h>
#include "Info/FlakonSettings.h"

#include "TCP/TcpDefines.h"
#include "TcpDevicesDefines.h"

#include "SolverEncoder.h"
#include "SolverEncoder1.h"

#include "SolverPacket.pb.h"

#define TCP_SOLVER_HOST "127.0.0.1"
#define TCP_SOLVER_PORT 1024

#define CLIENT_SOLVER_DATA "SOLVER_DATA"
#define CLIENT_BLA_DATA "BLA_DATA"

class SolverClient1 : public  TcpDeviceController, public ITcpListener
{
	Q_OBJECT

public:
	SolverClient1(QString deviceName, QObject* parent = 0);
	~SolverClient1();

	void onMessageReceived(const quint32 deviceType,
								   const QString& device,
								   const MessageSP argument);

	void onMethodCalled(const QString& method, const QVariant& argument); // RPC data

	SolverEncoder1 *getSolverEncoder();

	// ITcpDeviceController interface
	virtual void createTcpDeviceCoder();
	virtual void createTcpClient();
	virtual QObject* asQObject();

	virtual bool init();

	virtual QByteArray getFullInfo();

	virtual RpcRoutedServer::RouteId getRouteId() const;

private:
	SolverEncoder1* m_encoder1;
	FlakonSettings m_flakonSettingStruct;
signals:
	void onDataSended(bool res);
};
