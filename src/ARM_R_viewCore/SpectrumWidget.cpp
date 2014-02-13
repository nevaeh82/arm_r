#include "SpectrumWidget.h"
#include <QDebug>

#include "ui_SpectrumWidget.h"

#define TO_MHZ	1000000

SpectrumWidget::SpectrumWidget(QWidget *parent, Qt::WFlags flags, QString name, int id):
	QWidget(parent, flags), ui(new Ui::SpectrumWidget)
{
	ui->setupUi(this);

	m_current_frequency = 0;
	m_autoSearch = false;

	m_rett = -100;
	m_threshold = -1000;

	m_enableCorrelation = false;
	m_bandwidth = 0;
	m_pointCount = 0;
	m_isComplex = false;

	m_peakVisible = false;

	m_id = id;
	m_tab = NULL;

	m_graphicsContextMenu = new QMenu(ui->graphicsWidget);
	m_graphicsContextMenu->addAction(new QAction(tr("Add to white list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Add to black list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Determine signal"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Enable correlation"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Cleanup text fields"),this) );

	connect(m_graphicsContextMenu->actions()[0],SIGNAL(triggered()),SLOT(slotCMAddWhiteList()));
	connect(m_graphicsContextMenu->actions()[1],SIGNAL(triggered()),SLOT(slotCMAddBlackList()));
	connect(m_graphicsContextMenu->actions()[2],SIGNAL(triggered()),SLOT(slotRecognizeSignal()));
	connect(m_graphicsContextMenu->actions()[3],SIGNAL(triggered()),SLOT(slotSSCorrelation()));
	connect(m_graphicsContextMenu->actions()[4],SIGNAL(triggered()),SLOT(slotClearLabels()));

	connect(ui->graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(slotDoubleClicked(double, double)));
	connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(slotIsShowContextMenu()));

	ui->graphicsWidget->setContextMenu(m_graphicsContextMenu);

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SLOT(slotSetEnablePanorama(bool)));
	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SLOT(slotAutoSearch(bool)));
	connect(ui->thresholdCB, SIGNAL(clicked(bool)), this, SLOT(slotSelectiontypeChanged(bool)));
	connect(ui->getSpectrumCB, SIGNAL(clicked(bool)), this, SLOT(slotRequestData(bool)));

	///hide/show hold peaks
	connect(ui->maximumsCB, SIGNAL(clicked(bool)), this, SLOT(slotShowPeaks(bool)));
	connect(ui->prmControlCB, SIGNAL(clicked(bool)), this, SLOT(slotShowControlPRM(bool)));
	connect(this, SIGNAL(signalSetControlPRMState(bool)), ui->prmControlCB, SLOT(setChecked(bool)));
	connect(this, SIGNAL(signalCurSelChanged(int)), ui->graphicsWidget, SLOT(slotCurSelectionChanged(int)));

	ui->graphicsWidget->SetZoomOutMaxOnDataSet(true);
	ui->graphicsWidget->SetAlign(2);

	ui->graphicsWidget->ZoomOutFull();
	ui->graphicsWidget->SetHorizontalLabel(tr("Hz"));

	connect(ui->graphicsWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(ui->graphicsWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));

	m_spectrumPeakHold = new float[1];
	m_spectrumPeakHoldCorr = new float[1];

	connect(this, SIGNAL(signalSetFFTSetup(float*,float*)), this, SLOT(slotSetFFTSetup(float*,float*)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetFFT(float*, float*)), this, SLOT(slotSetFFT(float*, float*)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetDefModulation(QString)), this, SLOT(slotSetDefModulation(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetLabelName(QString,QString)), this, SLOT(slotSetLabelName(QString,QString)));

	connect(this, SIGNAL(signalSetDetectedAreas(QVector<QPointF>)), this, SLOT(slotSetDetectedAreas(QVector<QPointF>)));
	connect(this, SIGNAL(signalSetZeroFrequency(double)), this, SLOT(slotSetZeroFrequency(double)));

	this->installEventFilter(this);
}

SpectrumWidget::~SpectrumWidget()
{
}

