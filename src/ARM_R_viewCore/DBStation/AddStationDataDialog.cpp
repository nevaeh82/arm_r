#include "AddStationDataDialog.h"
#include "ui_AddStationDataDialog.h"

AddStationDataDialog::AddStationDataDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddStationDataDialog)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);
	setWindowIcon(QIcon(":/images/icons/AddStationDataDialog.png"));



	connect(ui->pbAccept, SIGNAL(clicked()), this, SLOT(slotAccept()));
	connect(ui->pbClear, SIGNAL(clicked()), this, SLOT(slotClear()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(slotClose()));
}

AddStationDataDialog::~AddStationDataDialog()
{
	delete ui;
}

void AddStationDataDialog::fillStation(const QStringList &list)
{
	ui->cbStation->addItems(list);
}

void AddStationDataDialog::fillCategory(const QStringList &list)
{
	ui->cbCategory->addItems(list);
}

void AddStationDataDialog::fillSignalType(const QStringList &list)
{
	ui->cbSignalType->addItems(list);
}

void AddStationDataDialog::slotAccept()
{
	QStringList list;
	list.append(ui->cbStation->currentText());
	list.append(QString::number(ui->sbPort->value()));
	list.append(ui->cbCategory->currentText());
	list.append(ui->dsbFrequency->text());
	list.append(ui->dsbBandwidth->text());
	list.append(ui->cbSignalType->currentText());
	emit signalAccept(list);
	emit signalUpdateList();

}

void AddStationDataDialog::slotClear()
{
	ui->cbStation->setCurrentIndex(DEFAULT_INDEX);
	ui->sbPort->setValue(DEFAULT_PORT);
	ui->cbCategory->setCurrentIndex(DEFAULT_INDEX);
	ui->dsbFrequency->setValue(DEFAULT_FREQUENCY);
	ui->dsbBandwidth->setValue(DEFAULT_BANDWIDTH);
	ui->cbSignalType->setCurrentIndex(DEFAULT_INDEX);

}

void AddStationDataDialog::slotClose()
{
	this->close();
	emit signalUpdateList();
}
