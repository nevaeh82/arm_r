#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include <QDataStream>
#include <QTime>
#include <QSettings>
#include <QTextCodec>
#include <QByteArray>

#include <Tcp/BaseTcpServer.h>
#include <Interfaces/Tcp/ITcpListener.h>

#include <RadiolocationInterface.h>

#include "TCP/TcpDefines.h"
#include "TcpDevicesDefines.h"

#include "SolverEncoder.h"
#include "SolverEncoder1.h"
#include "SprutEncoder.h"

#include "SolverPacket.pb.h"

#define TCP_SERVER_NAME "CLIENT_TCPSERVER"
#define TCP_SERVER_PORT 2323
#define TCP_SERVER_PORT_1 2424

#define CLIENT_SOLVER_DATA "SOLVER_DATA"
#define CLIENT_BLA_DATA "BLA_DATA"

class ClientTcpServer : public BaseTcpServer, public ITcpListener
{
	Q_OBJECT

public:
	ClientTcpServer(int port, int type = 0, QObject* parent = 0);
	~ClientTcpServer();

	void onMessageReceived( const quint32 deviceType,
							const QString& device,
							const MessageSP argument );

	SolverEncoder* getSolverEncoder();

public slots:
	void startServer();
	void stopServer();

private slots:
	void onRegisterNewConnection(uint n, ITcpServerClient*client);

	void slotNiipDataIncome(QString, bool);

	void slotSprutIncome(QByteArray data);
private:
	ITcpReceiver* m_encoder;
	SolverEncoder* m_solverEncoder;
	SprutEncoder* m_sprutEncoder;
	int m_port;
	int m_type;
	QByteArray m_sprutBuffer;

signals:
	void onDataSended(bool res);

	void signalNiippData(QString, bool);
};

inline QDataStream& operator<<(QDataStream& out, const DataFromFlacon& object)
{
	return out << object.numOfReferenceDetector_ << object.time_ << object.ranges_;
}

inline QDataStream& operator<<(QDataStream& out, const DataFromFlaconInt& object)
{
	return out << object.numOfReferenceDetector_ << object.time_ << object.ranges_;
}

#endif; //CLIENTSERVER_H