void SpectrumWidget::setTab(ITabSpectrum *tab)
{
	m_tab = tab;
}

void SpectrumWidget::setId(const int id)
{
	m_id = id;
}

void SpectrumWidget::setSpectrumName(const QString &name)
{
	ui->spectrumNameLB->setText(name);
}

QString SpectrumWidget::getSpectrumName() const
{
	return ui->spectrumNameLB->text();
}

void SpectrumWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
}

Q_MG_SpectrumInterface *SpectrumWidget::getSpectrumWidget()
{
	return ui->graphicsWidget;
}

void SpectrumWidget::setup()
{
	QString fileNameSpec = "2.pcm";

	QFile fl(fileNameSpec);
	if (!fl.open(QIODevice::ReadOnly)){
		return;
	}

	unsigned int PointCount = (fl.size()/sizeof(float));
	if (PointCount < 5) {QMessageBox::critical(this, tr("File error"), tr("Very small file or file not found")); return;}
	float* spectrum = new float[PointCount];
	fl.read((char*)spectrum,PointCount*sizeof(float));
	fl.close();

	ui->graphicsWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

	delete[] spectrum;
}

void SpectrumWidget::setCoontrolPrmState(bool state)
{
	emit signalSetControlPRMState(state);
}

void SpectrumWidget::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	m_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;
	m_mux.unlock();
	emit signalSetFFTSetup(spectrum, spectrum_peak_hold);
}

void SpectrumWidget::setSignal(float *spectrum, float *spectrum_peak_hold)
{
	emit signalSetFFT(spectrum, spectrum_peak_hold);
}

bool SpectrumWidget::isGraphicVisible()
{
	return isVisible();
}

void SpectrumWidget::setDefModulation(QString modulation)
{
	emit signalSetDefModulation(modulation);
}

void SpectrumWidget::setLabelName(QString base, QString second)
{
	emit signalSetLabelName(base, second);
}

void SpectrumWidget::slotSetDefModulation(QString modulation)
{
	ui->graphicsWidget->SetLabel(m_centerFreqDefModulation*1000, modulation);
}

void SpectrumWidget::setDetectedAreasUpdate(QVector<QPointF> vec)
{
	emit signalSetDetectedAreas(vec);
}

void SpectrumWidget::setZeroFrequency(double val)
{
	emit signalSetZeroFrequency(val);
}

void SpectrumWidget::slotSetDetectedAreas(QVector<QPointF> vec)
{
	if(m_autoSearch == false){
		return;
	}
	ui->graphicsWidget->ClearAllDetectedAreas();
	QVector<QPointF>::iterator it;
	for(it = vec.begin(); it != vec.end(); ++it){
		ui->graphicsWidget->SetDetectedAreas((*it).x()*TO_MHZ + m_current_frequency, 0, (*it).y()*TO_MHZ + m_current_frequency, 0, false);
	}
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

void SpectrumWidget::slotSelectionFinished(double x1, double y1, double x2, double y2)
{
	///TODO: fix

	/// To HZ
	x1 /= 1000;
	x2 /= 1000;
	double dx;
	double center;
	if(x2 >= x1) {
		dx = x2 - x1;
		center = x1 + dx/2;
	}
	else {
		dx = x1 - x2;
		center = x2 + dx/2;
	}

	m_centerFreqSelTemp = center;

	SpectrumSelection selection;
	selection.start = QPointF(x1, y1);
	selection.end = QPointF(x2, y2);

	m_tab->set_selected_area(selection);
}

void SpectrumWidget::slotSelectionFinishedRedLine(double y)
{
	m_threshold = y;
	emit signalChoosedThreshold(y);
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
void SpectrumWidget::slotSetCaption(QString name)
{
	ui->spectrumNameLB->setText(name);
}

/// request data
void SpectrumWidget::slotRequestData(bool state)
{
	int data[4] = {0, 1, 2, 3};
	if(state){
		emit signalRequestData(m_id, 0, &data[0], 4);
	}
}

/// getting FFT
void SpectrumWidget::slotSetFFTSetup(float* spectrum, float* spectrum_peak_hold)
{
	ui->graphicsWidget->SetSpectrumVisible(2, m_peakVisible);

	float maxv = 0.0;
	float minv = 0.0;
	m_mux.lock();

	ui->graphicsWidget->SetAutoscaleY(true);
	ui->graphicsWidget->SetAlign(0);
	//ui->graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

	m_isComplex = false;
	ui->graphicsWidget->Setup(m_isComplex,m_bandwidth,tr("Level"), spectrum, m_pointCount, spectrum_peak_hold, m_pointCount,false, false, minv, maxv);
	m_mux.unlock();
}

void SpectrumWidget::slotSetFFT(float* spectrum, float* spectrum_peak_hold)
{
	m_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;

	ui->graphicsWidget->SetAutoscaleY(true);
	ui->graphicsWidget->SetAlign(0);
	//ui->graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

	m_mux.unlock();

	if((m_threshold != -1000) && (m_rett == -100))
	{
		for(int i = 0; i < m_pointCount; i++)
		{
			if((spectrum[i] > m_threshold) && (m_rett != -99))
			{
				m_rett = 0;
				break;
			}
		}
	}

	if(m_rett == 0)
	{
		int ret = QMessageBox::warning(this, tr("Attention!"),
									   tr("Signal was decected!"),
									   QMessageBox::Cancel, QMessageBox::Ok);
		m_rett = -101;

	}
	ui->graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
	ui->graphicsWidget->ZoomOutY();
}

void SpectrumWidget::slotEnableKM(bool state)
{
	/// not yet realazed signal
	emit signalEnableKM(state);
}

/// getting FFT
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
		ui->graphicsWidget->PermanentDataSetup(spectrum, NULL /*_spectrum_peak_hold_corr*/, minv, maxv);
	}

	delete[] spectrum;
}

