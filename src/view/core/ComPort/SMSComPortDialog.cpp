#include "SMSComPortDialog.h"
#include "ui_SMSComPortDialog.h"

SMSComPortDialog::SMSComPortDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SMSComPortDialog),
	m_currentComPortIndex(0)
{
	ui->setupUi(this);
	connect(ui->tbtnOk, SIGNAL(pressed()), this, SLOT(slotAccept()));
	connect(ui->tbtnOk, SIGNAL(pressed()), this, SIGNAL(signalAccept()));
	connect(ui->tbtnCancel, SIGNAL(pressed()), this, SIGNAL(signalClose()));
	connect(ui->tbtnCancel, SIGNAL(pressed()), this, SLOT(slotClose()));

	ui->cb_com_name->setCurrentIndex(m_currentComPortIndex);
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

void SMSComPortDialog::setComPortName(QString name)
{
	m_currentComPort = name;
	m_currentComPortIndex = ui->cb_com_name->findText(name);
	ui->cb_com_name->setCurrentIndex(m_currentComPortIndex);
}

void SMSComPortDialog::setListNumbers(QStringList numbers)
{
	m_listNumbers = numbers;
	QString ns = m_listNumbers.join(",");
	ui->leNumbers->setText(ns);
}

void SMSComPortDialog::showEvent(QShowEvent *event)
{
	ui->cb_com_name->setCurrentIndex(m_currentComPortIndex);
}

void SMSComPortDialog::slotAccept()
{
	m_currentComPort = ui->cb_com_name->currentText();
	m_currentComPortIndex = ui->cb_com_name->currentIndex();
	m_listNumbers = ui->leNumbers->text().split(",");
}

void SMSComPortDialog::slotClose()
{

}
