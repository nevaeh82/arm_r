#include "GraphicWidget.h"
#include <QDebug>

#include "ui_GraphicsWidget.h"

GraphicWidget::GraphicWidget(QWidget *parent, Qt::WFlags flags, QString name, int id):
	QWidget(parent, flags), ui(new Ui::GraphicsWidget)
{

	ui->setupUi(this);

	_rett = -100;
	_threshold = -1000;

	_enable_correlation = false;
	_bandwidth = 0;
	_pointCount = 0;
	_isComplex = false;

	_peak_visible = false;

	_id = id;
	_tab = NULL;


	m_graphicsContextMenu = new QMenu(ui->spectrumWidget);
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


	connect(ui->spectrumWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(_slot_double_clicked(double, double)));


	connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(_slotIsShowContextMenu()));

	ui->spectrumWidget->setContextMenu(m_graphicsContextMenu);

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SLOT(_slotSetEnablePanorama(bool)));

	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SLOT(_slotAutoSearch(bool)));

	connect(ui->thresholdCB, SIGNAL(clicked(bool)), this, SLOT(_slotSelectiontypeChanged(bool)));

	connect(ui->getSpectrumCB, SIGNAL(clicked(bool)), this, SLOT(_slotRequestData(bool)));

	///hide/show hold peaks
	connect(ui->maximumsCB, SIGNAL(clicked(bool)), this, SLOT(_slotShowPeaks(bool)));

	connect(ui->prmControlCB, SIGNAL(clicked(bool)), this, SLOT(_slotShowControlPRM(bool)));

	connect(this, SIGNAL(signalSetControlPRMState(bool)), ui->prmControlCB, SLOT(setChecked(bool)));

	connect(this, SIGNAL(signalCurSelChanged(int)), ui->spectrumWidget, SLOT(slotCurSelectionChanged(int)));

	ui->spectrumWidget->SetZoomOutMaxOnDataSet(true);
	ui->spectrumWidget->SetAlign(2);

	//    QString fileNameSpec = "1.pcm";

	//    QFile fl(fileNameSpec);
	//    if (fl.open(QIODevice::ReadOnly))
	//    {
	//        unsigned int PointCount = (fl.size()/sizeof(float));
	//        if (PointCount < 5) {QMessageBox::critical(this,"File error","Very small file or file not found"); return;}
	//        float* spectrum = new float[PointCount];
	//        fl.read((char*)spectrum,PointCount*sizeof(float));
	//        fl.close();


	//        spectrumWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

	//        delete[] spectrum;
	//    }

	//        unsigned int PointCount = 2;
	//        float* spectrum = new float[PointCount];
	//        spectrum[0] = 0;
	//        spectrum[1] = 1;


	//        spectrumWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

	//        delete[] spectrum;

	ui->spectrumWidget->ZoomOutFull();
	ui->spectrumWidget->SetHorizontalLabel("√ц");

	//    spectrumWidget->Reset();

	connect(ui->spectrumWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(ui->spectrumWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));



	_spectrum_peak_hold = new float[1];
	_spectrum_peak_hold_corr = new float[1];

	connect(this, SIGNAL(signalSetFFTSetup(float*,float*)), this, SLOT(_slotSetFFTSetup(float*,float*)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetFFT(float*, float*)), this, SLOT(_slotSetFFT(float*, float*)), Qt::QueuedConnection);
	connect(this, SIGNAL(signalSetDefModulation(QString)), this, SLOT(_slotSetDefModulation(QString)), Qt::QueuedConnection);


	connect(this, SIGNAL(signalSetLabelName(QString,QString)), this, SLOT(_slotSetLabelName(QString,QString)));
	//    connect(this, SIGNAL(signalNeedToUpdate()), spectrumWidget, SLOT(update()));
	//    QTimer* timer = new QTimer();
	//    connect(timer, SIGNAL(timeout()), spectrumWidget, SLOT(update()));
	//    timer->start(2000);

	//this->setLayout(mainLayout);
	this->installEventFilter(this);

}

GraphicWidget::~GraphicWidget()
{
	//delete m_graphicsContextMenu;
}

void GraphicWidget::setTab(ITabSpectrum *tab)
{
	_tab = tab;
}

void GraphicWidget::setId(const int id)
{
	_id = id;
}

void GraphicWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
	//    emit DoubleClicked(GetValueByPixel(event->pos().x(),false),GetValueByPixel(event->pos().y(),true));
	//	event->accept();
}

