#include "SignalDetectedDialog.h"
#include "ui_SignalDetectedDialog.h"

SignalDetectedDialog::SignalDetectedDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SignalDetectedDialog)
{
	ui->setupUi(this);
}

SignalDetectedDialog::~SignalDetectedDialog()
{
	delete ui;
}

void SignalDetectedDialog::setFrequency(double freq)
{
	ui->dspFreq->setValue(freq);
}
