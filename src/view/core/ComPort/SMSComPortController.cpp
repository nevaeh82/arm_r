#include "SMSComPortController.h"

SMSComPortController::SMSComPortController(QObject *parent) : QObject(parent)
{
	m_view = NULL;
	m_comport = NULL;
	m_smsDialog = new SMSComPortDialog(0);
	appendView(m_smsDialog);
	connect(m_smsDialog, SIGNAL(signalClose()), this, SLOT(slotCloseUi()));
	connect(m_smsDialog, SIGNAL(signalAccept()), this, SLOT(slotUpdateFromUi()));

}

SMSComPortController::~SMSComPortController()
{
	delete m_smsDialog;
}

void SMSComPortController::init()
{
	m_comport = new ComPort(0);
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
	QStringList listNumbers = m_view->getNumbers();
	if(m_comport->isValidComPort())
	{
		for(QStringList::iterator it = listNumbers.begin(); it != listNumbers.end(); ++it)
		{
			QString No = (*it);
			QString command = (tr("AT+CMGS=\"") + No + tr("\" \r ") + msg + tr("\x1A"));
			m_comport->writeCommand(command.toAscii().data());
		}
	}
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
