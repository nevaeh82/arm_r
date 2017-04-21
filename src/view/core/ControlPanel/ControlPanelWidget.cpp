#include "ControlPanelWidget.h"
#include "ui_ControlPanel.h"

#include <QSound>

#define BORDER_NORMAL "rgb(49, 49, 49);"
#define BORDER_ERROR "rgb(255, 0, 0);"
#define BORDER_WARNING "rgb(255, 255, 0);"

ControlPanelWidget::ControlPanelWidget(QWidget* parent):
	QWidget(parent),
	ui(new Ui::ControlPanelWidget)
{
	ui->setupUi(this);

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SLOT(onSetPanorama(bool)));
	connect(ui->commonFreqSB, SIGNAL(valueChanged(int)), this, SLOT(onSetCommonFrequencySlot()));
	connect(ui->cbMode, SIGNAL(activated(int)), this, SLOT(slotChangeMode(int)));

	connect(ui->pbDown1MHz, SIGNAL(clicked()), this, SIGNAL(signalDown1Mhz()));
	connect(ui->pbDown10MHz, SIGNAL(clicked()), this, SIGNAL(signalDown10Mhz()));
	connect(ui->pbDown100MHz, SIGNAL(clicked()), this, SIGNAL(signalDown100Mhz()));
	connect(ui->pbUp1MHz, SIGNAL(clicked()), this, SIGNAL(signalUp1Mhz()));
	connect(ui->pbUp10MHz, SIGNAL(clicked()), this, SIGNAL(signalUp10Mhz()));
	connect(ui->pbUp100MHz, SIGNAL(clicked()), this, SIGNAL(signalUp100Mhz()));
    connect(ui->startFreqSB, SIGNAL(valueChanged(int)), this, SIGNAL(signalSetStartFreq(int)));
    connect(ui->endFreqSB, SIGNAL(valueChanged(int)), this, SIGNAL(signalSetEndFreq(int)));

	connect(ui->pbSpectrums, SIGNAL(clicked(bool)), this, SIGNAL(signalReceiveSpectrums(bool)));
	connect(ui->pbConvolution, SIGNAL(clicked(bool)), this, SIGNAL(signalConvolution(bool)));
	connect(ui->pbConvolution, SIGNAL(clicked(bool)), this, SIGNAL(signalDoppler(bool)));
	connect(ui->pbConvolution, SIGNAL(clicked(bool)), ui->pbDoppler, SLOT(setChecked(bool)));
	connect(ui->pbDoppler, SIGNAL(clicked(bool)), this, SIGNAL(signalDoppler(bool)));
	connect(ui->pbHumps, SIGNAL(clicked(bool)), this, SIGNAL(signalHumps(bool)));
	connect(ui->pbHumps, SIGNAL(clicked(bool)), this, SIGNAL(autoSearchCheckedSignal(bool)));

	connect(ui->cbSleepMode, SIGNAL(clicked(bool)), this, SIGNAL(onSleepMode(bool)));

	connect(ui->pbMerge, SIGNAL(clicked(bool)), this, SIGNAL(signalSystemMerge(bool)));

	m_pmRoundRed = new QPixmap(":/images/bullet_red.png");
	m_pmRoundGreen = new QPixmap(":/images/bullet_green.png");

	ui->correlationStatusLabelActive->setFixedSize(16, 16);
	ui->correlationStatusLabelActive->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));

	ui->solverQualityLB->setFixedSize(16, 16);
	ui->solverQualityLB->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));

//	ui->lblError->setVisible(false);
	//ui->lblWarning->setVisible(false);

	m_borderStyle = QString("QWidget#ControlPanelWidget{ \
									background-color: rgb(230, 230, 230); \
									border: 2px solid %1 \
									border-radius: 1; \
									}" );

	setStyleSheet(m_borderStyle.arg(BORDER_NORMAL));

	alarmAimVisible(false);

	m_alarmTimer = new QTimer(this);
	connect(m_alarmTimer, SIGNAL(timeout()), this, SLOT(slotAlarmTimeout()));
}

ControlPanelWidget::~ControlPanelWidget()
{
	delete m_pmRoundRed;
	delete m_pmRoundGreen;
}

void ControlPanelWidget::onSetCommonFrequencySlot()
{
	emit commonFreqChangedSignal(ui->commonFreqSB->value());
}

void ControlPanelWidget::onSetPanorama(bool on)
{
    emit signalPanorama(on);
	emit onPanoramaEnable(on, ui->startFreqSB->value(), ui->endFreqSB->value());
}

void ControlPanelWidget::showError(QString str)
{
	if(str.isEmpty()) {
//		ui->lblError->setVisible(false);
//		ui->lblError->clear();
		setStyleSheet(m_borderStyle.arg(BORDER_NORMAL));
	} else {
//		ui->lblError->setVisible(true);
//		ui->lblError->setText(str);
		setStyleSheet(m_borderStyle.arg(BORDER_ERROR));
	}
}

void ControlPanelWidget::showConfirm(QString)
{
	//ui->lblError->setVisible(false);
	//ui->lblError->clear();
	setStyleSheet(m_borderStyle.arg(BORDER_NORMAL));
}

