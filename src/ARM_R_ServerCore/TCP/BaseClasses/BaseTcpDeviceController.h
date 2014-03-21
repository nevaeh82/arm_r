#ifndef BASETCPDEVICECONTROLLER_H
#define BASETCPDEVICECONTROLLER_H

#include <QObject>
#include <QVariant>

#include <PwLogger/PwLogger.h>

#include "BaseSubject.h"
#include "../Interfaces/ITcpReceiver.h"
#include "../Interfaces/ITcpListener.h"
#include "../Interfaces/ITcpDeviceController.h"
#include "TcpDevicesDefines.h"

#include "BaseTcpClient.h"
#include "BaseTcpDeviceCoder.h"

class BaseTcpDeviceController : public QObject, public ITcpDeviceController, public ITcpReceiver, public BaseSubject<ITcpListener>
{
	Q_OBJECT

protected:
	BaseTcpClient* m_tcpClient;
	BaseTcpDeviceCoder* m_tcpDeviceCoder;
	QString m_tcpDeviceName;
	quint32 m_deviceType;

	Pw::Logger::ILogger* m_logger;

	QString	m_host;
	quint32	m_port;

	QByteArray m_deviceBaseInfo;
	QByteArray m_deviceFullInfo;

public:
	explicit BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	explicit BaseTcpDeviceController(const QString& tcpDeviceName, Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceController();

	// ITcpDeviceController interface
public:
	virtual void createTcpClient();
	virtual void createTcpDeviceCoder();
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void connectToHost();
	virtual void disconnectFromHost();
	virtual bool isConnected();
	virtual QString getHost();
	virtual void sendData(const MessageSP message);
	virtual QObject* asQObject();

	virtual void setDeviceInfo(const QByteArray& baseInfo, const QByteArray& fullInfo);
	virtual QByteArray getFullInfo();
	virtual bool init();
	// ITcpReceiver interface
public:
	virtual void onDataReceived(const QVariant& argument); // from BaseTcpClient

signals:
	void connectToHostInternalSignal(const QString& host, const quint32& port);
	void disconnectFromHostInternalSignal();
	void sendDataInternalSignal(const MessageSP message);
	void onDataReceivedInternalSignal(const QVariant& argument);
	void createTcpClientInternalSignal();
	void createTcpDeviceCoderInternalSignal();
	void signalTcpDeviceConnectedToHost(bool state);

private slots:
	void connectToHostInternalSlot(const QString& host, const quint32& port);
	void disconnectFromHostInternalSlot();
	void sendDataInternalSlot(const MessageSP message);
	void onDataReceivedInternalSlot(const QVariant& argument);
	void createTcpClientInternalSlot();
	void createTcpDeviceCoderInternalSlot();

};

#endif // BASETCPDEVICECONTROLLER_H
