#include "smtpclientthread.h"

#include "mimetext.h"

#include <QFile>
#include <QCoreApplication>

#define MESSAGE_TIMEOUT 250000
#define MESSAGE_TIMER 100000

SmtpClientThread::SmtpClientThread(QObject *parent)
{
	m_isOn = false;

	m_timer = new QTimer;
	m_elapsedTimer.start();
	m_timer->setInterval(MESSAGE_TIMER);
	m_timer->start();

	connect(m_timer, SIGNAL(timeout()), this, SLOT(letsSend()), Qt::QueuedConnection);
	connect(this, SIGNAL(onLetsSend()), this, SLOT(letsSendInternal()));
}

SmtpClientThread::~SmtpClientThread()
{

}

void SmtpClientThread::sendMessage(const QString &message)
{
	m_messageMutex.lock();

	m_messageBuffer.append("\r\n");
	m_messageBuffer.append(message);

	m_messageMutex.unlock();
}

void SmtpClientThread::sendDoplerMessage(const QString &message)
{
	m_messageMutex.lock();

	m_messageDoplerBuffer.append("\r\n");
	m_messageDoplerBuffer.append(message);

	m_messageMutex.unlock();
}

void SmtpClientThread::setLocalMailSettings(const MailSettings &settings)
{
	m_mailMutex.lock();
	m_localMailSettings = settings;
	m_mailMutex.unlock();
}

void SmtpClientThread::setRemoteMailSettings(const MailSettings &settings)
{
	m_mailMutex.lock();
	m_remoteMailSettings = settings;
	m_mailMutex.unlock();
}

void SmtpClientThread::setLocalMailList(const QStringList &mailList)
{
	m_mailMutex.lock();
	m_localMailList = mailList;
	m_mailMutex.unlock();
}

void SmtpClientThread::setRemoteMailList(const QStringList &mailList)
{
	m_mailMutex.lock();
	m_remoteMailList = mailList;
	m_mailMutex.unlock();
}

