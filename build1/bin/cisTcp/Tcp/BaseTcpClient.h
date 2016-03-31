#ifndef BASETCPCLIENT_H
#define BASETCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QVariant>

#include "Templates/BaseSubject.h"
#include "Interfaces/Tcp/ITcpReceiver.h"
#include "Interfaces/Tcp/ITcpClient.h"

#include "Logger/Logger.h"

#include "ThreadTimer/ThreadSafeTimer.h"

namespace TCP {
typedef enum TcpConnectionState
{
	Disconnected = 0,
	Connected = 1,
	Unknown = -1
}TcpConnectionState;
}

class BaseTcpClient : public QIODevice, public ITcpClient, public BaseSubject<ITcpReceiver>
{
	Q_OBJECT

protected:
	QString m_host;
	quint32 m_port;

	int m_reconnectPort;

	QTcpSocket* m_tcpSocket;
	ThreadSafeTimer* m_reconnectTimer;
	ThreadSafeTimer* m_reconnectTimer_t;

	quint32 m_reconnectTimeout;

public:
	explicit BaseTcpClient(const bool isReadRequired = true, QObject* parent = NULL);
	virtual ~BaseTcpClient();

	virtual void connectToHost(const QString& host, const quint32 port);
	virtual void disconnectFromHost();
	virtual bool isConnected();

	virtual QString getHost();
	virtual QObject* asQObject();
	virtual void setReconnectInterval(const uint timeMsec);

	virtual bool open(OpenMode mode);
	virtual void close();
	virtual qint64 bytesAvailable() const;

protected:
	virtual qint64 readData(char* data, qint64 maxlen);
	virtual qint64 writeData(const char* data, qint64 len);

private slots:
	void onSocketConnected();
	void onSocketDisconnected();
	void onSocketReadyRead();
	void onSocketDisplayError(QAbstractSocket::SocketError socketError);

	void connectToHostInternalSlot(QString host, quint32 port);
	void disconnectFromHostInternalSlot();
	void writeDataInternalSlot(const QByteArray& data);

	void onSocketReconnectSlot();
	void onSocketReconnectSlot_t();

signals:
	void writeDataInternalSignal(const QByteArray& data);

	void signalConnectedToHost(int status);
};

#endif // BASETCPCLIENT_H
