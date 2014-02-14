#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QThread>
#include <QMap>

#include <PwLogger/PwLogger.h>

#include "Interfaces/ITcpManager.h"
#include "Interfaces/ITcpReceiver.h"
#include "../RPC/IRPC.h"

#include "TcpDevicesDefines.h"
#include "TcpFlakonController.h"
#include "TcpAtlantController.h"
#include "TcpPRM300Controller.h"

class TcpManager : public QObject, public ITcpManager, public ITcpReceiver
{
	Q_OBJECT

private:
	Pw::Logger::ILogger* m_logger;
	IRPC* m_rpcServer;
	QMap< QString, QMap<QString, BaseTcpDeviceController*> > m_controllersMap;

public:
	explicit TcpManager(QObject* parent = NULL);
	virtual ~TcpManager();

	// ITcpManager interface
public:
	virtual void addConnection(const QString& deviceType, const QString& host, const quint32& port);
	virtual void removeConnection(const QString& deviceType, const QString& host, const quint32& port);
	virtual void setRpcServer(IRPC* rpcServer);
	virtual QObject* asQObject();

	// ITcpReceiver interface
public:
	virtual void onDataReceived(const QVariant& argument);

signals:
	void threadTerminateSignal();

};

#endif // TCPMANAGER_H
