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

	m_plot = new QCustomPlot(this);
	ui->gridLayout->addWidget(m_plot, 1, 1);
	m_plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//m_plot->plotLayout()->setMargins(QMargins(0, 0, 0, 0));
	//m_plot->setContentsMargins(0, 0, 0, 0);

	initCustomPlot(m_plot);

	m_plot->setVisible(false);
	m_plot->setMinimumWidth(100);
	m_plot->setMaximumWidth(300);

	m_plot->addGraph();
	m_plot->graph(0)->setPen(QPen(Qt::yellow));

	ui->gridLayout->setRowStretch(0, 2);
}

CorrelationWidget::~CorrelationWidget()
{
	//log_debug("<<<<<<");
}

void CorrelationWidget::initCustomPlot(QCustomPlot* plot)
{
	plot->setBackground(QBrush(Qt::black));
	plot->xAxis->setTickPen(QPen(QColor(155,255,255)));
	plot->xAxis->setLabelColor(QColor(155,255,255));
	plot->xAxis->setTickLabelColor(QColor(155,255,255));

	plot->yAxis->setTickPen(QPen(QColor(155,255,255)));
	plot->yAxis->setBasePen(QPen(QColor(155,255,255)));
	plot->yAxis->setTickLabelColor(QColor(155,255,255));
	plot->yAxis->setTickLabels(true);


	//plot->axisRect()->setAutoMargins(QCP::msNone);
	//plot->axisRect()->setMargins( QMargins(0, 0, 0, 0) );

	QPen gridPen;
	gridPen.setStyle(Qt::DotLine);
	gridPen.setColor(QColor(0,60,0));

	plot->xAxis->grid()->setPen(gridPen);
	plot->yAxis->grid()->setPen(gridPen);

	plot->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
//	plot->xAxis->setRange(0, 20);
//	plot->yAxis->setRange(0, 10);
//	plot->xAxis->setAutoTicks(true);
//	plot->xAxis->setAutoTickStep(true);
//	plot->xAxis->setTickLabels(true);

	//plot->xAxis->setTickStep(2);
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

    ui->spectrumWidget->ZoomOutFull();
}

void CorrelationWidget::permanentSetup(float *spectrum, float *spectrum_peak_hold, float sko)
{
	Q_UNUSED(sko);
	ui->spectrumWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, 0.0, 2.0);
	//ui->spectrumWidget->SetLabel(30, "sddsfew", "iii");

    ui->spectrumWidget->ZoomOutFull();
}

void CorrelationWidget::setupDoppler(const QQueue<double> &data)
{
	if(!m_plot->isVisible()) {
		return;
	}

	QVector<double> x;
	QVector<double> y;

	double rangeMin = -10;
	double rangeMax = 10;
	double yVal;

	for(int i = 0; i<data.size(); i++) {
		x.append(i);
		y.append(data.at(i));
		yVal = data.at(i);

		if(rangeMin > yVal) {
			rangeMin = yVal;
		}

		if(rangeMax < yVal) {
			rangeMax = yVal;
		}
	}

	m_plot->yAxis->setRange(rangeMin, rangeMax);
	m_plot->xAxis->setRange(0, data.size());

	m_plot->graph(0)->setData(x, y);

	m_plot->replot();
}

void CorrelationWidget::setLabel(const QString &label)
{
//	ui->spectrumWidget->ClearAllLabels();
//	ui->spectrumWidget->SetLabel(0, label, "hh", true );
	ui->label->setText(label);
}

void CorrelationWidget::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void CorrelationWidget::setDopplerVisible(bool visible)
{
	m_plot->setVisible(visible);
}

