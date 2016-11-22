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

//void LocationSetupWidget::setDetectorData(const RdsProtobuf::Detector &data)
//{
//	RdsProtobuf::Detector_DetectorOptions opt = data.options();

//	ui->sbDurationDet->setValue( opt.duration() );
//	ui->cbConvolutionDet->setChecked( opt.convolution() );
//	ui->cbDopplerDet->setChecked( opt.doppler() );
//	ui->sbLoopsDet->setValue( opt.loops_number() );
//	ui->sbAverageFreqDet->setValue( opt.averaging_frequency_band() );
//    ui->sbThresholdDet->setValue( opt.band_threshold()*1000 );

//	ui->listRangesDet->clear();
//	for(int i = 0; i < opt.range_size(); i++) {
//		ui->listRangesDet->addItem( QString("%1-%2").
//									arg( opt.range(i).start() ).
//									arg( opt.range(i).end() ) );
//	}
//}

//RdsProtobuf::Detector LocationSetupWidget::getDetectorData() const
//{
//	RdsProtobuf::Detector detector;
//	RdsProtobuf::Detector_DetectorOptions* opt = detector.mutable_options();

//	opt->set_duration(ui->sbDurationDet->value());
//	opt->set_convolution( ui->cbConvolutionDet->isChecked() );
//	opt->set_doppler( ui->cbDopplerDet->isChecked() );
//	opt->set_loops_number( ui->sbLoopsDet->value() );
//	opt->set_averaging_frequency_band( ui->sbAverageFreqDet->value() );
//    opt->set_band_threshold( (double)ui->sbThresholdDet->value() / 1000 );

//	for( int i = 0; i < ui->listRangesDet->count(); i++ ) {
//		RdsProtobuf::Range* range = opt->add_range();
//		QString line = ui->listRangesDet->item(i)->text();
//		QStringList ranges = line.split("-");
//		range->set_start( ranges.at(0).toDouble() );
//		range->set_end( ranges.at(1).toDouble() );
//	}

//	if( opt->range().empty() ) {
//		RdsProtobuf::Range* range = opt->add_range();
//		range->set_start( 100 );
//		range->set_end( 200 );
//	}

//	return detector;
//}

//void LocationSetupWidget::setCorrectionData(const RdsProtobuf::Correction &data)
//{
//	RdsProtobuf::Correction_CorrectionOptions opt = data.options();

//	ui->sbDurationCor->setValue( opt.duration() );
//	ui->sbLoopsCor->setValue( opt.loops_number() );

//	ui->listRangesCor->clear();
//	for(int i = 0; i < opt.range_size(); i++) {
//		ui->listRangesCor->addItem( QString("%1-%2").
//									arg( opt.range(i).start() ).
//									arg( opt.range(i).end() ) );
//	}
//}

//RdsProtobuf::Correction LocationSetupWidget::getCorrectionData() const
//{
//	RdsProtobuf::Correction correction;
//	RdsProtobuf::Correction_CorrectionOptions* opt = correction.mutable_options();

//	opt->set_duration(ui->sbDurationCor->value());
//	opt->set_loops_number( ui->sbLoopsCor->value() );

//	for( int i = 0; i < ui->listRangesCor->count(); i++ ) {
//		RdsProtobuf::Range* range = opt->add_range();
//		QString line = ui->listRangesCor->item(i)->text();
//		QStringList ranges = line.split("-");
//		range->set_start( ranges.at(0).toDouble() );
//		range->set_end( ranges.at(1).toDouble() );
//	}

//	if( opt->range().empty() ) {
//		RdsProtobuf::Range* range = opt->add_range();
//		range->set_start( 100 );
//		range->set_end( 200 );
//	}

//	return correction;
//}

//void LocationSetupWidget::setAnalysisData(const RdsProtobuf::Analysis &data)
//{
//	RdsProtobuf::Analysis_AnalysisOptions opt = data.options();

//	ui->sbAnalysisDetector->setValue( opt.detector_index() );
//	ui->sbAnalysisDuration->setValue( opt.duration() );
//	ui->sbAnalysisCentralFreq->setValue( opt.central_frequency() );

//	ui->sbAnalysisSelectStartMs->setValue( opt.selected_area().time_start() );
//	ui->sbAnalysisSelectEndMs->setValue( opt.selected_area().time_end() );
//	ui->sbAnalysisSelectStartMhz->setValue( opt.selected_area().freq_start() );
//	ui->sbAnalysisSelectEndMhz->setValue( opt.selected_area().freq_end() );

//	ui->sbAnalysisZoomStartMs->setValue( opt.zoomed_area().time_start() );
//	ui->sbAnalysisZoomEndMs->setValue( opt.zoomed_area().time_end() );
//	ui->sbAnalysisZoomStartMhz->setValue( opt.zoomed_area().freq_start() );
//	ui->sbAnalysisZoomEndMhz->setValue( opt.zoomed_area().freq_end() );

//	ui->cbAnalysisEnable->setChecked( opt.analysis() );
//}

//RdsProtobuf::Analysis LocationSetupWidget::getAnalysisData() const
//{
//	RdsProtobuf::Analysis analysis;
//	RdsProtobuf::Analysis_AnalysisOptions* opt = analysis.mutable_options();

//	opt->set_detector_index( ui->sbAnalysisDetector->value() );
//	opt->set_duration( ui->sbAnalysisDuration->value() );
//	opt->set_central_frequency( ui->sbAnalysisCentralFreq->value() );

//	RdsProtobuf::TimeFreqArea* selected = opt->mutable_selected_area();
//	selected->set_time_start( ui->sbAnalysisSelectStartMs->value() );
//	selected->set_time_end( ui->sbAnalysisSelectEndMs->value() );
//	selected->set_freq_start( ui->sbAnalysisSelectStartMhz->value() );
//	selected->set_freq_end( ui->sbAnalysisSelectEndMhz->value() );

//	RdsProtobuf::TimeFreqArea* zoomed = opt->mutable_zoomed_area();
//	zoomed->set_time_start( ui->sbAnalysisZoomStartMs->value() );
//	zoomed->set_time_end( ui->sbAnalysisZoomEndMs->value() );
//	zoomed->set_freq_start( ui->sbAnalysisZoomStartMhz->value() );
//	zoomed->set_freq_end( ui->sbAnalysisZoomEndMhz->value() );

//	opt->set_analysis( ui->cbAnalysisEnable->isChecked() );

//	return analysis;
//}

int LocationSetupWidget::getAnalysisChannel() const
{
	return ui->sbAnalysisDetector->value();
}

void LocationSetupWidget::setAnalysisChannelCount(int cnt)
{
	ui->sbAnalysisDetector->setMinimum(0);
	ui->sbAnalysisDetector->setMaximum(cnt-1);
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

void LocationSetupWidget::setWorkMode(int mode)
{
	ui->toolBox->setCurrentIndex(mode);
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
