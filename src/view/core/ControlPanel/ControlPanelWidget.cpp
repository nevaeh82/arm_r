#include "ControlPanelWidget.h"
#include "ui_ControlPanel.h"

ControlPanelWidget::ControlPanelWidget(QWidget* parent):
	QWidget(parent),
	ui(new Ui::ControlPanelWidget)
{
	ui->setupUi(this);

	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SIGNAL(autoSearchCheckedSignal(bool)));
	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SIGNAL(panoramaCheckedSignal(bool)));
	connect(ui->commonFrequencyPB, SIGNAL(clicked()), this, SLOT(onSetCommonFrequencySlot()));
	connect(ui->panoramaPB, SIGNAL(clicked()), this, SLOT(onSetBandWidthSlot()));
    connect(ui->cbMode, SIGNAL(activated(int)), this, SLOT(slotChangeMode(int)));
	connect(ui->cbMode, SIGNAL(activated(int)), this, SIGNAL(signalSetMode(int)));
	connect(ui->pbDown1MHz, SIGNAL(clicked()), this, SIGNAL(signalDown1Mhz()));
	connect(ui->pbDown10MHz, SIGNAL(clicked()), this, SIGNAL(signalDown10Mhz()));
	connect(ui->pbDown100MHz, SIGNAL(clicked()), this, SIGNAL(signalDown100Mhz()));
	connect(ui->pbUp1MHz, SIGNAL(clicked()), this, SIGNAL(signalUp1Mhz()));
	connect(ui->pbUp10MHz, SIGNAL(clicked()), this, SIGNAL(signalUp10Mhz()));
	connect(ui->pbUp100MHz, SIGNAL(clicked()), this, SIGNAL(signalUp100Mhz()));

	//connect(ui->workMode, SIGNAL(activated(int)), this, SLOT(slotOnWorkMode()));
	connect(ui->workMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotOnWorkMode()));
    connect(ui->workModeOn, SIGNAL(clicked(bool)), this, SLOT(slotTurnWorkMode(bool)));

	m_pmRoundRed = new QPixmap(":/images/bullet_red.png");
	m_pmRoundGreen = new QPixmap(":/images/bullet_green.png");

	ui->correlationStatusLabelActive->setFixedSize(16, 16);
	ui->correlationStatusLabelActive->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));

	ui->solverQualityLB->setFixedSize(16, 16);
	ui->solverQualityLB->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
}

ControlPanelWidget::~ControlPanelWidget()
{
}

void ControlPanelWidget::onSetCommonFrequencySlot()
{
	emit commonFreqChangedSignal(ui->commonFreqSB->value());
}

void ControlPanelWidget::onSetBandWidthSlot()
{
    ui->panoramaCB->setChecked(false);
    emit panoramaCheckedSignal(false);
    qApp->processEvents();
    emit bandwidthChangedSignal(ui->startFreqSB->value(), ui->endFreqSB->value());
}

void ControlPanelWidget::slotChangeMode(int index)
{
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

void ControlPanelWidget::slotOnWorkMode()
{
    int mode = ui->workMode->currentIndex();
    ui->workModeOn->setChecked(true);
    emit signalWorkMode(mode, true);
}

void ControlPanelWidget::slotTurnWorkMode(bool val)
{
    int mode = ui->workMode->currentIndex();
    emit signalWorkMode(mode, val);
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

void ControlPanelWidget::changeQualityStatus(const int status)
{
	if(1 == status) {
		ui->solverQualityLB->setPixmap(m_pmRoundGreen->scaled(16,16,Qt::KeepAspectRatio));
	}
	else {
		ui->correlationStatusLabelActive->setPixmap(m_pmRoundRed->scaled(16,16,Qt::KeepAspectRatio));
    }
}

void ControlPanelWidget::onSetMode(int modeId, bool isOn)
{
    ui->workMode->setCurrentIndex( modeId );
    ui->workModeOn->setChecked( isOn );
}

void ControlPanelWidget::slotChangeCommonFreq(int value)
{
	ui->commonFreqSB->setValue(value);        
}
