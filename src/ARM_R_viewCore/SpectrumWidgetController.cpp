#include "SpectrumWidgetController.h"
#include "SpectrumWidget.h"

#include "Rpc/RpcDefines.h"

//DoubleClick waiting time
#define TIMER_CLICK_DELAY 500

SpectrumWidgetController::SpectrumWidgetController(QObject *parent) : QObject(parent)
{
    m_dbManager = NULL;
    m_rpcClient = NULL;
	m_current_frequency = 0;
	m_autoSearch = false;

	m_rett = -100;
	m_threshold = -1000;

	m_enableCorrelation = false;
	m_bandwidth = 0;
	m_pointCount = 0;
	m_isComplex = false;

	m_peakVisible = false;

	m_id = 0;
	m_tab = NULL;

	m_graphicsWidget = NULL;
	m_graphicsContextMenu = NULL;
	flagDoubleClick = false;

	m_clickDelay = new QTimer(this);
	m_clickDelay->setInterval(TIMER_CLICK_DELAY);
	connect(m_clickDelay, SIGNAL(timeout()), this, SLOT(processClick()));
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
	if (PointCount < 5) {
		QMessageBox::critical(m_view, tr("File error"), tr("Very small file or file not found"));
		return;
	}
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

Q_DECLARE_METATYPE(float*)
void SpectrumWidgetController::onDataArrived(const QString &method, const QVariant &arg)
{
	if(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION == method) {
		setDefModulation(arg.toString());
		return;
	}

	if(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH == method) {
		setDetectedAreasUpdate(arg.toByteArray());
		return;
	}

	if (RPC_SLOT_SERVER_SEND_POINTS == method) {
		QList<QVariant> list = arg.toList();

		float* spectrum = list.at(0).value<float*>();
		float* spectrumPeakHold = (float*)list.at(1).value<float*>();

		if (list.count() == 2){
			setSignal(spectrum, spectrumPeakHold);
		} else {
			int pointCount = list.at(2).toInt();
			double bandwidth = list.at(3).toDouble();
			bool isComplex = list.at(4).toBool();
			setSignalSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
		}

		return;
	}
}

void SpectrumWidgetController::setRpcClient(RPCClient *rpcClient)
{
	m_rpcClient = rpcClient;
	m_prm300WidgetController->setRpcClient(m_rpcClient);
	m_rpcClient->registerReceiver(m_prm300WidgetController);
}

QString SpectrumWidgetController::getSpectrumName() const
{
	return m_view->getSpectrumName();
}

QWidget *SpectrumWidgetController::getWidget() const
{
	return m_view;
}

void SpectrumWidgetController::setSignalSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex)
{
	m_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;
	m_mux.unlock();

	setFFTSetup(spectrum, spectrum_peak_hold);
}

void SpectrumWidgetController::setFFTSetup(float* spectrum, float* spectrum_peak_hold)
{
	m_graphicsWidget->SetSpectrumVisible(2, m_peakVisible);

	float maxv = 0.0;
	float minv = 0.0;
	m_mux.lock();

	m_graphicsWidget->SetAutoscaleY(false);
	m_graphicsWidget->SetAlign(0);
	//ui->graphicsWidget->SetZeroFrequencyHz(spectrum[0]/* + bandwidth*/);

	m_isComplex = false;
	m_graphicsWidget->Setup(m_isComplex, m_bandwidth, tr("Level"),
							spectrum, m_pointCount, spectrum_peak_hold, m_pointCount, false, false, minv, maxv);
	m_mux.unlock();

}
void SpectrumWidgetController::setSignal(float *spectrum, float *spectrum_peak_hold)
{
	m_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;

	m_graphicsWidget->SetAutoscaleY(false);
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
		QMessageBox::warning(m_view, tr("Attention!"), tr("Signal was detected!"),
									   QMessageBox::Cancel, QMessageBox::Ok);
		m_rett = -101;

	}
	m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
	/// Next line creates new Scale event to widget which exec grid moving
	//m_graphicsWidget->ZoomOutY();
}