void ControlPanelWidget::showLocationError(QString str)
{
	if(str.isEmpty()) {
//		ui->lblWarning->setVisible(false);
//		ui->lblWarning->clear();
		setStyleSheet(m_borderStyle.arg(BORDER_NORMAL));
	} else {
//		ui->lblWarning->setVisible(true);
//		ui->lblWarning->setText(str);
		setStyleSheet(m_borderStyle.arg(BORDER_WARNING));
	}
}

void ControlPanelWidget::slotAlarmTimeout()
{
	alarmAimVisible(false);
	m_alarmTimer->stop();
}

void ControlPanelWidget::applyManualMode()
{
	ui->cbMode->setCurrentIndex(0);
	slotChangeMode(0);
}

void ControlPanelWidget::slotChangeMode(int index)
{
    emit signalChangeMode(index);
	switch(index)
	{
	case 0:
		emit signalManualMode();
		break;
	case 1:
		emit signalScanMode(ui->startFreqSB->value(), ui->endFreqSB->value());
		break;
	case 2:
		emit signalCheckMode();
		break;
	case 3:
		emit signalViewMode();
		break;
	default:
		break;
	}

}

void ControlPanelWidget::changeCorrelationStatus(QString correlationValue)
{
	ui->correlationStatusLabelValue->setText(correlationValue);
}

void ControlPanelWidget::changeCorrelationStatusActive(const bool isActive)
{
	if(isActive) {
		ui->correlationStatusLabelActive->setPixmap(m_pmRoundGreen->scaled(16,16,Qt::KeepAspectRatio));
	}
	else {
		ui->correlationStatusLabelActive->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
	}
}

void ControlPanelWidget::changeQualityStatus(const int status, bool isMoving, float freq)
{
	if(status) {
		ui->solverQualityLB->setPixmap(m_pmRoundGreen->scaled(16,16,Qt::KeepAspectRatio));
	}
	else {
		ui->solverQualityLB->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
	}

	if(isMoving) {
		alarmAimVisible(true);
		ui->labelAlarm->setText(tr("Moving aim founded!!! Freq: %1").arg(freq));
		if(!ui->sounOffCB->isChecked()) {
			QSound::play("./audio/aim.wav");
		}
		m_alarmTimer->start(10000);
	} else {
		alarmAimVisible(false);
	}
}

void ControlPanelWidget::alarmAimVisible(bool val)
{
	ui->labelAlarm->setVisible(val);
	ui->sounOffCB->setVisible(val);
}

void ControlPanelWidget::setStartFreq(int val)
{
	ui->startFreqSB->setValue(val);
}

void ControlPanelWidget::setfinishFreq(int val)
{
	ui->endFreqSB->setValue(val);
}

void ControlPanelWidget::setSystemMerge(bool state)
{
	ui->pbMerge->setChecked(state);
}

void ControlPanelWidget::setMode(int val)
{
	ui->cbMode->setCurrentIndex(val);
	slotChangeMode(val);
}

void ControlPanelWidget::setPanorama(bool state)
{
	ui->panoramaCB->setChecked(state);
	onSetPanorama(state);
}

void ControlPanelWidget::setReceiveSpectrums(bool val)
{
	ui->pbSpectrums->setChecked(val);
	emit signalReceiveSpectrums(val);
}

void ControlPanelWidget::slotChangeCommonFreq(double value)
{
	ui->commonFreqSB->blockSignals(true);
	ui->commonFreqSB->setValue(value);
	ui->commonFreqSB->blockSignals(false);
}

void ControlPanelWidget::setLocationSettings(RdsProtobuf::ClientMessage_OneShot_Location msg)
{
	blockSignals(true);
	ui->pbConvolution->setChecked( msg.convolution() );
	ui->pbDoppler->setChecked( msg.doppler() );
	ui->pbHumps->setChecked( msg.hump() );

	ui->commonFreqSB->setValue( msg.central_frequency() );

	blockSignals(false);

	emit autoSearchCheckedSignal(msg.hump());
}

void ControlPanelWidget::setCentralFreqValue(double freq)
{
	ui->lblCurFreq->setText(QString::number(freq) + tr(" mhz"));
}

bool ControlPanelWidget::getParoramaState(double &start, double &end)
{
	start = ui->startFreqSB->value();
	end = ui->endFreqSB->value();

	return ui->panoramaCB->isChecked();
}

void ControlPanelWidget::updatePanorama(const bool enable, const double &start, const double &end)
{
	blockSignals(true);
	ui->panoramaCB->setChecked(enable);
	ui->startFreqSB->setValue(start);
	ui->endFreqSB->setValue(end);
	blockSignals(false);
}

void ControlPanelWidget::setSolverConnectState(bool b)
{
	if(b) {
		ui->lblSolverState->setPixmap(m_pmRoundGreen->scaled(16,16,Qt::KeepAspectRatio));
	}
	else {
		ui->lblSolverState->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
	}
}

bool ControlPanelWidget::sleepMode() const
{
	return ui->cbSleepMode->isChecked();
}

void ControlPanelWidget::setSleepMode(bool val)
{
    ui->cbSleepMode->setChecked(val);
	emit onSleepMode(val);
}

int ControlPanelWidget::getFreqSBValue()
{
    return ui->commonFreqSB->value();
}

void ControlPanelWidget::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
