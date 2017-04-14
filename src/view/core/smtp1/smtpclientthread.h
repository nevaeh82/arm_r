#pragma once

#include <QObject>
#include <QtNetwork/QSslSocket>
#include <QTime>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>

#include "Logger/Logger.h"

#include "smtpclient.h"

class SmtpClientThread : public QObject
{

Q_OBJECT

public:
	SmtpClientThread(QObject* parent);
	~SmtpClientThread();

	void sendMessage(const QString& message);

private:
	QString m_messageBuffer;
	QTime m_elapsedTimer;
	QTimer m_timer;

	QMutex m_messageMutex;

private slots:
	int sendMail();
};
