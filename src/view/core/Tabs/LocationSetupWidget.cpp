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

	//List widgets
	connect(ui->pbAddRangeDet, SIGNAL(clicked(bool)), this, SLOT(onAddRangeDet()));
	connect(ui->pbRemoveRangeDet, SIGNAL(clicked(bool)), this, SLOT(onRemoveRangeDet()));

	connect(ui->pbAddRangeCorr, SIGNAL(clicked(bool)), this, SLOT(onAddRangeCor()));
	connect(ui->pbRemoveRangeCorr, SIGNAL(clicked(bool)), this, SLOT(onRemoveRangeCor()));

    ui->toolBox->setCurrentIndex(0);
}

LocationSetupWidget::~LocationSetupWidget()
{
	delete ui;
}

void LocationSetupWidget::setLocationData(const RdsProtobuf::Location &data)
{
	RdsProtobuf::Location_LocationOptions opt = data.options();

	ui->sbDuration->setValue( opt.duration() );
	ui->sbCentralFrequency->setValue( opt.central_frequency() );
	ui->cbConvolution->setChecked( opt.convolution() );
	ui->cbDoppler->setChecked( opt.doppler() );
	ui->sbAverageFreq->setValue( opt.averaging_frequency_band() );
	ui->sbFreqTuning->setValue( opt.frequency_tuning_mode() );

    log_debug(QString("Range: %1").arg(opt.filter().range()));
    log_debug(QString("Shift: %1").arg(opt.filter().shift()));

	ui->sbRange->setValue( opt.filter().range() );
	ui->sbShift->setValue( opt.filter().shift() );
}

RdsProtobuf::Location LocationSetupWidget::getLocationData() const
{
	RdsProtobuf::Location location;
	RdsProtobuf::Location_LocationOptions* opt = location.mutable_options();

	opt->set_duration(ui->sbDuration->value());
	opt->set_central_frequency( ui->sbCentralFrequency->value() );
	opt->set_convolution( ui->cbConvolution->isChecked() );
	opt->set_doppler( ui->cbDoppler->isChecked() );
	opt->set_averaging_frequency_band( ui->sbAverageFreq->value() );
	opt->set_frequency_tuning_mode( ui->sbFreqTuning->value() );
	opt->mutable_filter()->set_range( ui->sbRange->value() );
	opt->mutable_filter()->set_shift( ui->sbShift->value() );

	return location;
}

void LocationSetupWidget::setDetectorData(const RdsProtobuf::Detector &data)
{
	RdsProtobuf::Detector_DetectorOptions opt = data.options();

	ui->sbDurationDet->setValue( opt.duration() );
	ui->cbConvolutionDet->setChecked( opt.convolution() );
	ui->cbDopplerDet->setChecked( opt.doppler() );
	ui->sbLoopsDet->setValue( opt.loops_number() );
	ui->sbAverageFreqDet->setValue( opt.averaging_frequency_band() );
    ui->sbThresholdDet->setValue( opt.band_threshold()*1000 );

	ui->listRangesDet->clear();
	for(int i = 0; i < opt.range_size(); i++) {
		ui->listRangesDet->addItem( QString("%1-%2").
									arg( opt.range(i).start() ).
									arg( opt.range(i).end() ) );
	}
}

RdsProtobuf::Detector LocationSetupWidget::getDetectorData() const
{
	RdsProtobuf::Detector detector;
	RdsProtobuf::Detector_DetectorOptions* opt = detector.mutable_options();

	opt->set_duration(ui->sbDurationDet->value());
	opt->set_convolution( ui->cbConvolutionDet->isChecked() );
	opt->set_doppler( ui->cbDopplerDet->isChecked() );
	opt->set_loops_number( ui->sbLoopsDet->value() );
	opt->set_averaging_frequency_band( ui->sbAverageFreqDet->value() );
    opt->set_band_threshold( (double)ui->sbThresholdDet->value() / 1000 );

	for( int i = 0; i < ui->listRangesDet->count(); i++ ) {
		RdsProtobuf::Range* range = opt->add_range();
		QString line = ui->listRangesDet->item(i)->text();
		QStringList ranges = line.split("-");
		range->set_start( ranges.at(0).toDouble() );
		range->set_end( ranges.at(1).toDouble() );
	}

	if( opt->range().empty() ) {
		RdsProtobuf::Range* range = opt->add_range();
		range->set_start( 100 );
		range->set_end( 200 );
	}

	return detector;
}

void LocationSetupWidget::setCorrectionData(const RdsProtobuf::Correction &data)
{
	RdsProtobuf::Correction_CorrectionOptions opt = data.options();

	ui->sbDurationCor->setValue( opt.duration() );
	ui->sbLoopsCor->setValue( opt.loops_number() );

	ui->listRangesCor->clear();
	for(int i = 0; i < opt.range_size(); i++) {
		ui->listRangesCor->addItem( QString("%1-%2").
									arg( opt.range(i).start() ).
									arg( opt.range(i).end() ) );
	}
}

RdsProtobuf::Correction LocationSetupWidget::getCorrectionData() const
{
	RdsProtobuf::Correction correction;
	RdsProtobuf::Correction_CorrectionOptions* opt = correction.mutable_options();

	opt->set_duration(ui->sbDurationCor->value());
	opt->set_loops_number( ui->sbLoopsCor->value() );

	for( int i = 0; i < ui->listRangesCor->count(); i++ ) {
		RdsProtobuf::Range* range = opt->add_range();
		QString line = ui->listRangesCor->item(i)->text();
		QStringList ranges = line.split("-");
		range->set_start( ranges.at(0).toDouble() );
		range->set_end( ranges.at(1).toDouble() );
	}

	if( opt->range().empty() ) {
		RdsProtobuf::Range* range = opt->add_range();
		range->set_start( 100 );
		range->set_end( 200 );
	}

	return correction;
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
