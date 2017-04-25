#pragma once

#include <QObject>
#include <QtNetwork/QSslSocket>
#include <QTime>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include "Tabs/EmailSettings.h"

#include "Logger/Logger.h"

#include "smtpclient.h"

class SmtpClientThread : public QObject
{

Q_OBJECT

public:
	SmtpClientThread(QObject* parent);
	~SmtpClientThread();

	void sendMessage(const QString& message);

	void setLocalMailSettings(const MailSettings& settings);
	void setRemoteMailSettings(const MailSettings& settings);

	void setLocalMailList(const QStringList& mailList);
	void setRemoteMailList(const QStringList& mailList);

private:
	QString m_messageBuffer;
	QTime m_elapsedTimer;
	QTimer* m_timer;

	QMutex m_messageMutex;
	QMutex m_mailMutex;

	bool m_isOn;

	QStringList m_remoteMailList;
	QStringList m_localMailList;

	MailSettings m_localMailSettings;
	MailSettings m_remoteMailSettings;

	int sendLocalMail(const QString& messageMail);

private slots:
	int sendMail();
	void letsSend();
	void onStart();
	void letsSendInternal();

signals:
	void onLetsSend();
};
