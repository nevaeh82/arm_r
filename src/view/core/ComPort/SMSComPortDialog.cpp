#include "SMSComPortDialog.h"
#include "ui_SMSComPortDialog.h"

SMSComPortDialog::SMSComPortDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SMSComPortDialog)
{
	ui->setupUi(this);
	connect(ui->tbtnOk, SIGNAL(pressed()), this, SLOT(slotAccept()));
	connect(ui->tbtnCancel, SIGNAL(pressed()), this, SIGNAL(signalClose));
	connect(ui->tbtnOk, SIGNAL(pressed()), this, SIGNAL(signalAccept()));
}

SMSComPortDialog::~SMSComPortDialog()
{
	delete ui;
}

void SMSComPortDialog::fillComPorts(QList<QString> list)
{
	ui->cb_com_name->clear();
	QList<QString>::iterator i;
	for(i = list.begin(); i != list.end(); ++i)
	{
		ui->cb_com_name->addItem(*i);
	}
}

QStringList SMSComPortDialog::getNumbers()
{
	return m_listNumbers;
}

QString SMSComPortDialog::getComPortName()
{
	return m_currentComPort;
}

void SMSComPortDialog::slotAccept()
{
	m_currentComPort = ui->cb_com_name->currentText();
	m_listNumbers = ui->leNumbers->text().split(",");
}
