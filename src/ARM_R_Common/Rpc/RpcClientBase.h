#ifndef RPCCLIENTBASE_H
#define RPCCLIENTBASE_H

#include <QObject>
#include <QTimer>
#include <QxtRPCPeer>
#include <QString>

#include <PwLogger/PwLogger.h>
#include "Interfaces/IRpcControllerBase.h"

class RpcClientBase: public QObject, public IRpcControllerBase
{
	Q_OBJECT

private:
	quint16 m_port;
	QString m_ipAddress;
	QTimer * reconnectTimer;

protected:
	Pw::Logger::ILogger* m_logger;
	QxtRPCPeer* m_clientPeer;
	QList<IRpcListener*> m_listeners;

public:
	explicit RpcClientBase(Pw::Logger::ILogger* logger, QObject* parent = NULL);
	virtual ~RpcClientBase(){}

	virtual bool start(QString& ipAddress, quint16 port);
	virtual void stop();

	virtual void registerListener(IRpcListener* listener);
	virtual void deregisterListener(IRpcListener* listener);

private slots:
	void connectToServer();
	void connectedToServerSlot();
	void connectionToServerFailedSlot();
};

#endif // RPCCLIENTBASE_H