void SpectrumWidgetController::setDefModulation(QString modulation)
{
	m_graphicsWidget->SetLabel(m_centerFreqDefModulation*1000, modulation);
}

bool SpectrumWidgetController::isGraphicVisible()
{
	return m_view->isVisible();
}

quint32 SpectrumWidgetController::getId()
{
	return m_id;
}

void SpectrumWidgetController::setLabelName(QString base, QString second)
{
	m_graphicsWidget->SetLabel(m_bandwidth/2, base);
}

void SpectrumWidgetController::setDetectedAreasUpdate(const QByteArray &vecBA)
{
	if(m_autoSearch == false){
		return;
	}

	unsigned int id; // m_header.id

	QVector<QPointF> vec;
	QDataStream stream(vecBA);

	stream >> id; // m_header.id

	if (id != m_id) {
		return;
	}

	stream >> vec;

	m_graphicsWidget->ClearAllDetectedAreas();
	QVector<QPointF>::iterator it;
	for(it = vec.begin(); it != vec.end(); ++it){
		m_graphicsWidget->SetDetectedAreas((*it).x()*TO_MHZ + m_current_frequency, 0, (*it).y()*TO_MHZ + m_current_frequency, 0, false);
	}
}

void SpectrumWidgetController::setZeroFrequency(double val)
{
	double cur_freq = m_tab->getCurrentFrequency();
	m_current_frequency = cur_freq*TO_MHZ;
	m_graphicsWidget->SetZeroFrequencyHz(val + m_current_frequency);
}

void SpectrumWidgetController::setVisible(const bool isVisible)
{
	m_view->setVisible(isVisible);
}

void SpectrumWidgetController::setAutoSearch(bool enabled)
{
	m_view->setAutoSearch(enabled);
}

void SpectrumWidgetController::setPanorama(bool enabled)
{
	m_view->setPanorama(enabled);
}

void SpectrumWidgetController::setSelection(double start, double end)
{
    m_graphicsWidget->SetSelection(start*TO_MHZ, 0, end*TO_MHZ, 0);
}

void SpectrumWidgetController::setDbManager(IDbManager *dbManager)
{
    m_dbManager = dbManager;
    m_prm300WidgetController->setDbManager(m_dbManager);
}

void SpectrumWidgetController::setDbStationController(DBStationController *controller)
{
    m_dbStationController = controller;
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

	m_graphicsWidget->setContextMenu(m_graphicsContextMenu);

	connect(m_graphicsWidget, SIGNAL(SelectionCleared()), m_clickDelay, SLOT(start()));
	connect(m_graphicsWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(m_graphicsWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));
	connect(m_graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(slotDoubleClicked(double, double)));
	connect(this, SIGNAL(signalCurSelChanged(int)), m_graphicsWidget, SLOT(slotCurSelectionChanged(int)));

	connect(m_view, SIGNAL(setPanoramaSignal(bool)), this, SLOT(slotSetEnablePanorama(bool)));
	connect(m_view, SIGNAL(setAutoSearchSignal(bool)), this, SLOT(slotAutoSearch(bool)));
	connect(m_view, SIGNAL(selectionTypeChangedSignal(bool)), this, SLOT(slotSelectiontypeChanged(bool)));
	connect(m_view, SIGNAL(requestDataSignal(bool)), this, SLOT(slotRequestData(bool)));

	connect(m_view, SIGNAL(setShowPeaksSignal(bool)), this, SLOT(slotShowPeaks(bool)));
	connect(m_view, SIGNAL(setShowControlPRM(bool)), this, SLOT(slotShowControlPRM(bool)));

    m_prm300WidgetController = new Prm300ControlWidgetController(m_view->getSpectrumName(), this);
	m_prm300WidgetController->appendView(m_view->getPrm300Widget());
}

void SpectrumWidgetController::slotSetEnablePanorama(bool state)
{
	m_tab->setPanorama(state);
}

void SpectrumWidgetController::slotAutoSearch(bool state)
{
	m_autoSearch = state;

	if (!m_autoSearch) {
		m_graphicsWidget->ClearAllDetectedAreas();
	}
}

