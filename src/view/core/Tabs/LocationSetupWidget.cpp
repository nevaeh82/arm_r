#include "LocationSetupWidget.h"
#include "ui_LocationSetupWidget.h"

#include "Logger/Logger.h"

LocationSetupWidget::LocationSetupWidget(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LocationSetupWidget)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);

	this->setWindowTitle(tr("Mode settings"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));

	this->hide();

	//connect( ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(onSendSignal()) );
	connect(ui->pbUpdate, SIGNAL(clicked(bool)), this, SIGNAL(onSignalUpdate()));
	connect(ui->pbSet, SIGNAL(clicked(bool)), this, SIGNAL(onSignalSet()));

	connect(ui->pbUpdateDet, SIGNAL(clicked(bool)), this, SIGNAL(onSignalUpdateDet()));
	connect(ui->pbSetDet, SIGNAL(clicked(bool)), this, SIGNAL(onSignalSetDet()));

	connect(ui->pbUpdateCor, SIGNAL(clicked(bool)), this, SIGNAL(onSignalUpdateCor()));
	connect(ui->pbSetCor, SIGNAL(clicked(bool)), this, SIGNAL(onSignalSetCor()));

	connect(ui->pbSetAnalysis, SIGNAL(clicked(bool)), this, SIGNAL(onSignalSetAnalysis()));
	connect(ui->pbUpdateAnalysis, SIGNAL(clicked(bool)), this, SIGNAL(onSignalUpdateAnalysis()));

	//List widgets
	connect(ui->pbAddRangeDet, SIGNAL(clicked(bool)), this, SLOT(onAddRangeDet()));
	connect(ui->pbRemoveRangeDet, SIGNAL(clicked(bool)), this, SLOT(onRemoveRangeDet()));

	connect(ui->pbAddRangeCorr, SIGNAL(clicked(bool)), this, SLOT(onAddRangeCor()));
	connect(ui->pbRemoveRangeCorr, SIGNAL(clicked(bool)), this, SLOT(onRemoveRangeCor()));

	ui->toolBox->setCurrentIndex(0);

	connect(&m_devSignalMap, SIGNAL(mapped(int)), this, SLOT(onDeviceEnable(int)));

	connect(ui->systemSettingsWgt, SIGNAL(sendRdsData(QByteArray)), this, SIGNAL(sendRdsData(QByteArray)));
}

LocationSetupWidget::~LocationSetupWidget()
{
	delete ui;
}

void LocationSetupWidget::setLocationData(const RdsProtobuf::ClientMessage_OneShot_Location& data)
{
	ui->sbDuration->setValue( data.duration() );
	ui->sbCentralFrequency->setValue( data.central_frequency() );
	ui->cbConvolution->setChecked( data.convolution() );
	ui->cbDoppler->setChecked( data.doppler() );
	ui->sbAverageFreq->setValue( data.averaging_frequency_band() );
	ui->cbConvolution->setChecked( data.convolution_plot() );

	ui->sbRange->setValue( data.range().start() );
	ui->sbShift->setValue( data.range().end() );
}

RdsProtobuf::ClientMessage_OneShot_Location LocationSetupWidget::getLocationData() const
{
	RdsProtobuf::ClientMessage_OneShot_Location location;

	location.set_duration(ui->sbDuration->value());
	location.set_central_frequency( ui->sbCentralFrequency->value() );
	location.set_convolution( ui->cbConvolution->isChecked() );
	location.set_doppler( ui->cbDoppler->isChecked() );
	location.set_convolution_plot( ui->cbGetConvolution->isChecked() );
	location.set_averaging_frequency_band( ui->sbAverageFreq->value() );

	RdsProtobuf::Range* range = location.mutable_range();
	range->set_start( ui->sbRange->value() );
	range->set_end( ui->sbShift->value() );

	return location;
}

int LocationSetupWidget::getAnalysisDuration() const
{
	return ui->sbAnalysisDuration->value();
}

void LocationSetupWidget::setPlatformList(const QStringList& list)
{
	quint32 platformIndex = 0;

	foreach (QString platformTtile, list) {
		if(!m_cbDevMap.contains(platformIndex)) {
			addDeviceEnableControl(platformTtile, platformIndex);
		} else {
			m_cbDevMapTitle.value(platformIndex)->setText(platformTtile);
		}

		platformIndex++;
	}
}

void LocationSetupWidget::setDeviceEnableState(int dev, bool state)
{
	if(!m_cbDevMap.contains(dev)) {
		addDeviceEnableControl(QString("Platform_%1").arg(dev), dev);
	}

	m_cbDevMap.value(dev)->setChecked(state);
}

void LocationSetupWidget::onSpectrumLocationSelection(float bandwidth, float shift)
{
	//In khz
	ui->sbRange->setValue( bandwidth );
	ui->sbShift->setValue(shift);
}

void LocationSetupWidget::onSpectrumAnalysisSelection(double start, double end)
{
	ui->sbAnalysisSelectStartMhz->setValue(start);
	ui->sbAnalysisSelectEndMhz->setValue(end);
}

void LocationSetupWidget::setDeviceCommonState(const RdsProtobuf::System_SystemOptions& opt)
{
	ui->systemSettingsWgt->setSettings( opt );
}

void LocationSetupWidget::addDeviceEnableControl(QString platformTtile, int device) {
	int platformIndex = device;
	QString title = QString("%1_%2").arg(platformIndex).arg(platformTtile);
	QLabel* lbl = new QLabel(title, this);
	ui->gridLayoutPlatforms->addWidget( lbl, platformIndex, 0, 1, 1);
	QCheckBox* cb = new QCheckBox(tr("Enable"));
	ui->gridLayoutPlatforms->addWidget( cb, platformIndex, 1, 1, 1);

	m_devSignalMap.setMapping(cb, platformIndex);
	connect(cb, SIGNAL(clicked(bool)), &m_devSignalMap, SLOT(map()));

	m_cbDevMap.insert( platformIndex, cb );
	m_cbDevMapTitle.insert( platformIndex, lbl );
}

// List widgets
void LocationSetupWidget::onAddRangeDet()
{
	int min = ui->sbMinRangeDet->value();
	int max = ui->sbMaxRangeDet->value();
	QString range = QString("%1-%2").arg(min).arg(max);

	if( (min < max) && !ui->listRangesDet->findItems(range, Qt::MatchExactly).size() ) {
		ui->listRangesDet->addItem( range );
	}
}

void LocationSetupWidget::onRemoveRangeDet()
{
	if( ui->listRangesDet->selectedItems().size() ) {
		delete ui->listRangesDet->takeItem( ui->listRangesDet->currentRow() );
	}
}

void LocationSetupWidget::onAddRangeCor()
{
	int min = ui->sbMinRangeCorr->value();
	int max = ui->sbMaxRangeCorr->value();
	QString range = QString("%1-%2").arg(min).arg(max);

	if( (min < max) && !ui->listRangesDet->findItems(range, Qt::MatchExactly).size()) {
		ui->listRangesCor->addItem( range );
	}

}

void LocationSetupWidget::onRemoveRangeCor()
{
	if( ui->listRangesCor->selectedItems().size() ) {
		delete ui->listRangesCor->takeItem( ui->listRangesCor->currentRow() );
	}
}

void LocationSetupWidget::onDeviceEnable(int dev)
{
	int k = 0;

	QCheckBox* cb = m_cbDevMap.value( dev, NULL );
	if( !cb ) {
		return;
	}

	emit onSignalDeviceEnable(dev, cb->isChecked());

	Q_UNUSED(k)
}
