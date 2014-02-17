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

	_rett = -100;
	_threshold = -1000;

	_enable_correlation = false;
	_bandwidth = 0;
	_pointCount = 0;
	_isComplex = false;

	_peak_visible = false;

	_id = id;
	_tab = NULL;


	m_graphicsContextMenu = new QMenu(ui->graphicsWidget);
	m_graphicsContextMenu->addAction(new QAction(tr("Add to white list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Add to black list"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Determine signal"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Enable correlation"),this) );
	m_graphicsContextMenu->addAction(new QAction(tr("Cleanup text fields"),this) );

	connect(m_graphicsContextMenu->actions()[0],SIGNAL(triggered()),SLOT(_slotCMAddWhiteList()));
	connect(m_graphicsContextMenu->actions()[1],SIGNAL(triggered()),SLOT(_slotCMAddBlackList()));
	connect(m_graphicsContextMenu->actions()[2],SIGNAL(triggered()),SLOT(_slotRecognizeSignal()));
	connect(m_graphicsContextMenu->actions()[3],SIGNAL(triggered()),SLOT(_slotSSCorrelation()));
	connect(m_graphicsContextMenu->actions()[4],SIGNAL(triggered()),SLOT(_slotClearLabels()));


	connect(ui->graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(_slot_double_clicked(double, double)));


	connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(_slotIsShowContextMenu()));

	ui->graphicsWidget->setContextMenu(m_graphicsContextMenu);

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SLOT(_slotSetEnablePanorama(bool)));

	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SLOT(_slotAutoSearch(bool)));

	connect(ui->thresholdCB, SIGNAL(clicked(bool)), this, SLOT(_slotSelectiontypeChanged(bool)));

	connect(ui->getSpectrumCB, SIGNAL(clicked(bool)), this, SLOT(_slotRequestData(bool)));

	///hide/show hold peaks
	connect(ui->maximumsCB, SIGNAL(clicked(bool)), this, SLOT(_slotShowPeaks(bool)));

	connect(ui->prmControlCB, SIGNAL(clicked(bool)), this, SLOT(_slotShowControlPRM(bool)));

	connect(this, SIGNAL(signalSetControlPRMState(bool)), ui->prmControlCB, SLOT(setChecked(bool)));

	connect(this, SIGNAL(signalCurSelChanged(int)), ui->graphicsWidget, SLOT(slotCurSelectionChanged(int)));

	ui->graphicsWidget->SetZoomOutMaxOnDataSet(true);
	ui->graphicsWidget->SetAlign(2);

	ui->graphicsWidget->ZoomOutFull();
	ui->graphicsWidget->SetHorizontalLabel(tr("Hz"));

	connect(ui->graphicsWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(ui->graphicsWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));

	_spectrum_peak_hold = new float[1];
	_spectrum_peak_hold_corr = new float[1];

	connect(this, SIGNAL(signalSetFFTSetup(float*,float*)), this, SLOT(_slotSetFFTSetup(float*,float*)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetFFT(float*, float*)), this, SLOT(_slotSetFFT(float*, float*)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetDefModulation(QString)), this, SLOT(_slotSetDefModulation(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetLabelName(QString,QString)), this, SLOT(_slotSetLabelName(QString,QString)));

	connect(this, SIGNAL(signalSetDetectedAreas(QByteArray)), this, SLOT(m_slotSetDetectedAreas(QByteArray)));
	connect(this, SIGNAL(signalSetZeroFrequency(double)), this, SLOT(m_slotSetZeroFrequency(double)));



	this->installEventFilter(this);
}

SpectrumWidget::~SpectrumWidget()
{
}

void SpectrumWidget::setTab(ITabSpectrum *tab)
{
	_tab = tab;
}

