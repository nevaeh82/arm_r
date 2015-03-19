#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>

#include <Tcp/BaseTcpServer.h>

#include <RadiolocationInterface.h>

#define TCP_SERVER_NAME "CLIENT_TCPSERVER"
#define TCP_SERVER_PORT 2021

#define CLIENT_SOLVER_DATA "SOLVER_DATA"
#define CLIENT_BLA_DATA "BLA_DATA"

class ClientTcpServer : public BaseTcpServer
{
	Q_OBJECT

public:
	ClientTcpServer(QObject* parent = 0);
	~ClientTcpServer();

	bool sendClientSolverData( DataFromFlacon clientData );

public slots:
	void startServer();

private:
	int getClientTcpPortValue();
};

inline QDataStream& operator<<(QDataStream& out, const DataFromFlacon& object)
{
	return out << object.numOfReferenceDetector_ << object.time_ << object.ranges_;
}

#endif; //CLIENTSERVER_H
