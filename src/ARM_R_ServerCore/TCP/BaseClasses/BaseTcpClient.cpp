#include "BaseTcpClient.h"

BaseTcpClient::BaseTcpClient(QObject* parent) :
	QObject(parent)
{
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(BaseTcpClient));

	m_host = "127.0.0.1";
	m_port = 0;

	connect(this, SIGNAL(connectToHostInternalSignal(QString,quint32)), this, SLOT(connectToHostInternalSlot(QString,quint32)));
	connect(this, SIGNAL(disconnectFromHostInternalSignal()), this, SLOT(disconnectFromHostInternalSlot()));
	connect(this, SIGNAL(writeDataInternalSignal(QByteArray)), this, SLOT(writeDataInternalSlot(QByteArray)));

	m_reconnectTimer = new QTimer(this);
	connect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(reconnectSlot()));

	m_tcpSocket = new QTcpSocket(this);

	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError)));
}

BaseTcpClient::~BaseTcpClient()
{
}

void BaseTcpClient::connectToHost(const QString& host, const quint32& port)
{
	emit connectToHostInternalSignal(host, port);
}

void BaseTcpClient::disconnectFromHost()
{
	emit disconnectFromHostInternalSignal();
}

bool BaseTcpClient::isConnected()
{
	switch(m_tcpSocket->state())
	{
		case QAbstractSocket::ConnectedState:
			emit signalConnectedToHost(1);
		break;
		case QAbstractSocket::UnconnectedState:
			emit signalConnectedToHost(0);
			break;
		default:
			emit signalConnectedToHost(0);
		break;
	}

	return m_tcpSocket->state() == QAbstractSocket::ConnectedState;
}

void BaseTcpClient::writeData(const QByteArray& data)
{
	emit writeDataInternalSignal(data);
}

QString BaseTcpClient::getHost()
{
	QString host = m_host + ":" + QString::number(m_port);
	return host;
}

QObject* BaseTcpClient::asQObject()
{
	return this;
}

void BaseTcpClient::onSocketConnected()
{
	m_reconnectTimer->stop();
	m_logger->debug("Socket connected");
	emit signalConnectedToHost(1);
}

void BaseTcpClient::onSocketDisconnected()
{
	m_logger->debug("Socket disconnected");
	emit signalConnectedToHost(0);
}

void BaseTcpClient::onSocketReadyRead()
{
//	m_logger->debug("Socket ready read");
	QByteArray data = m_tcpSocket->readAll();

	foreach (ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(data));
	}
}

void BaseTcpClient::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
			m_logger->debug("Error! Connection with TCP device was lost");
			break;
		case QAbstractSocket::HostNotFoundError:
			m_logger->debug("Error! Host not found");
			break;
		case QAbstractSocket::ConnectionRefusedError:
			m_logger->debug("Error! Connection was refused");
			break;
		default:
			m_logger->debug(QString("Error! Error text: %1").arg(m_tcpSocket->errorString()));
			break;
	}

	m_logger->debug(QString("Host = %1, Port = %2").arg(m_host).arg(m_port));
	m_reconnectTimer->start(1000);

//	emit signalConnectedToHost(false);
}

void BaseTcpClient::connectToHostInternalSlot(const QString& host, const quint32& port)
{
	m_host = host;
	m_port = port;

	m_reconnectTimer->stop();
	m_tcpSocket->connectToHost(m_host, m_port);
	if (!m_tcpSocket->waitForConnected(300)) {
		m_logger->debug(QString("Connect failed to %1:%2").arg(host).arg(QString::number(port)));
	}
	m_logger->debug(QString("Connectection established to %1:%2").arg(host).arg(QString::number(port)));

}

void BaseTcpClient::disconnectFromHostInternalSlot()
{
	m_reconnectTimer->stop();
	m_tcpSocket->disconnectFromHost();
	m_tcpSocket->abort();
//	emit signalConnectedToHost(false);
}

void BaseTcpClient::writeDataInternalSlot(const QByteArray& data)
{
	m_tcpSocket->write(data);
}

void BaseTcpClient::reconnectSlot()
{
	if(m_tcpSocket->state() != QAbstractSocket::UnconnectedState)
		return;

	m_tcpSocket->reset();
	if(!m_tcpSocket->isValid())
	{
		m_tcpSocket->disconnectFromHost();
		QTextStream(stdout) << "socket_->isNOTValid()" << endl;

	}
	m_tcpSocket->connectToHost(m_host, m_port);
	m_tcpSocket->waitForConnected(1000);
}
