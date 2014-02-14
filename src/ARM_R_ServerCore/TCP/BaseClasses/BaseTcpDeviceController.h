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
	Q_OBJECT

protected:
	BaseTcpClient* m_tcpClient;
	BaseTcpDeviceCoder* m_tcpDeviceCoder;
	QString m_tcpDeviceName;

	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceController();

	// ITcpDeviceController interface
public:
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual bool isConnected();
	virtual void sendData(const IMessage<QByteArray>* message);
	virtual QObject* asQObject();

	// ITcpReceiver interface
public:
	virtual void onDataReceived(const QVariant& argument); // from BaseTcpClient

signals:
	void connectToHostInternalSignal(const QString& host, const quint32& port);
	void disconnectFromHostInternalSignal();
	void sendDataInternalSignal(const IMessage<QByteArray>* message);
	void onDataReceivedInternalSignal(const QVariant& argument);

private slots:
	void connectToHostInternalSlot(const QString& host, const quint32& port);
	void disconnectFromHostInternalSlot();
	void sendDataInternalSlot(const IMessage<QByteArray>* message);
	void onDataReceivedInternalSlot(const QVariant& argument);
};

#endif // BASETCPDEVICECONTROLLER_H
