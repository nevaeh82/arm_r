#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QThread>
#include <QMap>

#include <PwLogger/PwLogger.h>

#include "Interfaces/ITcpManager.h"
#include "Interfaces/ITcpListener.h"
#include "../RPC/IRPC.h"
#include "Interfaces/IRpcListener.h"

#include "TcpDevicesDefines.h"
#include "TcpFlakonController.h"
#include "TcpAtlantController.h"
#include "TcpPRM300Controller.h"

#include "Rpc/RpcDefines.h"

class TcpManager : public QObject, public ITcpManager, public ITcpListener, public IRpcListener
{
	Q_OBJECT

private:
	Pw::Logger::ILogger* m_logger;
	IRPC* m_rpcServer;
	QMap< QString, BaseTcpDeviceController* > m_controllersMap;
	/**
	 * key = deviceType
	 * Define own deviceType in TcpDevicesDefines
	 *
	 **/

public:
	explicit TcpManager(QObject* parent = NULL);
	virtual ~TcpManager();

	// ITcpManager interface
public:
	virtual void addTcpDevice(const QString& deviceType, const QString& host, const quint32& port);
	virtual void removeTcpDevice(const QString& deviceType, const QString& host, const quint32& port);
	virtual void setRpcServer(IRPC* rpcServer);
	virtual QObject* asQObject();

	/// TODO: add sendDataToDevice from some internal message

	// ITcpListener interface
public:
	virtual void onMessageReceived(const QString& device, const IMessage<QByteArray>* argument);

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

signals:
	void threadTerminateSignal();

};

#endif // TCPMANAGER_H
