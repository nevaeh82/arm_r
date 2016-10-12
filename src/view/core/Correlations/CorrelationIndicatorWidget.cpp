#include "CorrelationIndicatorWidget.h"

#include "ui_CorrelationIndicatorWidget.h"

CorrelationIndicatorWidget::CorrelationIndicatorWidget(QWidget *parent, Qt::WFlags flags):
    BaseCorrelationWidget(parent, flags),
    ui(new Ui::CorrelationIndicatorWidget),
    m_limit(5000)
{
	ui->setupUi(this);

    m_red = new QPixmap(":/images/bullet_red.png");
    m_green = new QPixmap(":/images/bullet_green.png");

    ui->labelIndicator->setFixedSize(16, 16);
    ui->labelIndicator->setPixmap(m_red->scaled(16,16,Qt::KeepAspectRatio));

    //this->setFixedSize(50, 50);
}

CorrelationIndicatorWidget::~CorrelationIndicatorWidget()
{
}

void CorrelationIndicatorWidget::reset()
{

}

void CorrelationIndicatorWidget::setDataSetup(double bandwidth, int m_pointCount, float *spectrum, float *spectrum_peak_hold, float sko)
{

}

void CorrelationIndicatorWidget::permanentSetup(float *spectrum, float *spectrum_peak_hold, float sko)
{
    float skoM = sko* 3e8;
    if( skoM > m_limit ) {
        ui->labelIndicator->setFixedSize(16, 16);
        ui->labelIndicator->setPixmap(m_red->scaled(16,16,Qt::KeepAspectRatio));
    } else {
        ui->labelIndicator->setFixedSize(16, 16);
        ui->labelIndicator->setPixmap(m_green->scaled(16,16,Qt::KeepAspectRatio));
    }
}

void CorrelationIndicatorWidget::setLabel(const QString &label)
{
    ui->labelTitle->setText(label);
}

