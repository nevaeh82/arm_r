#include "BaseTcpClient.h"

#include "Sleeper.h"

#include "QSettings"

#define BASE_TCP_RECONNECT_TIMEOUT 3000

BaseTcpClient::BaseTcpClient(const bool isReadRequired, QObject* parent):
	QIODevice( parent ),
	m_reconnectTimeout( BASE_TCP_RECONNECT_TIMEOUT )
{
	m_host = "127.0.0.1";
	m_port = 0;

	QSettings settings("./TCP/reconnect.ini", QSettings::IniFormat, this);
	m_reconnectPort = settings.value("FLAKON/port", 1024).toInt();
	log_debug(QString("Reconnect port: %1 - %2").arg(m_port).arg(m_reconnectPort));

	connect(this, SIGNAL(writeDataInternalSignal(QByteArray)), this, SLOT(writeDataInternalSlot(QByteArray)));

	m_reconnectTimer = new ThreadSafeTimer(this);//this
	m_reconnectTimer->setInterval(m_reconnectTimeout);

	m_reconnectTimer_t = new ThreadSafeTimer(this);
	m_reconnectTimer_t->setInterval(BASE_TCP_RECONNECT_TIMEOUT);

	m_tcpSocket = new QTcpSocket(this);

	if (isReadRequired) {
		connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
	}
	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketDisplayError(QAbstractSocket::SocketError)));

	connect(m_tcpSocket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));

}

BaseTcpClient::~BaseTcpClient()
{
	log_debug("~BaseTcpClient");
}

bool BaseTcpClient::open(QIODevice::OpenMode mode)
{
	return QIODevice::open(mode);
}

void BaseTcpClient::close()
{
	m_tcpSocket->abort();

	QIODevice::close();
}

qint64 BaseTcpClient::bytesAvailable() const
{
	return m_tcpSocket->bytesAvailable();
}

qint64 BaseTcpClient::readData(char* data, qint64 maxlen)
{
	return m_tcpSocket->read(data, maxlen);
}

qint64 BaseTcpClient::writeData(const char* data, qint64 len)
{
	emit writeDataInternalSignal(QByteArray(data, len));

	return len;
}


void BaseTcpClient::setReconnectInterval(const uint timeMsec)
{
	m_reconnectTimeout = timeMsec;
	m_reconnectTimer->setInterval(m_reconnectTimeout);
}


void BaseTcpClient::connectToHost(const QString& host, const quint32 port)
{
	qRegisterMetaType<quint32>("quint32");
	QMetaObject::invokeMethod(this, "connectToHostInternalSlot", Qt::QueuedConnection,
							  Q_ARG(QString, host),
							  Q_ARG(quint32, port));

	//QxtSignalWaiter::wait(this, SIGNAL(signalConnectedToHost(int)), 2000);
}

void BaseTcpClient::disconnectFromHost()
{
	QMetaObject::invokeMethod( this, "disconnectFromHostInternalSlot", Qt::QueuedConnection );

	Sleeper::msleep(200);
	//disconnectFromHostInternalSlot();
	//QxtSignalWaiter::wait(this, SIGNAL(signalConnectedToHost(int)), 2000);
}

bool BaseTcpClient::isConnected()
{
	log_debug(">>>, m_tcpSocketState = " + QString::number(m_tcpSocket->state()));
	switch(m_tcpSocket->state())
	{
		case QAbstractSocket::ConnectedState:
			emit signalConnectedToHost(TCP::Connected);
			break;
		case QAbstractSocket::UnconnectedState:
			emit signalConnectedToHost(TCP::Disconnected);
			break;
		default:
			emit signalConnectedToHost(TCP::Unknown);
			break;
	}

	return (m_tcpSocket->state() == QAbstractSocket::ConnectedState);
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
	log_debug("Socket connected");
	m_reconnectTimer->stop();
	open(QIODevice::ReadWrite);
	emit signalConnectedToHost(TCP::Connected);
}

void BaseTcpClient::onSocketDisconnected()
{
	log_debug("Socket disconnected");
	QIODevice::close();
	emit signalConnectedToHost(TCP::Disconnected);
}

void BaseTcpClient::onSocketReadyRead()
{
	QByteArray data = m_tcpSocket->readAll();
	//log_debug("in data >>>");
	QSettings settings("./TCP/reconnect.ini", QSettings::IniFormat, this);
	int port = settings.value("FLAKON/port", 1111).toInt();
	m_reconnectTimer_t->start();

	foreach (ITcpReceiver* receiver, m_receiversList) {
		receiver->onDataReceived(QVariant(data));
	}
}

void BaseTcpClient::onSocketDisplayError(QAbstractSocket::SocketError socketError)
{
	switch(socketError)
	{
		case QAbstractSocket::RemoteHostClosedError:
			log_error("Error! Connection with TCP device was lost");
			break;
		case QAbstractSocket::HostNotFoundError:
			log_error("Error! Host not found");
			break;
		case QAbstractSocket::ConnectionRefusedError:
			log_error("Error! Connection was refused");
			break;
		default:
			log_error(QString("Error! Error text: %1").arg(m_tcpSocket->errorString()));
			break;
	}

	QIODevice::close();

	log_info(QString("Host = %1, Port = %2").arg(m_host).arg(m_port));
	if (!m_reconnectTimer->isActive()) {
		m_reconnectTimer->start();
	}
}

void BaseTcpClient::connectToHostInternalSlot(QString host, quint32 port)
{
	log_info(">>>");
	m_host = host;
	m_port = port;

	m_reconnectTimer->stop();

	disconnect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(onSocketReconnectSlot()));
	connect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(onSocketReconnectSlot()));

	disconnect(m_reconnectTimer_t, SIGNAL(timeout()), this, SLOT(onSocketReconnectSlot_t()));
	connect(m_reconnectTimer_t, SIGNAL(timeout()), this, SLOT(onSocketReconnectSlot_t()));

	m_tcpSocket->connectToHost(m_host, m_port);
	if (!m_tcpSocket->waitForConnected(1000)) {
		log_error(QString("Connect failed to %1:%2").arg(host).arg(QString::number(port)));
		emit signalConnectedToHost(TCP::Disconnected);
		m_reconnectTimer->start();
		return;
	}

	log_info(QString("Connectection established to %1:%2").arg(host).arg(QString::number(port)));
}

void BaseTcpClient::disconnectFromHostInternalSlot()
{
	log_debug("Disconnecting...");
	m_reconnectTimer->stop();
	close();
}

void BaseTcpClient::writeDataInternalSlot(const QByteArray& data)
{
	m_tcpSocket->write(data);
}

void BaseTcpClient::onSocketReconnectSlot()
{
	log_info(">>>");
	if(m_tcpSocket->state() != QAbstractSocket::UnconnectedState) {
		log_error("Get UNCONNECTED state");
		return;
	}

	close();

	m_tcpSocket->connectToHost(m_host, m_port);
	m_tcpSocket->waitForConnected(1000);
}

void BaseTcpClient::onSocketReconnectSlot_t()
{
	if( m_port != m_reconnectPort ) {
		return;
	}

	m_reconnectTimer_t->stop();

	log_debug("FUCK STOPPED!!!!");

	close();

	m_tcpSocket->connectToHost(m_host, m_port);
	m_tcpSocket->waitForConnected(1000);

	m_reconnectTimer_t->start();
}