void SpectrumWidgetController::slotSelectiontypeChanged(bool state)
{
	if(state){
		emit signalCurSelChanged(4);
		return;
	}

	emit signalCurSelChanged(1);

	m_threshold = -1000;
}

void SpectrumWidgetController::slotRequestData(bool state)
{
	int data[4] = {0, 1, 2, 3};
	if(state){
		emit signalRequestData(m_id, 0, &data[0], 4);
	}
}

/// add selection to white list
void SpectrumWidgetController::slotCMAddWhiteList()
{
    StationData data;
    data.stationName = m_view->getSpectrumName();
    data.port = 155;
    data.category = "White";
    data.signalType = "Unknown";
    data.frequency = m_centerFreqSelTemp;
    data.bandwidth= m_bandwidhtTemp;

    int index = m_dbStationController->addStationData(data);
	emit signalAddSelToLists(1);
}

/// add selection to black list
void SpectrumWidgetController::slotCMAddBlackList()
{
    StationData data;
    data.stationName = m_view->getSpectrumName();
    data.port = 155;
    data.category = "Black";
    data.signalType = "Unknown";
    data.frequency = m_centerFreqSelTemp;
    data.bandwidth= m_bandwidhtTemp;

    int index = m_dbStationController->addStationData(data);

	emit signalAddSelToLists(2);
}

/// signal for flakon to recognize signal
void SpectrumWidgetController::slotRecognizeSignal()
{
	/// TODO: recheck new Message to memory leak
	m_centerFreqDefModulation = m_centerFreqSelTemp;
	CommandMessage *msg = new CommandMessage(COMMAND_RECOGNIZESIGNAL, QVariant());
	m_tab->sendCommand(TypeGraphicCommand,msg);
}

/// signal for flakon to recognize signal
void SpectrumWidgetController::slotSSCorrelation()
{
	/// TODO: recheck new Message to memory leak
	m_enableCorrelation = !m_enableCorrelation;
	CommandMessage *msg = new CommandMessage(COMMAND_KM, m_enableCorrelation);
	m_tab->sendCommand(TypeGraphicCommand,msg);
	if(m_enableCorrelation)
		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	else
		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
}

void SpectrumWidgetController::slotClearLabels()
{
	m_graphicsWidget->ClearAllLabels();
}

void SpectrumWidgetController::slotSelectionCleared()
{
	double coordinateZero = (double)0;
	SpectrumSelection selection;
	selection.start = QPointF(coordinateZero, coordinateZero);
	selection.end = QPointF(coordinateZero, coordinateZero);

	m_tab->setSelectedArea(selection);
}

void SpectrumWidgetController::slotSelectionFinished(double x1, double y1, double x2, double y2)
{
	m_clickDelay->stop();
	///TODO: fix

	/// To HZ
	x1 /= 1000;
	x2 /= 1000;

    m_centerFreqSelTemp = (x1 + x2)/2;
    m_bandwidhtTemp = qAbs(x2-x1);

	SpectrumSelection selection;
	selection.start = QPointF(x1, y1);
	selection.end = QPointF(x2, y2);

	m_tab->setSelectedArea(selection);
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

void SpectrumWidgetController::slotDoubleClicked(double, double)
{
	flagDoubleClick = true;
	emit doubleClickedSignal(m_id);
}

void SpectrumWidgetController::slotShowPeaks(bool visible)
{
	m_graphicsWidget->SetSpectrumVisible(2, visible);
	m_peakVisible = visible;
}

void SpectrumWidgetController::slotShowControlPRM(bool state)
{
	m_tab->setShowControlPrm(state);
	switch(state)
	{
		case false:
			m_view->getPrm300Widget()->hide();
			break;
		case true:
			m_view->getPrm300Widget()->show();
			break;
		default:
			break;
	}
}

void SpectrumWidgetController::processClick()
{
	m_clickDelay->stop();
	if(!flagDoubleClick) {
		slotSelectionCleared();
	}

	flagDoubleClick = false;
}
