#ifndef RPCCLIENTBASE_H
#define RPCCLIENTBASE_H

#include <QObject>
#include <QTimer>
#include <QxtRPCPeer>
#include <QHostAddress>

#include "Interfaces/IRpcControllerBase.h"

class RpcClientBase: public QObject, public IRpcControllerBase
{
	Q_OBJECT

private:
	quint16 m_port;
	QHostAddress m_address;
	QTimer* reconnectTimer;

protected:
	QxtRPCPeer* m_clientPeer;

public:
	explicit RpcClientBase(QObject* parent = NULL);
	virtual ~RpcClientBase(){}

	virtual bool start(quint16 port, QHostAddress address);
	virtual void stop();

private slots:
	void connectToServer();
	void connectedToServerSlot();
	void connectionToServerFailedSlot();
};

#endif // RPCCLIENTBASE_H
