#include "CorrelationWidget.h"

#include "ui_CorrelationWidget.h"

CorrelationWidget::CorrelationWidget(QWidget *parent, Qt::WFlags flags, QString name, int id):
	QWidget(parent, flags), ui(new Ui::CorrelationWidget)
{
	ui->setupUi(this);

	ui->spectrumWidget->SetZoomOutMaxOnDataSet(true);
	ui->spectrumWidget->SetAlign(2);

	ui->spectrumWidget->SetHorizontalLabel(tr("m"));
}

CorrelationWidget::~CorrelationWidget()
{
}

Q_MG_SpectrumInterface *CorrelationWidget::getGraphicsWidget()
{
	return ui->spectrumWidget;
}
