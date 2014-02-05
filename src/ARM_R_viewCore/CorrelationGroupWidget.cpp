#include "CorrelationGroupWidget.h"
#include "ui_CorrelationGroupWidget.h"

CorrelationGroupWidget::CorrelationGroupWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CorrelationGroupWidget)
{
	ui->setupUi(this);
}

CorrelationGroupWidget::~CorrelationGroupWidget()
{
	delete ui;
}