Q_MG_SpectrumInterface *GraphicWidget::get_spectrum_widget()
{   
	return ui->spectrumWidget;
}

void GraphicWidget::setup()
{
	QString fileNameSpec = "2.pcm";

	QFile fl(fileNameSpec);
	if (fl.open(QIODevice::ReadOnly))
	{
		unsigned int PointCount = (fl.size()/sizeof(float));
		if (PointCount < 5) {QMessageBox::critical(this,"File error","Very small file or file not found"); return;}
		float* spectrum = new float[PointCount];
		fl.read((char*)spectrum,PointCount*sizeof(float));
		fl.close();


		ui->spectrumWidget->Setup(true,9000,"",spectrum,PointCount, 0, 0);

		delete[] spectrum;
	}
}

void GraphicWidget::set_coontrolPRM_state(bool state)
{
	emit signalSetControlPRMState(state);
}

void GraphicWidget::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	_mux.lock();
	_bandwidth = bandwidth;
	_pointCount = PointCount;
	_isComplex = isComplex;
	_mux.unlock();
	emit signalSetFFTSetup(spectrum, spectrum_peak_hold);
}

void GraphicWidget::setSignal(float *spectrum, float *spectrum_peak_hold)
{
	emit signalSetFFT(spectrum, spectrum_peak_hold);
}

bool GraphicWidget::isGraphicVisible()
{
	return isVisible();
}

void GraphicWidget::setDefModulation(QString modulation)
{
	emit signalSetDefModulation(modulation);
}

void GraphicWidget::setLabelName(QString base, QString second)
{
	emit signalSetLabelName(base, second);
}

void GraphicWidget::_slotSetDefModulation(QString modulation)
{
	ui->spectrumWidget->SetLabel(_center_freq_def_modulation*1000, modulation);
}

void GraphicWidget::_slotSetEnablePanorama(bool state)
{
	/// set panorama
}

void GraphicWidget::_slotAutoSearch(bool state)
{
	/// set auto search
}

void GraphicWidget::slotSelectionFinished(double x1, double y1, double x2, double y2)
{
	/// to к√ц
	x1 /= 1000;
	x2 /= 1000;
	double dx;
	double center;
	if(x2 >= x1)
	{
		dx = x2 - x1;
		center = x1 + dx/2;
	}
	else
	{
		dx = x1 - x2;
		center = x2 + dx/2;
	}
	double dy = y2 - y1;
	//    emit selected(x1, x2, y1, y2);

	QMap<int, QVariant> _selection;
	_selection.insert(1, QString::number(dx, 'f', 3));
	_selection.insert(2, QString::number(center, 'f', 3));
	_selection.insert(3, QString::number(x1, 'f', 3));
	_selection.insert(4, QString::number(x2, 'f', 3));

	_center_freq_sel_temp = center;

	_tab->set_selected_area(_selection);
}

void GraphicWidget::slotSelectionFinishedRedLine(double y)
{
	_threshold = y;
	emit signalChoosedThreshold(y);
}

/// change cur selection type
void GraphicWidget::_slotSelectiontypeChanged(bool state)
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
void GraphicWidget::slotSetCaption(QString name)
{
	//_caption->setText(name);
}

/// request data
void GraphicWidget::_slotRequestData(bool state)
{
	int data[4] = {0, 1, 2, 3};
	if(state)
	{
		emit signalRequestData(_id, 0, &data[0], 4);
	}
}

/// getting FFT
void GraphicWidget::_slotSetFFTSetup(float* spectrum, float* spectrum_peak_hold)
{

	ui->spectrumWidget->SetSpectrumVisible(2, _peak_visible);

	float maxv = 0.0;
	float minv = 0.0;
	_mux.lock();
	//    if(_isComplex)
	//    {
	//        maxv = 1.0;
	//        minv = 0.0;
	//    }

	//    if(!_isComplex)
	//    {
	ui->spectrumWidget->SetAutoscaleY(true);
	ui->spectrumWidget->SetAlign(0);
	ui->spectrumWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);
	//    }
	//    else
	//    {
	//        spectrumWidget->SetAutoscaleY(false);
	////        spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
	//        spectrumWidget->SetAlign(3);
	//    }

	ui->spectrumWidget->Setup(_isComplex,_bandwidth,"”ровень", spectrum, _pointCount, spectrum_peak_hold, _pointCount,false, false, minv, maxv);
	_mux.unlock();

}

