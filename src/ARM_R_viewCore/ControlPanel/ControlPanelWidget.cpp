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
	connect(ui->bandWidthPB, SIGNAL(clicked()), this, SLOT(onSetBandWidthSlot()));
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
