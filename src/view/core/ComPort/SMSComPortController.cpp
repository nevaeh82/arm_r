#include "SMSComPortController.h"

SMSComPortController::SMSComPortController(QObject *parent) : QObject(parent)
{
	m_view = NULL;
	m_comport = NULL;
	m_listNumbersCurrentIndex = 0;
	m_smsDialog = new SMSComPortDialog(0);
//	m_timer = new QTimer();

	appendView(m_smsDialog);
	connect(m_smsDialog, SIGNAL(signalClose()), this, SLOT(slotCloseUi()));
	connect(m_smsDialog, SIGNAL(signalAccept()), this, SLOT(slotUpdateFromUi()));
	connect(this, SIGNAL(signalSendNewMessage()), this, SLOT(slotSendSMByTimer()));

	//connect(m_timer, SIGNAL(timeout()), this, SLOT(slotSendSMS()));
}

SMSComPortController::~SMSComPortController()
{
	delete m_smsDialog;
}

void SMSComPortController::init()
{
	m_comport = new ComPort(0);
	connect(m_comport, SIGNAL(signalSent()), this, SLOT(slotSendSMS()));
	QList<QString> list = m_comport->getAllPorts();
	m_view->fillComPorts(list);
	m_comport->setComPort(m_view->getComPortName());
	if(m_comport->isValidComPort())
	{
		QString command = (tr("AT+CMGF=1 \r"));
		m_comport->writeCommand(command.toAscii().data());
	}
}

void SMSComPortController::appendView(SMSComPortDialog *view)
{
	m_view = view;
	init();
}

void SMSComPortController::showDialog()
{
	m_view->show();
}

QStringList SMSComPortController::getNumbers()
{
	return m_view->getNumbers();
}

void SMSComPortController::sendMessage(QString msg)
{
	SMSMessage sms;
	sms.currentIndexNumber = 0;
	sms.msg = msg;
	m_listMessages.append(sms);
	m_listNumbers = m_view->getNumbers();
	emit signalSendNewMessage();
}

void SMSComPortController::send2()
{
	QTimer::singleShot(1000, this, SLOT(slotSendSMS2()));
}

void SMSComPortController::slotCloseUi()
{
	m_view->close();
}

void SMSComPortController::slotUpdateFromUi()
{
	m_view->close();
	delete m_comport;
	init();
	sendMessage("Freq = 1999");
}

void SMSComPortController::slotSendSMS()
{
	if(m_listMessages.isEmpty())
	{
		return;
	}
	if(m_comport->isValidComPort())
	{
		QString command;
		SMSMessage &sms = m_listMessages.first();
		if(sms.currentIndexNumber >= m_listNumbers.count())
		{
			command = (tr("AT+CMSD=0 \r"));
			m_comport->writeCommand(command.toAscii().data());
			m_listMessages.removeFirst();
			emit signalSendNewMessage();
			return;
		}

		int currentIndex = sms.currentIndexNumber;
		QString No= m_listNumbers.at(currentIndex);

		if(currentIndex == 0)
		{
			command = (tr("AT+CMSW=\"") + No + tr("\" \r ") + sms.msg + tr("\x1A"));
			m_comport->writeCommand(command.toAscii().data());
//			command = (tr("AT+CMSS=0, \"") + No + tr("\" \r"));
//			m_comport->writeCommand(command.toAscii().data());
			send2();
		}
		else
		{
			command = (tr("AT+CMSS=0, \"") + No + tr("\" \r"));
			m_comport->writeCommand(command.toAscii().data());
			sms.currentIndexNumber++;
			slotSendSMByTimer();
		}
	}
}

void SMSComPortController::slotSendSMS2()
{
	QString command;
	SMSMessage &sms = m_listMessages.first();
	QString No= m_listNumbers.at(sms.currentIndexNumber++);

	command = (tr("AT+CMSS=0, \"") + No + tr("\" \r"));
	m_comport->writeCommand(command.toAscii().data());
	slotSendSMByTimer();
}

void SMSComPortController::slotSendSMByTimer()
{
	QTimer::singleShot(1000, this, SLOT(slotSendSMS()));
}
