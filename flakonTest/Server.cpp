#include "Server.h"

Server::Server(QObject *parent) : QObject(parent)
{
	srv = new QTcpServer(this);
	srv->listen(QHostAddress::Any, 1022);

	connect(srv, SIGNAL(newConnection()), this, SLOT(onConnection()));

	onSend();
}

void Server::onConnection()
{
	scList.append( srv->nextPendingConnection() );
}

void Server::onSend()
{
	QFile f("spect.txt");
	if( !f.open(QIODevice::ReadOnly) )
		return;

	QByteArray txt;

	do
	{
		//txt = f.readLine();

        txt = f.read(100000);
//		pos += 1000000;
//		txt = f.readAll();

		foreach (QTcpSocket* sc, scList) {
			if( sc->state() != QAbstractSocket::ConnectedState ) {
				scList.removeOne(sc);
				continue;
			}

			sc->write(txt);
			sc->waitForBytesWritten();

			qApp->processEvents();
		}
		onPause();

		if( txt.length() <= 0 ) {
			qApp->processEvents();
			f.close();
			qDebug() << "f close ....";
			qDebug() << scList.size();
			//getchar();

			if( !f.open(QIODevice::ReadOnly) )
				return;
		}

	} while(true);


	qDebug() << "f close ....";
	f.close();

	//qApp->processEvents();

	//srvTime.singleShot(100, this, SLOT(onSend()));

}

void Server::onPause()
{
	QTime current = QTime::currentTime();
	current.start();

	while (current.elapsed() < 100) {
		qApp->processEvents();
	}
}
