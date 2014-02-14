#include "SpectrumWidgetController.h"
#include "SpectrumWidget.h"

SpectrumWidgetController::SpectrumWidgetController(QObject *parent) :
	QObject(parent)
{
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

	m_spectrumPeakHold = new float[1];
	m_spectrumPeakHoldCorr = new float[1];

	m_graphicsWidget = NULL;
	m_graphicsContextMenu = NULL;
}

void SpectrumWidgetController::appendView(SpectrumWidget* view)
{
	m_view = view;

	init();
}

void SpectrumWidgetController::setTab(ITabSpectrum *tab)
{
	m_tab = tab;
}

void SpectrumWidgetController::setId(const int id)
{
	m_id = id;
}

void SpectrumWidgetController::setSpectrumName(const QString &name)
{
	m_view->setSpectrumName(name);
}

void SpectrumWidgetController::setup()
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

	m_graphicsWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

	delete[] spectrum;
}

void SpectrumWidgetController::setControlPrmState(bool state)
{
	m_view->setControlPrmState(state);
}

QString SpectrumWidgetController::getSpectrumName() const
{
	return m_view->getSpectrumName();
}

void SpectrumWidgetController::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	m_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;
	m_mux.unlock();

	m_graphicsWidget->SetSpectrumVisible(2, m_peakVisible);

	float maxv = 0.0;
	float minv = 0.0;
	m_mux.lock();

	m_graphicsWidget->SetAutoscaleY(true);
	m_graphicsWidget->SetAlign(0);
	//m_graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

	m_isComplex = false;
	m_graphicsWidget->Setup(m_isComplex,m_bandwidth,tr("Level"), spectrum, m_pointCount, spectrum_peak_hold, m_pointCount,false, false, minv, maxv);
	m_mux.unlock();
}

void SpectrumWidgetController::setSignal(float *spectrum, float *spectrum_peak_hold)
{
	m_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;

	m_graphicsWidget->SetAutoscaleY(true);
	m_graphicsWidget->SetAlign(0);
	//m_graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

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
	m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
	m_graphicsWidget->ZoomOutY();
}

void SpectrumWidgetController::setDefModulation(QString modulation)
{
	m_graphicsWidget->SetLabel(m_centerFreqDefModulation*1000, modulation);
}

bool SpectrumWidgetController::isGraphicVisible()
{
	return m_view->isVisible();
}

void SpectrumWidgetController::setLabelName(QString base, QString second)
{
	m_graphicsWidget->SetLabel(m_bandwidth/2, base);
}

void SpectrumWidgetController::setDetectedAreasUpdate(QVector<QPointF> vec)
{
	if(m_autoSearch == false){
		return;
	}
	m_graphicsWidget->ClearAllDetectedAreas();
	QVector<QPointF>::iterator it;
	for(it = vec.begin(); it != vec.end(); ++it){
		m_graphicsWidget->SetDetectedAreas((*it).x()*TO_MHZ + m_current_frequency, 0, (*it).y()*TO_MHZ + m_current_frequency, 0, false);
	}
}

void SpectrumWidgetController::setZeroFrequency(double val)
{
	double cur_freq = m_tab->get_current_frequency();
	m_current_frequency = cur_freq*TO_MHZ;
	m_graphicsWidget->SetZeroFrequencyHz(val + m_current_frequency);
}

void SpectrumWidgetController::init()
{
	m_graphicsWidget = m_view->getGraphicsWidget();

	m_graphicsWidget->SetZoomOutMaxOnDataSet(true);
	m_graphicsWidget->SetAlign(2);

	m_graphicsWidget->ZoomOutFull();
	m_graphicsWidget->SetHorizontalLabel(tr("Hz"));

	m_graphicsContextMenu = new QMenu(m_graphicsWidget);
	m_graphicsContextMenu->addAction(new QAction(tr("Add to white list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Add to black list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Determine signal"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Enable correlation"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Cleanup text fields"),this) );

	connect(m_graphicsContextMenu->actions().at(0),SIGNAL(triggered()),SLOT(slotCMAddWhiteList()));
	connect(m_graphicsContextMenu->actions().at(1),SIGNAL(triggered()),SLOT(slotCMAddBlackList()));
	connect(m_graphicsContextMenu->actions().at(2),SIGNAL(triggered()),SLOT(slotRecognizeSignal()));
	connect(m_graphicsContextMenu->actions().at(3),SIGNAL(triggered()),SLOT(slotSSCorrelation()));
	connect(m_graphicsContextMenu->actions().at(4),SIGNAL(triggered()),SLOT(slotClearLabels()));
	connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(slotIsShowContextMenu()));

	ui->graphicsWidget->setContextMenu(m_graphicsContextMenu);

	connect(m_graphicsWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(m_graphicsWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));
	connect(ui->graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(slotDoubleClicked(double, double)));
}

/// add selection to white list
void SpectrumWidgetController::slotCMAddWhiteList()
{
	emit signalAddSelToLists(1);
}

/// add selection to black list
void SpectrumWidgetController::slotCMAddBlackList()
{
	emit signalAddSelToLists(2);
}

/// signal for flakon to recognize signal
void SpectrumWidgetController::slotRecognizeSignal()
{
	m_centerFreqDefModulation = m_centerFreqSelTemp;
	CommandMessage *msg = new CommandMessage(COMMAND_RECOGNIZESIGNAL, QVariant());
	m_tab->set_command(TypeGraphicCommand,msg);
}

/// signal for flakon to recognize signal
void SpectrumWidgetController::slotSSCorrelation()
{
	m_enableCorrelation = !m_enableCorrelation;
	CommandMessage *msg = new CommandMessage(COMMAND_KM, m_enableCorrelation);
	m_tab->set_command(TypeGraphicCommand,msg);
	if(m_enableCorrelation)
		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	else
		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
}

void SpectrumWidgetController::slotClearLabels()
{
	m_graphicsWidget->ClearAllLabels();
}

void SpectrumWidgetController::slotSelectionFinished(double x1, double y1, double x2, double y2)
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

void SpectrumWidgetController::slotSelectionFinishedRedLine(double y)
{
	m_threshold = y;
	emit signalChoosedThreshold(y);
}

void SpectrumWidgetController::slotIsShowContextMenu()
{
	if(m_graphicsWidget->IsSomethingSelected())
		m_graphicsWidget->contextMenu()->setEnabled(true);
	else
		m_graphicsWidget->contextMenu()->setEnabled(false);
}

void SpectrumWidgetController::slotDoubleClicked(double d1, double d2)
{
	emit doubleClickedSignal(m_id);
}
