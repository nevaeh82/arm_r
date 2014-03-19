#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent) :
	BaseTcpServer(parent)
{
	connect(this, SIGNAL(newClientSignal(uint,QTcpSocket*)), this, SLOT(onNewClientSlot(uint,QTcpSocket*)));
}

void TcpServer::onNewClientSlot(uint, QTcpSocket*)
{
}
