#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QCoreApplication>

class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QObject *parent = 0);

	void onPause();
private:
	QTcpServer* srv;

	QList<QTcpSocket*> scList;
	QTimer srvTime;

	QByteArray common;

signals:

public slots:

	void onConnection();
	void onSend();
};

#endif // SERVER_H