/// if something selected then show context menu
void SpectrumWidget::slotIsShowContextMenu()
{
	if(ui->graphicsWidget->IsSomethingSelected())
		ui->graphicsWidget->contextMenu()->setEnabled(true);
	else
		ui->graphicsWidget->contextMenu()->setEnabled(false);
}

/// add selection to white list
void SpectrumWidget::slotCMAddWhiteList()
{
	emit signalAddSelToLists(1);
}

/// add selection to black list
void SpectrumWidget::slotCMAddBlackList()
{
	emit signalAddSelToLists(2);
}

/// signal for flakon to recognize signal
void SpectrumWidget::slotRecognizeSignal()
{
	m_centerFreqDefModulation = m_centerFreqSelTemp;
	CommandMessage *msg = new CommandMessage(COMMAND_RECOGNIZESIGNAL, QVariant());
	m_tab->set_command(TypeGraphicCommand,msg);
}

/// signal for flakon to recognize signal
void SpectrumWidget::slotSSCorrelation()
{
	m_enableCorrelation = !m_enableCorrelation;
	CommandMessage *msg = new CommandMessage(COMMAND_KM, m_enableCorrelation);
	m_tab->set_command(TypeGraphicCommand,msg);
	if(m_enableCorrelation)
		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	else
		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
}

void SpectrumWidget::slotClearLabels()
{
	ui->graphicsWidget->ClearAllLabels();
}

/// show/hide peaks
void SpectrumWidget::slotShowPeaks(bool visible)
{
	ui->graphicsWidget->SetSpectrumVisible(2, visible);
	m_peakVisible = visible;
}

void SpectrumWidget::slotSetLabelName(QString base, QString second)
{
	ui->graphicsWidget->SetLabel(m_bandwidth/2, base);
}

void SpectrumWidget::slotShowControlPRM(bool state)
{
	m_tab->set_show_controlPRM(state);
}

void SpectrumWidget::slotDoubleClicked(double d1, double d2)
{
	emit doubleClickedSignal(m_id);
	//_tab->set_double_clicked(_id, d1, d2);
}

void SpectrumWidget::slotSetZeroFrequency(double val)
{
	double cur_freq = m_tab->get_current_frequency();
	m_current_frequency = cur_freq*TO_MHZ;
	ui->graphicsWidget->SetZeroFrequencyHz(val + m_current_frequency);
}
