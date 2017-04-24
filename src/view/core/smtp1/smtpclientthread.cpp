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

int SmtpClientThread::sendMail()
{
	m_messageMutex.lock();
	bool isEmpty = m_messageBuffer.isEmpty();
	m_messageMutex.unlock();

	if(isEmpty || m_elapsedTimer.elapsed() < MESSAGE_TIMEOUT) {
		return -10;
	}

	m_elapsedTimer.restart();

	// Code of sending ...

	SmtpClient smtp("smtp.yandex.ru", 465, SmtpClient::SslConnection);

	// We need to set the username (your email address) and password
	// for smtp authentification.

	smtp.setUser("ZavTestMail@yandex.ru");
	smtp.setPassword("orange");
	//smtp.setAuthMethod(SmtpClient::AuthLogin);

	// Now we create a MimeMessage object. This is the email.

	MimeMessage message;

	EmailAddress sender("ZavTestMail@yandex.ru", "ARM_R");
	message.setSender(&sender);

	{
		//Read Mail adresses
		QString mailFile = QCoreApplication::applicationDirPath();
		mailFile.append("/Tabs/mail.ini");
		QFile addresses(mailFile);
		if(!addresses.open(QIODevice::ReadOnly)) {
			m_elapsedTimer.restart();
			return -1;
		}

		QStringList addrList = QString(addresses.readAll()).split(",");

		if(addrList.isEmpty()) {
			m_elapsedTimer.restart();
			return -1;
		}

		foreach (QString addr, addrList) {
			EmailAddress* to = new EmailAddress(addr, "Zaviruha");
			message.addRecipient(to);
		}
	}


//	EmailAddress to1("tastyreefer@yahoo.com", "Paul");
//	message.addRecipient(&to1);
//	EmailAddress to2("nevaeh.pavlov@gmail.com", "Andrey Pavlov");
//	message.addRecipient(&to2);

	message.setSubject("Zaviruha arm_r message!");

	// Now add some text to the email.
	// First we create a MimeText object.

	MimeText text;

	m_messageMutex.lock();
	text.setText(m_messageBuffer);
	m_messageMutex.unlock();

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

	if( sendMail() != -10 ) {
		sendLocalMail(m_messageBuffer);
		m_messageBuffer.clear();
	}
}

void SmtpClientThread::onStart()
{
}

int SmtpClientThread::sendLocalMail(const QString &messageMail)
{
	// Code of sending ...

	SmtpClient smtp("192.168.55.59", 25, SmtpClient::TcpConnection);

	// We need to set the username (your email address) and password
	// for smtp authentification.

	smtp.setUser("alarm_od@crimea.local");
	smtp.setPassword("nppntt2016");
	smtp.setAuthMethod(SmtpClient::AuthLogin);

	// Now we create a MimeMessage object. This is the email.

	MimeMessage message;

	EmailAddress sender("alarm_od@crimea.local", "ARM_OD");
	message.setSender(&sender);

	{
		//Read Mail adresses
		QString mailFile = QCoreApplication::applicationDirPath();
		mailFile.append("/Tabs/localmail.ini");
		QFile addresses(mailFile);
		if(!addresses.open(QIODevice::ReadOnly)) {
			return -1;
		}

		QStringList addrList = QString(addresses.readAll()).split(",");

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

