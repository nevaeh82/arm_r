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
	connect(ui->pbDown1MHz, SIGNAL(clicked()), this, SIGNAL(signalDown1Mhz()));
	connect(ui->pbDown10MHz, SIGNAL(clicked()), this, SIGNAL(signalDown10Mhz()));
	connect(ui->pbDown100MHz, SIGNAL(clicked()), this, SIGNAL(signalDown100Mhz()));
	connect(ui->pbUp1MHz, SIGNAL(clicked()), this, SIGNAL(signalUp1Mhz()));
	connect(ui->pbUp10MHz, SIGNAL(clicked()), this, SIGNAL(signalUp10Mhz()));
	connect(ui->pbUp100MHz, SIGNAL(clicked()), this, SIGNAL(signalUp100Mhz()));

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

void ControlPanelWidget::slotChangeCommonFreq(int value)
{
	ui->commonFreqSB->setValue(value);
}