void SpectrumWidget::setId(const int id)
{
	_id = id;
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

Q_MG_SpectrumInterface *SpectrumWidget::get_spectrum_widget()
{
	return ui->graphicsWidget;
}

void SpectrumWidget::setup()
{
	QString fileNameSpec = "2.pcm";

	QFile fl(fileNameSpec);
	if (fl.open(QIODevice::ReadOnly))
	{
		unsigned int PointCount = (fl.size()/sizeof(float));
		if (PointCount < 5) {QMessageBox::critical(this, tr("File error"), tr("Very small file or file not found")); return;}
		float* spectrum = new float[PointCount];
		fl.read((char*)spectrum,PointCount*sizeof(float));
		fl.close();


		ui->graphicsWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

		delete[] spectrum;
	}
}

void SpectrumWidget::set_coontrolPRM_state(bool state)
{
	emit signalSetControlPRMState(state);
}

void SpectrumWidget::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	_mux.lock();
	_bandwidth = bandwidth;
	_pointCount = PointCount;
	_isComplex = isComplex;
	_mux.unlock();
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

void SpectrumWidget::_slotSetDefModulation(QString modulation)
{
	ui->graphicsWidget->SetLabel(_center_freq_def_modulation*1000, modulation);
}


void SpectrumWidget::setDetectedAreasUpdate(const QByteArray &vec)
{
	emit signalSetDetectedAreas(vec);
}

void SpectrumWidget::setZeroFrequency(double val)
{
	emit signalSetZeroFrequency(val);
}



void SpectrumWidget::m_slotSetDetectedAreas(QByteArray inVecBA)
{
	if(!m_autoSearch){
		return;
	}

	QVector<QPointF> vec;
	QDataStream stream(inVecBA);
	stream >> vec;

	ui->graphicsWidget->ClearAllDetectedAreas();
	QVector<QPointF>::iterator it;
	for(it = vec.begin(); it != vec.end(); ++it)
	{
		ui->graphicsWidget->SetDetectedAreas((*it).x()*TO_MHZ + m_current_frequency, 0, (*it).y()*TO_MHZ + m_current_frequency, 0, false);
	}
}


void SpectrumWidget::_slotSetEnablePanorama(bool state)
{
	/// set panorama
	_tab->set_panorama(state);
}

void SpectrumWidget::_slotAutoSearch(bool state)
{
	m_autoSearch = state;

	if (!m_autoSearch) {
		ui->graphicsWidget->ClearAllDetectedAreas();
	}
}

void SpectrumWidget::slotSelectionFinished(double x1, double y1, double x2, double y2)
{
	///TODO: fix

	/// Ho HZ
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

	_center_freq_sel_temp = center;

	SpectrumSelection selection;
	selection.start = QPointF(x1, y1);
	selection.end = QPointF(x2, y2);

	_tab->set_selected_area(selection);
}

void SpectrumWidget::slotSelectionFinishedRedLine(double y)
{
	_threshold = y;
	emit signalChoosedThreshold(y);
}

/// change cur selection type
void SpectrumWidget::_slotSelectiontypeChanged(bool state)
{
	if(state)
	{
		emit signalCurSelChanged(4);
	}
	else
	{
		emit signalCurSelChanged(1);
		_threshold = -1000;
	}
}

/// set graphic name
void SpectrumWidget::slotSetCaption(QString name)
{
	ui->spectrumNameLB->setText(name);
}

/// request data
void SpectrumWidget::_slotRequestData(bool state)
{
	int data[4] = {0, 1, 2, 3};
	if(state)
	{
		emit signalRequestData(_id, 0, &data[0], 4);
	}
}

/// getting FFT
void SpectrumWidget::_slotSetFFTSetup(float* spectrum, float* spectrum_peak_hold)
{
	ui->graphicsWidget->SetSpectrumVisible(2, _peak_visible);

	float maxv = 0.0;
	float minv = 0.0;
	_mux.lock();

	ui->graphicsWidget->SetAutoscaleY(true);
	ui->graphicsWidget->SetAlign(0);
	//ui->graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

	_isComplex = false;
	ui->graphicsWidget->Setup(_isComplex,_bandwidth,tr("Level"), spectrum, _pointCount, spectrum_peak_hold, _pointCount,false, false, minv, maxv);
	_mux.unlock();

}

void SpectrumWidget::_slotSetFFT(float* spectrum, float* spectrum_peak_hold)
{
	_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;

	ui->graphicsWidget->SetAutoscaleY(true);
	ui->graphicsWidget->SetAlign(0);
	//ui->graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

	_mux.unlock();

	if((_threshold != -1000) && (_rett == -100))
	{
		for(int i = 0; i < _pointCount; i++)
		{
			if((spectrum[i] > _threshold) && (_rett != -99))
			{
				_rett = 0;
				break;
			}
		}
	}

	if(_rett == 0 )
	{
		int ret = QMessageBox::warning(this, tr("Attention!"),
									   tr("Signal was decected!"),
									   QMessageBox::Cancel, QMessageBox::Ok);

		_rett = -101;

	}
	ui->graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
	ui->graphicsWidget->ZoomOutY();

}

void SpectrumWidget::_slotEnableKM(bool state)
{
	/// not yet realazed signal
	emit signalEnableKM(state);
}

/// getting FFT
void SpectrumWidget::slotSetCorrelations(int id, const QVector<QPointF> vecFFT, const bool isComplex)
{
	if(id != _id)
		return;

	int PointCount = vecFFT.size();
	float* spectrum = new float[PointCount];
	qreal startx = vecFFT.at(0).x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = endx - startx;
	for(int i = 0; i < vecFFT.size(); i++)
	{
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
void SpectrumWidget::_slotIsShowContextMenu()
{
	if(ui->graphicsWidget->IsSomethingSelected())
		ui->graphicsWidget->contextMenu()->setEnabled(true);
	else
		ui->graphicsWidget->contextMenu()->setEnabled(false);
}

/// add selection to white list
void SpectrumWidget::_slotCMAddWhiteList()
{
	emit signalAddSelToLists(1);
}

/// add selection to black list
void SpectrumWidget::_slotCMAddBlackList()
{
	emit signalAddSelToLists(2);
}

/// signal for flakon to recognize signal
void SpectrumWidget::_slotRecognizeSignal()
{
	_center_freq_def_modulation = _center_freq_sel_temp;
	CommandMessage *msg = new CommandMessage(COMMAND_RECOGNIZESIGNAL, QVariant());
	_tab->set_command(TypeGraphicCommand,msg);
}

/// signal for flakon to recognize signal
void SpectrumWidget::_slotSSCorrelation()
{
	_enable_correlation = !_enable_correlation;
	CommandMessage *msg = new CommandMessage(COMMAND_KM, _enable_correlation);
	_tab->set_command(TypeGraphicCommand,msg);
	if(_enable_correlation)
		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	else
		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
}

void SpectrumWidget::_slotClearLabels()
{
	ui->graphicsWidget->ClearAllLabels();
}

/// show/hide peaks
void SpectrumWidget::_slotShowPeaks(bool visible)
{
	ui->graphicsWidget->SetSpectrumVisible(2, visible);
	_peak_visible = visible;
}

void SpectrumWidget::_slotSetLabelName(QString base, QString second)
{
	ui->graphicsWidget->SetLabel(_bandwidth/2, base);
}

void SpectrumWidget::_slotShowControlPRM(bool state)
{
	_tab->set_show_controlPRM(state);
}

void SpectrumWidget::_slot_double_clicked(double d1, double d2)
{
	emit doubleClickedSignal(_id);
	//_tab->set_double_clicked(_id, d1, d2);
}

void SpectrumWidget::m_slotSetZeroFrequency(double val)
{
	double cur_freq = _tab->get_current_frequency();
	m_current_frequency = cur_freq*TO_MHZ;
	ui->graphicsWidget->SetZeroFrequencyHz(val + m_current_frequency);
}

