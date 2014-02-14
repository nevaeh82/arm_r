#include "SpectrumWidget.h"
#include <QDebug>

#include "ui_SpectrumWidget.h"

#define TO_MHZ	1000000

SpectrumWidget::SpectrumWidget(QWidget *parent, Qt::WFlags flags, QString name, int id):
	QWidget(parent, flags), ui(new Ui::SpectrumWidget)
{
	ui->setupUi(this);

	//connect(ui->graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(slotDoubleClicked(double, double)));
	//connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(slotIsShowContextMenu()));

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SLOT(slotSetEnablePanorama(bool)));
	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SLOT(slotAutoSearch(bool)));
	connect(ui->thresholdCB, SIGNAL(clicked(bool)), this, SLOT(slotSelectiontypeChanged(bool)));
	connect(ui->getSpectrumCB, SIGNAL(clicked(bool)), this, SLOT(slotRequestData(bool)));

	///hide/show hold peaks
	connect(ui->maximumsCB, SIGNAL(clicked(bool)), this, SLOT(slotShowPeaks(bool)));
	connect(ui->prmControlCB, SIGNAL(clicked(bool)), this, SLOT(slotShowControlPRM(bool)));
	connect(this, SIGNAL(signalCurSelChanged(int)), ui->graphicsWidget, SLOT(slotCurSelectionChanged(int)));
}

SpectrumWidget::~SpectrumWidget()
{
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

void SpectrumWidget::setControlPrmState(bool state)
{
	ui->prmControlCB->setChecked(state);
	//emit signalSetControlPRMState(state);
}

void SpectrumWidget::slotSetEnablePanorama(bool state)
{
	/// set panorama
	m_tab->set_panorama(state);
}

void SpectrumWidget::slotAutoSearch(bool state)
{
	m_autoSearch = state;

	if (!m_autoSearch) {
		ui->graphicsWidget->ClearAllDetectedAreas();
	}
}

/// change cur selection type
void SpectrumWidget::slotSelectiontypeChanged(bool state)
{
	if(state){
		emit signalCurSelChanged(4);
		return;
	}

	emit signalCurSelChanged(1);

	m_threshold = -1000;
}

/// set graphic name
/*
void SpectrumWidget::slotSetCaption(QString name)
{
	ui->spectrumNameLB->setText(name);
}*/

/// request data
void SpectrumWidget::slotRequestData(bool state)
{
	int data[4] = {0, 1, 2, 3};
	if(state){
		emit signalRequestData(m_id, 0, &data[0], 4);
	}
}

void SpectrumWidget::slotEnableKM(bool state)
{
	/// not yet realazed signal
	emit signalEnableKM(state);
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
/// if something selected then show context menu
/*void SpectrumWidget::slotIsShowContextMenu()
{
	if(ui->graphicsWidget->IsSomethingSelected())
		ui->graphicsWidget->contextMenu()->setEnabled(true);
	else
		ui->graphicsWidget->contextMenu()->setEnabled(false);
}*/

/// show/hide peaks
void SpectrumWidget::slotShowPeaks(bool visible)
{
	ui->graphicsWidget->SetSpectrumVisible(2, visible);
	m_peakVisible = visible;
}

void SpectrumWidget::slotShowControlPRM(bool state)
{
	m_tab->set_show_controlPRM(state);
}

/*void SpectrumWidget::slotDoubleClicked(double d1, double d2)
{
	emit doubleClickedSignal(m_id);
	//_tab->set_double_clicked(_id, d1, d2);
}
*/
