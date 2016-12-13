#include "CorrelationWidget.h"

#include "ui_CorrelationWidget.h"

#include "Logger/Logger.h"

CorrelationWidget::CorrelationWidget(QWidget *parent, Qt::WFlags flags):
    BaseCorrelationWidget(parent, flags), ui(new Ui::CorrelationWidget)
{
	ui->setupUi(this);

	ui->spectrumWidget->SetZoomOutMaxOnDataSet(true);
	ui->spectrumWidget->SetAlign(2);

	ui->spectrumWidget->SetHorizontalLabel(tr("m"));

	ui->spectrumWidget->SetLabel(0, "sddsfew");

	ui->spectrumWidget->SetShowOptionsForStarndartLabels("", true);
}

CorrelationWidget::~CorrelationWidget()
{
	//log_debug("<<<<<<");
}

void CorrelationWidget::reset()
{

}

void CorrelationWidget::setDataSetup(double bandwidth, int m_pointCount, float *spectrum, float *spectrum_peak_hold, float sko)
{
    Q_UNUSED(sko);

    float maxv = 2.0;
    float minv = 0.0;

    ui->spectrumWidget->SetAutoscaleY(false);
    ui->spectrumWidget->SetAlign(3);
    ui->spectrumWidget->SetHorizontalLabel(tr("m"));
    ui->spectrumWidget->Setup(true,bandwidth,tr("Level"), spectrum, m_pointCount, spectrum_peak_hold, m_pointCount,false, false, minv, maxv);
    ui->spectrumWidget->SetSpectrumVisible(1, true);

    // ui->spectrumWidget->ZoomOutFull();
}

void CorrelationWidget::permanentSetup(float *spectrum, float *spectrum_peak_hold, float sko)
{
    Q_UNUSED(sko);
    ui->spectrumWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, 0.0, 2.0);
    //ui->spectrumWidget->SetLabel(30, "sddsfew", "iii");

    //ui->spectrumWidget->ZoomOutFull();
}

void CorrelationWidget::setLabel(const QString &label)
{
    ui->spectrumWidget->ClearAllLabels();
    ui->spectrumWidget->SetLabel(0, label, "hh", true );
}
