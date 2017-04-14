#include "smtpclientthread.h"

#include "mimetext.h"

#define MESSAGE_TIMEOUT 120000
#define MESSAGE_TIMER 60000

SmtpClientThread::SmtpClientThread(QObject *parent)
{
	m_elapsedTimer.start();
	m_timer.setSingleShot(true);
	m_timer.setInterval(MESSAGE_TIMER);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(sendMail()));
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

	sendMail();
}

int SmtpClientThread::sendMail()
{
	m_messageMutex.lock();
	bool isEmpty = m_messageBuffer.isEmpty();
	m_messageMutex.unlock();

	if(isEmpty || m_elapsedTimer.elapsed() < MESSAGE_TIMEOUT) {
		m_timer.start();
		return -1;
	}

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

	EmailAddress to("tastyreefer@gmail.com", "Paul");
	message.addRecipient(&to);
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
		m_timer.start();
		return -1;
	}

	if (!smtp.login()) {
		qDebug() << "Failed to login!" << endl;
		return -2;
	}

	if (!smtp.sendMail(message)) {
		qDebug() << "Failed to send mail!" << endl;
		m_timer.start();
		return -3;
	}

	smtp.quit();

	// ....

	m_elapsedTimer.restart();
	m_messageBuffer.clear();

	m_timer.start();
}
