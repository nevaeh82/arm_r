#ifndef CLIENTSERVERTEST_H
#define CLIENTSERVERTEST_H

#include <cxxtest/TestSuite.h>
#include <QtGlobal>
#include <QTcpSocket>
#include <QThread>

#include <QxtSignalWaiter>

#include "Flakon/CoordinateCounter.h"
#include "Flakon/Server/ClientServer.h"

class ClientServerTest: public CxxTest::TestSuite
{

public:
	ClientTcpServer* server;
	QTcpSocket* clientSocket;

	QThread* clientServerThread;

	void setUp()
	{
		server = new ClientTcpServer(0);

		clientServerThread = new QThread();
		server = new ClientTcpServer(0);

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

		TS_ASSERT_EQUALS( server->isListening(), false );
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
		clientSocket->connectToHost("127.0.0.1", 2020, QIODevice::ReadOnly);

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
		clientSocket->connectToHost("127.0.0.1", 2020, QIODevice::ReadOnly);
		bool res = QxtSignalWaiter::wait(server, SIGNAL(newClientSignal(uint,ITcpServerClient*)), 5000);
		TS_ASSERT_EQUALS(true, res);

		DataFromFlacon m_aData;
		m_aData.numOfReferenceDetector_ = 123;
		m_aData.time_ = QTime::currentTime();
		QVector<double> vector;
		m_aData.ranges_ = (vector  << 1.1 << 2.2 << 3.3 );

		QByteArray dataToSend;
		QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
		dataStream << QString( CLIENT_TCP_SERVER_SOLVER_DATA );
		dataStream << m_aData;
		MessageSP message(new Message<QByteArray>(CLIENT_TCP_SERVER_SOLVER_DATA, dataToSend));

		server->onMessageReceived(CLIENT_TCP_SERVER, "testDeviceName", message);

		QxtSignalWaiter::wait(clientSocket, SIGNAL(readyRead()), 5000);
		TS_ASSERT_DIFFERS( clientSocket->bytesAvailable(), 0 );

		QByteArray inData = clientSocket->readAll();
		QDataStream stream(&inData, QIODevice::ReadOnly);

		QString method;
		DataFromFlacon receiveData;

		stream >> method;
		stream >> receiveData.numOfReferenceDetector_;
		stream >> receiveData.time_;
		stream >> receiveData.ranges_;

		TS_ASSERT_EQUALS(method, QString(CLIENT_TCP_SERVER_SOLVER_DATA) );

		TS_ASSERT_EQUALS(receiveData.numOfReferenceDetector_, m_aData.numOfReferenceDetector_);
		TS_ASSERT_EQUALS(receiveData.time_, m_aData.time_);
		TS_ASSERT_EQUALS(receiveData.ranges_, m_aData.ranges_);
	}
};

#endif // CLIENTSERVERTEST_H