void GraphicWidget::_slotSetFFT(float* spectrum, float* spectrum_peak_hold)
{
	_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;
	//    if(_isComplex)
	//    {
	//        maxv = 1.0;
	//        minv = 0.0;
	//    }

	//    if(!_isComplex)
	//    {
	ui->spectrumWidget->SetAutoscaleY(true);
	ui->spectrumWidget->SetAlign(0);
	ui->spectrumWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);
	//    }
	//    else
	//    {
	//        spectrumWidget->SetAutoscaleY(false);
	////        spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
	//        spectrumWidget->SetAlign(3);
	//    }


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
	ui->spectrumWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
	ui->spectrumWidget->ZoomOutY();

}

void GraphicWidget::_slotEnableKM(bool state)
{
	/// not yet realazed signal
	emit signalEnableKM(state);
}

/// getting FFT
void GraphicWidget::slotSetCorrelations(int id, const QVector<QPointF> vecFFT, const bool isComplex)
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
		//        if(spectrum[i] > _spectrum_peak_hold_corr[i])
		//        {
		//            _spectrum_peak_hold_corr[i] = spectrum[i];
		//        }
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
		ui->spectrumWidget->SetAutoscaleY(true);
		ui->spectrumWidget->SetAlign(0);
		ui->spectrumWidget->SetZeroFrequencyHz(vecFFT.at(0).x());
	}
	else
	{
		ui->spectrumWidget->SetAutoscaleY(false);
		//        spectrumWidget->SetZeroFrequencyHz(-bandwidth/2);
		ui->spectrumWidget->SetAlign(3);
	}

	if(ui->spectrumWidget->BandWidth() == bandwidth)
	{
		ui->spectrumWidget->PermanentDataSetup(spectrum, NULL /*_spectrum_peak_hold_corr*/, minv, maxv);
	}
	else
	{
		//        delete[] _spectrum_peak_hold_corr;
		//        _spectrum_peak_hold_corr = new float[PointCount];
		//        for(int i = 0; i < PointCount; i++)
		//        {
		//            _spectrum_peak_hold_corr[i] = spectrum[i];
		//        }
		//        spectrumWidget->Setup(isComplex,bandwidth,"”ровень",spectrum,PointCount, 0/*_spectrum_peak_hold_corr*/,0/*PointCount*/,false, false, minv, maxv);
	}
	delete[] spectrum;
}

/// if something selected then show context menu
void GraphicWidget::_slotIsShowContextMenu()
{
	if(ui->spectrumWidget->IsSomethingSelected())
		ui->spectrumWidget->contextMenu()->setEnabled(true);
	else
		ui->spectrumWidget->contextMenu()->setEnabled(false);
}

/// add selection to white list
void GraphicWidget::_slotCMAddWhiteList()
{
	emit signalAddSelToLists(1);
}

/// add selection to black list
void GraphicWidget::_slotCMAddBlackList()
{
	emit signalAddSelToLists(2);
}

/// signal for flakon to recognize signal
void GraphicWidget::_slotRecognizeSignal()
{
	_center_freq_def_modulation = _center_freq_sel_temp;
	CommandMessage *msg = new CommandMessage(COMMAND_RECOGNIZESIGNAL, QVariant());
	_tab->set_command(msg);
}

/// signal for flakon to recognize signal
void GraphicWidget::_slotSSCorrelation()
{
	_enable_correlation = !_enable_correlation;
	CommandMessage *msg = new CommandMessage(COMMAND_KM, _enable_correlation);
	_tab->set_command(msg);
	if(_enable_correlation)
		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	else
		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
}

void GraphicWidget::_slotClearLabels()
{
	ui->spectrumWidget->ClearAllLabels();
}

/// show/hide peaks
void GraphicWidget::_slotShowPeaks(bool visible)
{
	ui->spectrumWidget->SetSpectrumVisible(2, visible);
	_peak_visible = visible;
}

void GraphicWidget::_slotSetLabelName(QString base, QString second)
{
	ui->spectrumWidget->SetLabel(_bandwidth/2, base);
}

void GraphicWidget::_slotShowControlPRM(bool state)
{
	_tab->set_show_controlPRM(state);
}

void GraphicWidget::_slot_double_clicked(double d1, double d2)
{
	_tab->set_double_clicked(_id, d1, d2);
}
