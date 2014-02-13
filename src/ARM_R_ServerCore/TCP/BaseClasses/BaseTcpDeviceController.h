#ifndef BASETCPDEVICECONTROLLER_H
#define BASETCPDEVICECONTROLLER_H

#include <QObject>

#include <PwLogger/PwLogger.h>

#include "BaseSubject.h"
#include "../Interfaces/ITcpReceiver.h"
#include "../Interfaces/ITcpDeviceController.h"

#include "BaseTcpClient.h"
#include "BaseTcpDeviceCoder.h"

class BaseTcpDeviceController : public QObject, public ITcpDeviceController, public ITcpReceiver, public BaseSubject<ITcpReceiver>
{
	Q_OBJECT

protected:
	BaseTcpClient* m_tcpClient;
	BaseTcpDeviceCoder* m_tcpDeviceCoder;
	Pw::Logger::ILogger* m_logger;

public:
	explicit BaseTcpDeviceController(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~BaseTcpDeviceController();

	// ITcpDeviceController interface
public:
	virtual void connectToHost(const QString& host, const quint32& port);
	virtual void disconnectFromHost();
	virtual void isConnected();
	virtual void sendData(const QByteArray& data);
	virtual QObject* asQObject();

	// ITcpReceiver interface
public:
	virtual void onDataReceived(const QVariant& argument); // from BaseTcpClient
};

#endif // BASETCPDEVICECONTROLLER_H
