#include "SpectrumWidget.h"
#include <QDebug>
#include <QPaintEngine>

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

	connect( ui->graphicsWidget, SIGNAL(viewPortChanged(double, double, double, double)),
			 this, SLOT(onSlotChangedViewPort(double, double, double, double)) );

    m_analysisResultWidget = new AnalysisResultWidget();
    m_analysisResultWidget->close();

//	m_qwtSonogram = new QwtPlot(0);
//	m_qwtSonogramGL = new QwtPlotGLCanvas(m_qwtSonogram);
//	m_qwtSonogram->setCanvas(m_qwtSonogramGL);
//	m_qwtSonogram->setMaximumHeight(100);
	//ui->sonogramQwtLayout->addWidget( m_qwtSonogram, 0, 1 );
//	m_qwtSonogram->setVisible(false);

	m_plot = new QCustomPlot(this);
	ui->sonogramQwtLayout->addWidget(m_plot);
	m_plot->plotLayout()->setMargins(QMargins(0, 0, 0, 0));
	m_plot->setContentsMargins(0, 0, 0, 0);

	m_plot->setBackground(QBrush(Qt::black));
	m_plot->xAxis->setTickPen(QPen(Qt::white));
	m_plot->yAxis->setTickPen(QPen(Qt::white));

	QCPMarginGroup *group = new QCPMarginGroup(m_plot);

	m_plot->axisRect()->setAutoMargins(QCP::msNone);
	m_plot->axisRect()->setMargins( QMargins(49, 0, 0, 0) );

	m_plot->setMinimumHeight(50);
    m_plot->setMaximumHeight(500);
	m_plot->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
	m_plot->xAxis->setRange(0, 20);
	m_plot->yAxis->setRange(0, 10);
	m_plot->xAxis->setAutoTicks(false);
	m_plot->xAxis->setAutoTickStep(false);
	m_plot->xAxis->setTickLabels(false);

	m_plot->xAxis->setTickLabelColor(Qt::white);
	m_plot->xAxis->setTickStep(2);

	m_plot->yAxis->setBasePen(QPen(Qt::white));
	m_plot->yAxis->setTickLabelColor(Qt::white);

   // m_plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);


	QVector<double> tick;
	for(int i = 0; i<20; i++) {
		tick.append(i);
	}
	m_plot->xAxis->setTickVector(tick);

	QVector<double> ytick;
	for(int i = 0; i<10; i++) {
		ytick.append(i);
	}
	m_plot->yAxis->setTickVector(ytick);
	//m_plot->xAxis->setTickLabels(false);

	m_plot->addLayer("son", 0);

	m_plotPixmap = new QCPItemPixmap(m_plot);
	m_plotPixmap->setScaled(true, Qt::IgnoreAspectRatio);
	m_plotPixmap->setClipToAxisRect(true);
	m_plotPixmap->setLayer("son");

	m_plotPixmap->topLeft->setCoords(0, 10);
	m_plotPixmap->bottomRight->setCoords(20, 0);

	ui->textEdit->setVisible(false);
    ui->labelDetect->setVisible(false);
    m_plot->setVisible(false);
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

void SpectrumWidget::sonogramUpdate(const QPixmap &px)
{
//	ui->sonogramLbl->setMinimumHeight( px.height() );

//	int w = ui->sonogramLbl->width();
//	int h = ui->sonogramLbl->height();

//	// set a scaled pixmap to a w x h window keeping its aspect ratio
//	ui->sonogramLbl->setPixmap(px.scaled(w,h,Qt::KeepAspectRatio));

//	ui->sonogramLbl->setScaledContents(true);

	//==-======================================================

	//m_texture->setTexture( px );
	//m_qwtSonogram->setCanvasBackground( *m_texture );

	//================================================================
	m_plotPixmap->setPixmap(px);
	double start_hz = 0;
	double end_hz = 0;
	double start_n = 0;
	double end_n = 0;
	ui->graphicsWidget->GetCurrentViewport(start_hz, start_n, end_hz, end_n);
	onZoomSonogram(start_hz, end_hz, true);
	//m_plot->replot();
}

void SpectrumWidget::setAnalysisDetectedData(const RdsProtobuf::AnalysisDetected &msg)
{
    m_analysisResultWidget->setAnalysisData(msg);

	ui->textEdit->clear();

	QString txtDetected;
	int tst = msg.signal_size();
	Q_UNUSED(tst);

	for(int i = 0; i < msg.signal_size(); i++) {
		txtDetected.append( QString("Detected signal #%1 : \r\n").arg(i) );
		txtDetected.append( QString("	Range start %1  end %2 \r\n").arg( msg.signal(i).signal().range().start() ).
																	  arg( msg.signal(i).signal().range().end() ) );
		if( msg.signal(i).signal().has_parameters() ) {
			RdsProtobuf::Signal_SignalParameters param = msg.signal(i).signal().parameters();
			txtDetected.append( QString("	Signal System %1 \r\n").arg( QString::fromStdString(param.system()) ) );
			txtDetected.append( QString("	Signal Modulation %1 \r\n").arg( QString::fromStdString(param.modulation()) ) );
			txtDetected.append(  QString("	Signal Type %1\r\n").arg( QString::fromStdString(param.type()) ) );
		}

		if( msg.signal(i).signal().has_info() ) {
			txtDetected.append( QString("	Signal Info %1\r\n").arg( QString::fromStdString(msg.signal(i).signal().info()) ) );
		}
	}
    if(!txtDetected.isEmpty()) {
        ui->textEdit->clear();
        ui->textEdit->append(txtDetected);
    }
}

AnalysisResultWidget *SpectrumWidget::getAnalysisResultWidget()
{
    return m_analysisResultWidget;
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

void SpectrumWidget::recognize()
{
    m_analysisResultWidget->show();
}

void SpectrumWidget::slotSetWorkMode(int mode, bool isOn)
{
	if(mode == 2) {
		ui->textEdit->setVisible(true);
		m_plot->setVisible(true);
		ui->labelDetect->setVisible(true);
	} else {
		ui->textEdit->setVisible(false);
		m_plot->setVisible(false);
		ui->labelDetect->setVisible(false);
	}
}

void SpectrumWidget::onSlotChangedViewPort(double start_hz, double start_Y, double end_hz, double end_Y)
{
	int k = 0;

	if( start_hz == 0 && end_hz == 0 ) {
		return;
	}

	onZoomSonogram(start_hz, end_hz);
}


void SpectrumWidget::onZoomSonogram(double start_hz, double end_hz, bool replot)
{
	if( !m_plot->isVisible() ) {
		return;
	}

	double Z =  ui->graphicsWidget->ZeroFrequencyHz();

	double d1 = fabs(Z-start_hz)/TO_MHZ;
	double d2 = fabs(Z-end_hz)/TO_MHZ;

	if(d1 >= 0 && d1 <=20 && d2 >=0 && d2 <=20 && d1 < d2 &&
			d1 != m_plot->xAxis->range().lower &&
			d2 != m_plot->xAxis->range().upper ) {
		if(fabs(d1-d2) <= 0.5) {
			m_plot->xAxis->setRange(d1, d1+0.5);
		} else {
			m_plot->xAxis->setRange(d1, d2);
		}
		m_plot->replot();
		replot = false;
	}

	if(replot) {
		m_plot->replot();
	}
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
