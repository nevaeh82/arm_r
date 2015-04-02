#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>

#include <Tcp/BaseTcpServer.h>
#include <Interfaces/Tcp/ITcpListener.h>

#include <RadiolocationInterface.h>

#include "TCP/TcpDefines.h"
#include "TcpDevicesDefines.h"

#include "SolverEncoder.h"

#include "SolverPacket.pb.h"

#define TCP_SERVER_NAME "CLIENT_TCPSERVER"
#define TCP_SERVER_PORT 2021

#define CLIENT_SOLVER_DATA "SOLVER_DATA"
#define CLIENT_BLA_DATA "BLA_DATA"

class ClientTcpServer : public BaseTcpServer, public ITcpListener
{
	Q_OBJECT

public:
	ClientTcpServer(QObject* parent = 0);
	~ClientTcpServer();

	void onMessageReceived(const quint32 deviceType,
								   const QString& device,
								   const MessageSP argument);

	SolverEncoder* getSolverEncoder();

public slots:
	void startServer();
	void stopServer();

private:
	int getClientTcpPortValue();

	QByteArray decode(const MessageSP message);
	void toProtobufSolverData(SolverClient::Packet::ArgumentVariant::SolverInput* arg, QByteArray& data);
	void addPreambula(QByteArray& data);

	SolverEncoder* m_encoder;

signals:
	void onDataSended(bool res);
};

inline QDataStream& operator<<(QDataStream& out, const DataFromFlacon& object)
{
	return out << object.numOfReferenceDetector_ << object.time_ << object.ranges_;
}

#endif; //CLIENTSERVER_H
