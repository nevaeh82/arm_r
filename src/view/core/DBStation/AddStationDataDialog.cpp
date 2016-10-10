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

AddStationDataDialog::AddStationDataDialog(QString station, double freq, double bandwidth, QWidget *parent) : QDialog(parent), ui(new Ui::AddStationDataDialog)
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

    ui->dsbFrequency->setValue(freq);
    ui->dsbBandwidth->setValue(bandwidth);

    m_inStationName = station;
}

AddStationDataDialog::~AddStationDataDialog()
{
	delete ui;
}

void AddStationDataDialog::fillStation(const QStringList &list)
{
	ui->cbStation->addItems(list);

    //ui->cbStation->ge
    int id = ui->cbStation->findText(m_inStationName, static_cast<Qt::MatchFlags>(Qt::MatchContains));
//    int id = -1;
//    for(int i = 0; i<ui->cbStation->count(); i++) {
//        QString t1 =
//        if( ui->cbStation->itemText(i) == station ) {
//            id = i;
//            break;
//        }
//    }

    if( id >= 0 ) {
        ui->cbStation->setCurrentIndex( id );
    }
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
    unsigned short set = 0;
    if(ui->cbSet->isChecked())
    {
        set = 1;
    }
    data.checked = set;

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
    ui->cbSet->setChecked(false);

}
