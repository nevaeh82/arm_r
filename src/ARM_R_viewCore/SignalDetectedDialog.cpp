#include "SignalDetectedDialog.h"
#include "ui_SignalDetectedDialog.h"

SignalDetectedDialog::SignalDetectedDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SignalDetectedDialog)
{
	ui->setupUi(this);
	connect (ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SIGNAL(signalContinueSpectrum()));
	connect (ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SIGNAL(signalStopSpectrum()));
}

SignalDetectedDialog::~SignalDetectedDialog()
{
	delete ui;
}

void SignalDetectedDialog::setFrequency(double freq)
{
	ui->dspFreq->setValue(freq);
}
