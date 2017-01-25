#include "LocationSetupWidget.h"
#include "ui_LocationSetupWidget.h"

#include "Logger/Logger.h"

#include <QTextBrowser>
#include <QDateTime>

LocationSetupWidget::LocationSetupWidget(int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LocationSetupWidget),
	m_id(id)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);

	this->setWindowTitle(tr("Mode settings System #%1").arg(id));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));

	this->hide();

	ui->toolBox->setCurrentIndex(0);

	connect(&m_devSignalMap, SIGNAL(mapped(int)), this, SLOT(onDeviceEnable(int)));

	connect(ui->systemSettingsWgt, SIGNAL(sendRdsData(QByteArray)), this, SIGNAL(sendRdsData(QByteArray)));

	connect(ui->sbCentralFrequency, SIGNAL(valueChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->sbDuration, SIGNAL(valueChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->cbConvolution, SIGNAL(stateChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->cbHumps, SIGNAL(stateChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->cbDoppler, SIGNAL(stateChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->sbAverageFreq, SIGNAL(valueChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->cbConvolution, SIGNAL(stateChanged(int)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->sbSelStart, SIGNAL(valueChanged(double)), this, SIGNAL(signalSettingsChanged()));
	connect(ui->sbSelEnd, SIGNAL(valueChanged(double)), this, SIGNAL(signalSettingsChanged()));

	connect(ui->pbRestartRds, SIGNAL(clicked(bool)), this, SIGNAL(signalRestartRds()));
	connect(ui->pbGetSystem, SIGNAL(clicked(bool)), this, SIGNAL(signalGetSystem()));

	ui->textEdit->setTextBackgroundColor( Qt::darkGray );
}

LocationSetupWidget::~LocationSetupWidget()
{
	delete ui;
}

void LocationSetupWidget::setLocationData(const RdsProtobuf::ClientMessage_OneShot_Location& data)
{
	blockSignals(true);
	ui->sbDuration->setValue( data.duration() );
	ui->sbCentralFrequency->setValue( data.central_frequency() );
	ui->cbConvolution->setChecked( data.convolution() );
	ui->cbDoppler->setChecked( data.doppler() );
	ui->cbHumps->setChecked( data.hump() );
	ui->sbAverageFreq->setValue( data.averaging_frequency_band() );

	ui->sbSelStart->setValue( data.range().start() );
	ui->sbSelEnd->setValue( data.range().end() );
	blockSignals(false);
}

RdsProtobuf::ClientMessage_OneShot_Location LocationSetupWidget::getLocationData() const
{
	RdsProtobuf::ClientMessage_OneShot_Location location;

	location.set_duration(ui->sbDuration->value());
	location.set_central_frequency( ui->sbCentralFrequency->value() );

	RdsProtobuf::Range* range = location.mutable_range();
	range->set_start( ui->sbSelStart->value() );
	range->set_end( ui->sbSelEnd->value() );

	location.set_convolution( ui->cbConvolution->isChecked() );
	location.set_doppler( ui->cbDoppler->isChecked() );
	location.set_convolution_plot( ui->cbGetConvolution->isChecked() );
	location.set_hump( ui->cbHumps->isChecked() );
	location.set_averaging_frequency_band( ui->sbAverageFreq->value() );

	return location;
}

int LocationSetupWidget::getAnalysisDuration() const
{
	return ui->sbAnalysisDuration->value();
}

int LocationSetupWidget::getRecordDuration() const
{
	return ui->sbRecordDuration->value();
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

void LocationSetupWidget::setChannelEnableState(int dev, int channel, bool state)
{

}

void LocationSetupWidget::setDeviceCommonState(const RdsProtobuf::System_SystemOptions& opt)
{
	ui->systemSettingsWgt->setSettings( opt );
}

void LocationSetupWidget::setTitle(const QString &title)
{
	this->setWindowTitle( tr("Mode settings System #%1 %2").arg(m_id).arg(title) );
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

void LocationSetupWidget::showError(QString str)
{
	ui->textEdit->append( QString("<font color=red>%1 %2</font>").arg(QDateTime::currentDateTime().toString())
																 .arg(str) );
}

void LocationSetupWidget::showConfirm(QString str)
{
//	ui->textEdit->append( QString("<font color=gray>%1 %2</font>").arg(QDateTime::currentDateTime().toString())
//																 .arg(str) );
	if(ui->textEdit->toPlainText().size() > 100000 ) {
		ui->textEdit->clear();
	}
}

void LocationSetupWidget::showLocationError(QString str)
{
	if(str.isEmpty()) {
		return;
	}
	ui->textEdit->append( QString("<font color=yellow>%1 %2</font>").arg(QDateTime::currentDateTime().toString())
																 .arg(str) );
}

