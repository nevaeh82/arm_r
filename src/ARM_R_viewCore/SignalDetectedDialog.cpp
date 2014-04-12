#include "SignalDetectedDialog.h"
#include "ui_SignalDetectedDialog.h"

SignalDetectedDialog::SignalDetectedDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SignalDetectedDialog)
{
	ui->setupUi(this);
	QPushButton* btnOk = ui->buttonBox->button(QDialogButtonBox::Ok);
	btnOk->setText(tr("Ok"));

	QPushButton* btnCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
	btnCancel->setText(tr("Cancel"));
}

SignalDetectedDialog::~SignalDetectedDialog()
{
	delete ui;
}

void SignalDetectedDialog::setFrequency(double freq)
{
	ui->dspFreq->setValue(freq);
}
