#include "SMSComPortController.h"

SMSComPortController::SMSComPortController(QObject *parent) : QObject(parent)
{
	m_view = NULL;
	m_comport = new ComPort(0);;
	m_listNumbersCurrentIndex = 0;
    //m_smsDialog = new SMSComPortDialog(0);
	m_comPortName = "";
//	m_timer = new QTimer();

    //appendView(m_smsDialog)
	connect(this, SIGNAL(signalSendNewMessage()), this, SLOT(slotSendSMByTimer()));

	//connect(m_timer, SIGNAL(timeout()), this, SLOT(slotSendSMS()));
}

SMSComPortController::~SMSComPortController()
{
	saveSettings();
	delete m_smsDialog;
}

void SMSComPortController::init()
{
	delete m_comport;
	m_comport = new ComPort(0);
	connect(m_comport, SIGNAL(signalSent()), this, SLOT(slotSendSMByTimer()));
	QList<QString> list = m_comport->getAllPorts();
	m_view->fillComPorts(list);
	m_comPortName = m_view->getComPortName();
	m_comport->setComPort(m_comPortName);
	if(m_comport->isValidComPort())
	{
		QString command = (tr("AT+CMGF=1 \r"));
		m_comport->writeCommand(command.toAscii().data());
	}
}

void SMSComPortController::appendView(SMSComPortDialog *view)
{
	m_view = view;
    m_smsDialog = view;

    connect(m_smsDialog, SIGNAL(signalClose()), this, SLOT(slotCloseUi()));
    connect(m_smsDialog, SIGNAL(signalAccept()), this, SLOT(slotUpdateFromUi()));

    readSettings();

	init();
}

void SMSComPortController::showDialog()
{
    //m_view->show();
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

void SMSComPortController::onStart()
{

}

void SMSComPortController::readSettings()
{
	QSettings aSettings("./Tabs/sms.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	m_comPortName=aSettings.value("comPortName").toString();
	m_listNumbers = aSettings.value("numbers").toString().split(",");

	QList<QString> list = m_comport->getAllPorts();
	m_view->fillComPorts(list);

	m_view->setComPortName(m_comPortName);
	m_view->setListNumbers(m_listNumbers);

}

void SMSComPortController::saveSettings()
{
	QSettings aSettings("./Tabs/sms.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	aSettings.setValue("comPortName", m_view->getComPortName());
	aSettings.setValue("numbers", m_view->getNumbers().join(","));
}

void SMSComPortController::slotCloseUi()
{
//	m_view->close();
//	saveSettings();
}

void SMSComPortController::slotUpdateFromUi()
{
    //m_view->close();
	init();
	saveSettings();
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
			m_listMessages.removeFirst();
			emit signalSendNewMessage();
			return;
		}

		int currentIndex = sms.currentIndexNumber++;
		QString No = m_listNumbers.at(currentIndex);

        command = (tr("AT+CMGS=\"") + No.trimmed() + tr("\" \r ") + sms.msg + tr("\x1A"));
		m_comport->writeCommand(command.toAscii().data());
		slotSendSMByTimer();
	}
}


void SMSComPortController::slotSendSMByTimer()
{
	QTimer::singleShot(5000, this, SLOT(slotSendSMS()));
}
