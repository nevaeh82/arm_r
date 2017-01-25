#include "SpectrumWidget.h"
#include <QDebug>
#include <QPaintEngine>
#include <QComboBox>

#include "ui_SpectrumWidget.h"

SpectrumWidget::SpectrumWidget(QWidget *parent, Qt::WFlags flags):
	QWidget(parent, flags), ui(new Ui::SpectrumWidget)
{
	ui->setupUi(this);

	ui->labelNo->setVisible(false);

	connect(ui->panoramaCB, SIGNAL(toggled(bool)), this, SIGNAL(setPanoramaSignal(bool)));

	connect(ui->autosearchCB, SIGNAL(toggled(bool)), this, SIGNAL(setAutoSearchSignal(bool)));
	connect(ui->thresholdCB, SIGNAL(toggled(bool)), this, SIGNAL(selectionTypeChangedSignal(bool)));
	connect(ui->getSpectrumCB, SIGNAL(toggled(bool)), this, SIGNAL(requestDataSignal(bool)));

	connect(ui->maximumsCB, SIGNAL(clicked(bool)), this, SIGNAL(setShowPeaksSignal(bool)));
	connect(ui->prmControlCB, SIGNAL(clicked(bool)), this, SIGNAL(setShowControlPRM(bool)));

	connect( ui->graphicsWidget, SIGNAL(viewPortChanged(double, double, double, double)),
			 this, SLOT(onSlotChangedViewPort(double, double, double, double)) );

	//	m_qwtSonogram = new QwtPlot(0);
	//	m_qwtSonogramGL = new QwtPlotGLCanvas(m_qwtSonogram);
	//	m_qwtSonogram->setCanvas(m_qwtSonogramGL);
	//	m_qwtSonogram->setMaximumHeight(100);
	//ui->sonogramQwtLayout->addWidget( m_qwtSonogram, 0, 1 );
	//	m_qwtSonogram->setVisible(false);

	//m_plot = new QCustomPlot(this);
	m_plot = ui->sPlot;
	//ui->sonogramQwtLayout->addWidget(m_plot);
	m_plot->plotLayout()->setMargins(QMargins(0, 0, 0, 0));
	m_plot->setContentsMargins(0, 0, 0, 0);

	initCustomPlot(m_plot);

	m_plot->addLayer("son", 0);

	m_plotPixmap = new QCPItemPixmap(m_plot);
	m_plotPixmap->setScaled(true, Qt::IgnoreAspectRatio);
	m_plotPixmap->setClipToAxisRect(true);
	m_plotPixmap->setLayer("son");

	m_plotPixmap->topLeft->setCoords(0, 10);
	m_plotPixmap->bottomRight->setCoords(20, 0);

	m_plot->setVisible(false);
	//m_dopplerPlot->setVisible(false);

	ui->graphicsWidget->SetLabel(0, "weewfewg");

	ui->autosearchCB->setVisible(false);
	ui->panoramaCB->setVisible(false);
//	ui->getSpectrumCB->setVisible(false);

	connect(ui->cbSonogram, SIGNAL(clicked(bool)), m_plot, SLOT(setVisible(bool)));
}

SpectrumWidget::~SpectrumWidget()
{
}

void SpectrumWidget::initCustomPlot(QCustomPlot* plot)
{
	plot->setBackground(QBrush(Qt::black));
	plot->xAxis->setTickPen(QPen(Qt::white));
	plot->yAxis->setTickPen(QPen(Qt::white));

	plot->axisRect()->setAutoMargins(QCP::msNone);
	plot->axisRect()->setMargins( QMargins(49, 0, 0, 0) );

	plot->setMinimumHeight(300);
	plot->setMaximumHeight(500);
	plot->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
	plot->xAxis->setRange(0, 20);
	plot->yAxis->setRange(0, 10);

	plot->xAxis->setAutoTicks(false);
	plot->xAxis->setAutoTickStep(false);
	plot->xAxis->setTickLabels(false);

	plot->setBackground(QBrush(Qt::black));
	plot->xAxis->setTickPen(QPen(QColor(155,255,255)));
	plot->xAxis->setLabelColor(QColor(155,255,255));
	plot->xAxis->setTickLabelColor(QColor(155,255,255));

	plot->yAxis->setTickPen(QPen(QColor(155,255,255)));
	plot->yAxis->setBasePen(QPen(QColor(155,255,255)));
	plot->yAxis->setTickLabelColor(QColor(155,255,255));
	plot->yAxis->setTickLabels(true);


	QPen gridPen;
	gridPen.setStyle(Qt::DotLine);
	gridPen.setColor(QColor(0,60,0));

	plot->xAxis->grid()->setPen(gridPen);
	plot->yAxis->grid()->setPen(gridPen);

	plot->xAxis->setTickStep(2);

	QVector<double> tick;
	for(int i = 0; i<20; i++) {
		tick.append(i);
	}
	plot->xAxis->setTickVector(tick);

	QVector<double> ytick;
	for(int i = 0; i<10; i++) {
		ytick.append(i);
	}
	plot->yAxis->setTickVector(ytick);
}

void SpectrumWidget::setAutoSearch(bool checked)
{
	ui->autosearchCB->setChecked(checked);
}

void SpectrumWidget::setPanorama(bool checked)
{
	//ui->panoramaCB->setChecked(checked);
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
}

Prm300ControlWidget *SpectrumWidget::getPrm300Widget()
{
	return ui->PrmControlWidget;
}

void SpectrumWidget::sonogramUpdate(const QPixmap &px)
{
	if(m_plot->isVisible()) {
		m_plotPixmap->setPixmap(px);
		double start_hz = 0;
		double end_hz = 0;
		double start_n = 0;
		double end_n = 0;
		ui->graphicsWidget->GetCurrentViewport(start_hz, start_n, end_hz, end_n);
		onZoomSonogram(start_hz, end_hz, true);
	}
}

bool SpectrumWidget::isSonogram()
{
	return ui->cbSonogram->isChecked();
}

void SpectrumWidget::setAnalysisDetectedData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData &msg)
{
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

void SpectrumWidget::slotSetWorkMode(int mode, bool isOn)
{
	if(mode == 2) {
		m_plot->setVisible(true);
	} else {
		m_plot->setVisible(false);
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

void SpectrumWidget::setNoSignal(bool b)
{
	if( b ) {
		ui->graphicsWidget->setVisible(true);
		ui->labelNo->setVisible(false);
	}  else {
		ui->graphicsWidget->setVisible(false);
		ui->labelNo->setVisible(true);

		m_plot->setVisible(false);
	}
}

void SpectrumWidget::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
