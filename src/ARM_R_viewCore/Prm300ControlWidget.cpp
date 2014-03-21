#include "Prm300ControlWidget.h"
#include "ui_Prm300ControlWidget.h"

Prm300ControlWidget::Prm300ControlWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Prm300ControlWidget)
{
	ui->setupUi(this);
}

Prm300ControlWidget::~Prm300ControlWidget()
{
	delete ui;
}
