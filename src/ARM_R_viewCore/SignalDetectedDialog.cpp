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

	Qt::WindowFlags flags=windowFlags();
	Qt::WindowFlags helpFlag=Qt::WindowContextHelpButtonHint;
	flags=flags&(~helpFlag);

	setWindowFlags(flags);
	setWindowIcon(QIcon(":images/icons/ListsDialog.png"));
}

SignalDetectedDialog::~SignalDetectedDialog()
{
	delete ui;
}

void SignalDetectedDialog::setFrequency(double freq)
{
	ui->dspFreq->setValue(freq);
}
