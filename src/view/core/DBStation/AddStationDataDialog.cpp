#include <QMessageBox>

#include "StationHelper.h"
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



	connect(ui->pbAccept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui->pbClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));
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
	foreach( QString item, list ) {
		QString value = StationHelper::translateCategory( item );
		ui->cbCategory->addItem( value, item );
	}
}

void AddStationDataDialog::fillSignalType(const QStringList &list)
{
	ui->cbSignalType->addItems(list);
}

void AddStationDataDialog::accept()
{
	StationData data;
	bool doubleOK;

	data.frequency = ui->dsbFrequency->text().toDouble( &doubleOK );
	if( !doubleOK ) {
		QMessageBox::critical( this, tr( "Insertion error" ), tr( "Frequence format is wrong" ) );
		return;
	}

	data.bandwidth = ui->dsbBandwidth->text().toDouble( &doubleOK );
	if( !doubleOK ) {
		QMessageBox::critical( this, tr( "Insertion error" ), tr( "Bandwidth format is wrong" ) );
		return;
	}

	data.stationName = ui->cbStation->currentText();
	data.port = ui->sbPort->value();
	data.category = ui->cbCategory->itemData( ui->cbCategory->currentIndex() ).toString();
	data.signalType = ui->cbSignalType->currentText();

	emit accepted( data );
}

void AddStationDataDialog::clear()
{
	ui->cbStation->setCurrentIndex(DEFAULT_INDEX);
	ui->sbPort->setValue(DEFAULT_PORT);
	ui->cbCategory->setCurrentIndex(DEFAULT_INDEX);
	ui->dsbFrequency->setValue(DEFAULT_FREQUENCY);
	ui->dsbBandwidth->setValue(DEFAULT_BANDWIDTH);
	ui->cbSignalType->setCurrentIndex(DEFAULT_INDEX);

}