int SmtpClientThread::sendMail(const QString &messageMail, bool isPermanentSend)
{
	bool isEmpty = messageMail.isEmpty();

	if(!isPermanentSend) {
		if(m_elapsedTimer.elapsed() < MESSAGE_TIMEOUT) {
			return -10;
		}
	}

	if(isEmpty) { //Check for permanent send
		return -11;
	}

	m_elapsedTimer.restart();

	log_debug("SENDING EMAIL!@!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

	// Code of sending ...

	//SmtpClient smtp("smtp.yandex.ru", 465, SmtpClient::SslConnection);
	m_mailMutex.lock();
	SmtpClient smtp(m_remoteMailSettings.host,
					m_remoteMailSettings.port,
					SmtpClient::ConnectionType(m_remoteMailSettings.connetionType));

	// We need to set the username (your email address) and password
	// for smtp authentification.

//	smtp.setUser("ZavTestMail@yandex.ru");
//	smtp.setPassword("orange");
	//smtp.setAuthMethod(SmtpClient::AuthPlain);
	smtp.setUser(m_remoteMailSettings.userName);
	smtp.setPassword(m_remoteMailSettings.passwd);
	smtp.setAuthMethod(SmtpClient::AuthMethod(m_remoteMailSettings.authType));

	m_mailMutex.unlock();

	// Now we create a MimeMessage object. This is the email.

	MimeMessage message;

	EmailAddress sender("ZavTestMail@yandex.ru", "ARM_R");
	message.setSender(&sender);

	{
		m_mailMutex.lock();
		QStringList addrList = m_remoteMailList;
		m_mailMutex.unlock();

		if(addrList.isEmpty()) {
			m_elapsedTimer.restart();
			return -1;
		}

		foreach (QString addr, addrList) {
			EmailAddress* to = new EmailAddress(addr, "Zaviruha");
			message.addRecipient(to);
		}
	}

	message.setSubject("Zaviruha arm_r message!");

	// Now add some text to the email.
	// First we create a MimeText object.

	MimeText text;

	text.setText(messageMail);

	// Now add it to the mail

	message.addPart(&text);

	// Now we can send the mail

	if (!smtp.connectToHost()) {
		qDebug() << "Failed to connect to host!" << endl;
		m_elapsedTimer.restart();
		return -1;
	}

	if (!smtp.login()) {
		qDebug() << "Failed to login!" << endl;
		m_elapsedTimer.restart();
		return -2;
	}

	if (!smtp.sendMail(message)) {
		qDebug() << "Failed to send mail!" << endl;
		m_elapsedTimer.restart();
		return -3;
	}

	smtp.quit();


	QList<EmailAddress*> addrList = message.getRecipients(MimeMessage::To);
	foreach (EmailAddress* addr, addrList) {
		delete addr;
	}
	// ....

	m_elapsedTimer.restart();
    return 1;
}

void SmtpClientThread::letsSend()
{
	emit onLetsSend();
}

void SmtpClientThread::letsSendInternal()
{
	if(m_isOn) {
		return;
	}

	QString msg1;
	QString msg2;

	m_messageMutex.lock();
	msg1 = m_messageBuffer;
	msg2 = m_messageDoplerBuffer;
	m_messageMutex.unlock();

	if( sendMail(msg1) != -10 ) {
		sendLocalMail(msg1);

		sendMail(msg2, true);
		sendLocalMail(msg2);

		m_messageBuffer.clear();
		m_messageDoplerBuffer.clear();
	}
}

void SmtpClientThread::onStart()
{
}

int SmtpClientThread::sendLocalMail(const QString &messageMail)
{
	// Code of sending ...

	//SmtpClient smtp("192.168.55.59", 25, SmtpClient::TcpConnection);

	bool isEmpty = messageMail.isEmpty();

	if(isEmpty) {
		return -1;
	}


	m_mailMutex.lock();
	SmtpClient smtp(m_localMailSettings.host,
					m_localMailSettings.port,
					SmtpClient::ConnectionType(m_localMailSettings.connetionType));

	// We need to set the username (your email address) and password
	// for smtp authentification.

//	smtp.setUser("alarm_od@crimea.local");
//	smtp.setPassword("nppntt2016");
//	smtp.setAuthMethod(SmtpClient::AuthLogin);

	smtp.setUser(m_localMailSettings.userName);
	smtp.setPassword(m_localMailSettings.passwd);
	smtp.setAuthMethod(SmtpClient::AuthMethod(m_localMailSettings.authType));

	m_mailMutex.unlock();

	// Now we create a MimeMessage object. This is the email.

	MimeMessage message;

	EmailAddress sender("alarm_od@crimea.local", "ARM_OD");
	message.setSender(&sender);

	{
		//Read Mail adresses
//		QString mailFile = QCoreApplication::applicationDirPath();
//		mailFile.append("/Tabs/localmail.ini");
//		QFile addresses(mailFile);
//		if(!addresses.open(QIODevice::ReadOnly)) {
//			return -1;
//		}

//		QStringList addrList = QString(addresses.readAll()).split(",");

		m_mailMutex.lock();
		QStringList addrList = m_localMailList;
		m_mailMutex.unlock();

		if(addrList.isEmpty()) {
			return -1;
		}

		foreach (QString addr, addrList) {
			EmailAddress* to = new EmailAddress(addr, "Zaviruha");
			message.addRecipient(to);
		}
	}

	message.setSubject("Zaviruha arm_r message!");

	// Now add some text to the email.
	// First we create a MimeText object.

	MimeText text;
	text.setText(messageMail);

	// Now add it to the mail

	message.addPart(&text);

	// Now we can send the mail

	if (!smtp.connectToHost()) {
		qDebug() << "Failed to connect to host local!" << endl;
		return -1;
	}

	if (!smtp.login()) {
		qDebug() << "Failed to login local!" << endl;
		return -2;
	}

	if (!smtp.sendMail(message)) {
		qDebug() << "Failed to send mail local!" << endl;
		return -3;
	}

	smtp.quit();

	QList<EmailAddress*> addrList = message.getRecipients(MimeMessage::To);
	foreach (EmailAddress* addr, addrList) {
		delete addr;
	}
	// ....

	return 1;
}

