#ifndef CLIENTSERVERTEST_H
#define CLIENTSERVERTEST_H

#include <cxxtest/TestSuite.h>
#include <QtGlobal>
#include <QTcpSocket>
#include <QThread>
#include <QSettings>

#include <QxtSignalWaiter>

#include "ZVPacket.pb.h"

#include "Flakon/CoordinateCounter.h"
#include "Flakon/Server/ClientServer.h"

class ClientServerTest: public CxxTest::TestSuite
{

public:
	ClientTcpServer* server;
	QTcpSocket* clientSocket;

	QThread* clientServerThread;
	int m_serverPort;

	QByteArray rawTestProtobuf;

	void setUp()
	{
		server = new ClientTcpServer(0);

		clientServerThread = new QThread();
		server = new ClientTcpServer(0);

		QSettings param("./ARM_R.ini", QSettings::IniFormat, 0);
		param.setIniCodec(QTextCodec::codecForName("UTF-8"));
		m_serverPort = param.value("ClientTCPServer/Port", TCP_SERVER_PORT).toInt();

		QObject::connect(clientServerThread, SIGNAL(started()), server, SLOT(startServer()));
		QObject::connect(server, SIGNAL(destroyed()), clientServerThread, SLOT(quit()));
		QObject::connect(clientServerThread, SIGNAL(finished()), server, SLOT(stopServer()), Qt::DirectConnection);
		QObject::connect(clientServerThread, SIGNAL(finished()), server, SLOT(deleteLater()));
		QObject::connect(clientServerThread, SIGNAL(finished()), clientServerThread, SLOT(deleteLater()));

		server->moveToThread(clientServerThread);
		clientServerThread->start();

		bool res = QxtSignalWaiter::wait(clientServerThread, SIGNAL(started()), 1000);
		TS_ASSERT_EQUALS(true, res);

		clientSocket = new QTcpSocket(0);
	}

	void tearDown()
	{
		clientSocket->disconnectFromHost();
		clientSocket->deleteLater();

		clientServerThread->quit();
		bool res = QxtSignalWaiter::wait(clientServerThread, SIGNAL(finished()), 1000);
		TS_ASSERT_EQUALS(true, res);
	}

	void testServerSendAnyDataWhenNoClients()
	{
		bool testResult;
		testResult = server->sendData(QByteArray("Hello!"));

		TS_ASSERT_EQUALS( testResult, false );
		TS_ASSERT_EQUALS( clientSocket->bytesAvailable(), 0 );
	}

	void testConnectToClientServerAndSendAnyData()
	{
		TS_ASSERT_EQUALS(clientServerThread->isRunning(), true);
		clientSocket->connectToHost("127.0.0.1", m_serverPort, QIODevice::ReadOnly);

		bool res = QxtSignalWaiter::wait(server, SIGNAL(newClientSignal(uint,ITcpServerClient*)), 5000);
		TS_ASSERT_EQUALS(true, res);

		bool testResult;
		testResult = server->sendData(QByteArray("Hello!"));
		TS_ASSERT_EQUALS( testResult, true );

		QxtSignalWaiter::wait(clientSocket, SIGNAL(readyRead()), 5000);
		TS_ASSERT_EQUALS( clientSocket->bytesAvailable(), 6 );
	}

	void testClientServerOnMessageReceive()
	{
		clientSocket->connectToHost("127.0.0.1", m_serverPort, QIODevice::ReadOnly);
		bool res = QxtSignalWaiter::wait(server, SIGNAL(newClientSignal(uint,ITcpServerClient*)), 5000);
		TS_ASSERT_EQUALS(true, res);

		DataFromFlacon m_aData;
		m_aData.numOfReferenceDetector_ = 123;
		m_aData.time_ = QTime::currentTime();
		QVector<double> vector;
		m_aData.ranges_ = ( vector  << 1.1 << 2.2 << 3.3 );

		QByteArray dataToSend;
		QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
		dataStream << m_aData;
		dataStream << 1355.55; // Setting some of central frequency
		MessageSP message(new Message<QByteArray>(CLIENT_TCP_SERVER_SOLVER_DATA, dataToSend));

		server->onMessageReceived(CLIENT_TCP_SERVER, "testDeviceName", message);

		QxtSignalWaiter::wait(clientSocket, SIGNAL(readyRead()), 5000);
		TS_ASSERT_DIFFERS( clientSocket->bytesAvailable(), 0 );

		QByteArray inData = clientSocket->readAll();
		rawTestProtobuf = inData; // Prepare raw data for testSolverEncoder()

		TS_ASSERT_LESS_THAN(TCP_ZAVIRUHA_PREAMBULA_LEN, inData.size());
		TS_ASSERT_EQUALS(0, inData.indexOf(TCP_ZAVIRUHA_PREAMBULA_SOLVER) );

		int messageLen = 0;
		memcpy((char*)&messageLen, inData.data() + 3, sizeof(uint));

		int packetLen = messageLen + TCP_ZAVIRUHA_PREAMBULA_LEN;

		TS_ASSERT_EQUALS(packetLen, inData.size());

// ========================================================================================
		QByteArray protoData = inData.mid(TCP_ZAVIRUHA_PREAMBULA_LEN, messageLen);
		SolverClient::Packet packet;
		packet.ParseFromArray(protoData.constData(), protoData.size());

		TS_ASSERT_EQUALS( packet.command().action(), SolverClient::sendSolverClientData );
		SolverClient::Packet::ArgumentVariant::SolverInput arg = packet.command().arguments().solverinput();

		TS_ASSERT_EQUALS(arg.delays_size(), 3);
		TS_ASSERT_EQUALS(arg.delays(0), 1.1);
		TS_ASSERT_EQUALS(arg.delays(1), 2.2);
		TS_ASSERT_EQUALS(arg.delays(2), 3.3);

		//TS_ASSERT_EQUALS(arg.datetime(), m_aData.time_.toTime_t());
		TS_ASSERT_EQUALS(arg.centerfrequency(), 1355.55);
	}

	void testSolverEncoder()
	{
		SolverEncoder encoder;
		TS_ASSERT_DIFFERS( 0, rawTestProtobuf.length() );

		rawTestProtobuf.prepend("TESTdataNoValidMessageOlolo");
		rawTestProtobuf.append("EndOfMessageInvalidData");

		QByteArray part1 = rawTestProtobuf.mid(0, 30);
		QByteArray part2 = rawTestProtobuf.mid(30, 10);
		QByteArray part3 = rawTestProtobuf.mid(40, 10);
		QByteArray part4 = rawTestProtobuf.right(rawTestProtobuf.length() - 50);

		TS_ASSERT_EQUALS(rawTestProtobuf.length(), part1.length() + part2.length() + part3.length() + part4.length() );

		for(int i = 0; i < 3; i++) {
			QByteArray encodedData = encoder.encode( part1 );
			TS_ASSERT_EQUALS( 0, encodedData.length() );
			encodedData = encoder.encode( part2 );
			TS_ASSERT_EQUALS( 0, encodedData.length() );
			encodedData = encoder.encode( part3 );
			TS_ASSERT_EQUALS( 0, encodedData.length() );
			encodedData = encoder.encode( part4 );
			TS_ASSERT_DIFFERS( 0, encodedData.length() );
		}
	}
};

#endif // CLIENTSERVERTEST_H
