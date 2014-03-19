#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include "Tcp/BaseTcpServer.h"

class TcpServer : public BaseTcpServer
{
public:
	explicit TcpServer(QObject *parent = 0);
	
signals:
	
private slots:
	void onNewClientSlot(uint, QTcpSocket*);
	
};

#endif // TCPSERVER_H
