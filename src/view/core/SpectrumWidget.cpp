#include "SpectrumWidget.h"
#include <QDebug>

#include "ui_SpectrumWidget.h"

SpectrumWidget::SpectrumWidget(QWidget *parent, Qt::WFlags flags):
	QWidget(parent, flags), ui(new Ui::SpectrumWidget)
{
	ui->setupUi(this);

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SIGNAL(setPanoramaSignal(bool)));

	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SIGNAL(setAutoSearchSignal(bool)));
	connect(ui->thresholdCB, SIGNAL(toggled(bool)), this, SIGNAL(selectionTypeChangedSignal(bool)));
	connect(ui->getSpectrumCB, SIGNAL(toggled(bool)), this, SIGNAL(requestDataSignal(bool)));

	connect(ui->maximumsCB, SIGNAL(clicked(bool)), this, SIGNAL(setShowPeaksSignal(bool)));
	connect(ui->prmControlCB, SIGNAL(clicked(bool)), this, SIGNAL(setShowControlPRM(bool)));

	ui->sonogramWidget->setVisible(false);

	m_qwtSonogram = new QwtPlot(0);
	ui->sonogramQwtLayout->addWidget( m_qwtSonogram );
}

SpectrumWidget::~SpectrumWidget()
{
}

void SpectrumWidget::setAutoSearch(bool checked)
{
	ui->autosearchCB->setChecked(checked);
}

void SpectrumWidget::setPanorama(bool checked)
{
	ui->panoramaCB->setChecked(checked);
}

void SpectrumWidget::setSpectrumName(const QString &name)
{
	ui->spectrumNameLB->setText(name);
}

QString SpectrumWidget::getSpectrumName() const
{
	return ui->spectrumNameLB->text();
}

Q_MG_SpectrumInterface *SpectrumWidget::getGraphicsWidget()
{
    return ui->graphicsWidget;
}

QwtPlot *SpectrumWidget::getSonogramWidget()
{
	return m_qwtSonogram;
}

void SpectrumWidget::setControlPrmState(bool state)
{
	ui->prmControlCB->setChecked(state);
}

Prm300ControlWidget *SpectrumWidget::getPrm300Widget()
{
	return ui->PrmControlWidget;
}

void SpectrumWidget::sonogramUpdate()
{
	if(ui->sonogramWidget->isVisible()) {
		ui->sonogramWidget->replot();
	}
}

void SpectrumWidget::slotSetEnableSpactrum(bool state)
{
	ui->getSpectrumCB->setChecked(state);
}

void SpectrumWidget::slotEnableKM(bool state)
{
	/// not yet realazed signal
	emit signalEnableKM(state);

	ui->thresholdCB->setChecked(state);
}

/// getting FFT
/*
void SpectrumWidget::slotSetCorrelations(int id, const QVector<QPointF> vecFFT, const bool isComplex)
{
	if(id != m_id)
		return;

	int PointCount = vecFFT.size();
	float* spectrum = new float[PointCount];
	qreal startx = vecFFT.at(0).x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = endx - startx;
	for(int i = 0; i < vecFFT.size(); i++){
		spectrum[i] = vecFFT.at(i).y();
	}

	float maxv = 0.0;
	float minv = 0.0;
	if(isComplex)
	{
		maxv = 1.0;
		minv = 0.0;
	}

	if(!isComplex)
	{
		ui->graphicsWidget->SetAutoscaleY(true);
		ui->graphicsWidget->SetAlign(0);
		ui->graphicsWidget->SetZeroFrequencyHz(vecFFT.at(0).x());
	}
	else
	{
		ui->graphicsWidget->SetAutoscaleY(false);
		ui->graphicsWidget->SetAlign(3);
	}

	if(ui->graphicsWidget->BandWidth() == bandwidth)
	{
		ui->graphicsWidget->PermanentDataSetup(spectrum, NULL /*_spectrum_peak_hold_corr*//*, minv, maxv);
	}

	delete[] spectrum;
}
*/
