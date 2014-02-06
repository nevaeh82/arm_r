#include "ControlPanelWidget.h"
#include "ui_ControlPanel.h"

ControlPanelWidget::ControlPanelWidget(QWidget* parent):
	QWidget(parent),
	ui(new Ui::ControlPanelWidget)
{
	ui->setupUi(this);

	//connect(this, SIGNAL(onAutoSearchCheched(bool));

	//connect(ui->	)
}

ControlPanelWidget::~ControlPanelWidget()
{
}
