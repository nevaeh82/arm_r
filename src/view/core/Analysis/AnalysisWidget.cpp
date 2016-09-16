#include "AnalysisWidget.h"

#include "ui_AnalysisWidget.h"

#include <QLabel>

AnalysisWidget::AnalysisWidget(int type, QWidget *parent, Qt::WFlags flags):
	QWidget(parent, flags),
	ui(new Ui::AnalysisWidget),
	m_type(type)
{
	ui->setupUi(this);

	ui->spectrumWidget->SetZoomOutMaxOnDataSet(true);
	ui->spectrumWidget->SetAlign(2);

	ui->spectrumWidget->SetHorizontalLabel(tr("m"));

	ui->title->setText(QString("Title %1").arg(type));
}

AnalysisWidget::~AnalysisWidget()
{
}

Q_MG_SpectrumInterface *AnalysisWidget::getGraphicsWidget()
{
	return ui->spectrumWidget;
}
