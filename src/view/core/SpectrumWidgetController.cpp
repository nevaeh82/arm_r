#include "SpectrumWidgetController.h"
#include "SpectrumWidget.h"

#include "Rpc/RpcDefines.h"

#include "DBStation/StationHelper.h"

SpectrumWidgetController::SpectrumWidgetController(QObject *parent) : QObject(parent)
{
	m_dbManager = NULL;
	m_rpcClient = NULL;

	m_prm300WidgetController = NULL;

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

	m_controlPanelController = NULL;
	nextClearState = false;

	m_spectrumShow = true;
	correlationFlag = false;

	m_controlPanelMode = -1;
	m_stopFlag = true;

	m_sigDialog = NULL;

	connect(this, SIGNAL(signalVisible(bool)), this, SLOT(onVisible(bool)));

	connect(this, SIGNAL(onCorrelationStateChangedSignal(bool)), this, SLOT(onCorrelationStateChangedSlot(bool)));
}

SpectrumWidgetController::~SpectrumWidgetController()
{
}

void SpectrumWidgetController::appendView(SpectrumWidget* view)
{
	m_view = view;
	m_sigDialog = new SignalDetectedDialog(m_view);

	//m_sigDialog->hide();


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
	m_name = name;
	m_view->setSpectrumName(name);

	if(m_prm300WidgetController) {
		m_prm300WidgetController->setName(m_name);
	}
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


		//log_debug(QString("Setup Spectrum : %1").arg(m_name));

		if (list.count() == 2){
			setSignal(spectrum, spectrumPeakHold);
		} else {
			int pointCount = list.at(2).toInt();
			double bandwidth = list.at(3).toDouble();
			bool isComplex = list.at(4).toBool();

			setSignalSetup(spectrum, spectrumPeakHold, pointCount, bandwidth, isComplex);
		}
		//m_graphicsWidget->ZoomOutFull();

		return;
	}
}

void SpectrumWidgetController::setRpcPrmClient(RpcPrmClient *rpcClient)
{
	m_rpcClient = rpcClient;
	m_prm300WidgetController->setRpcPrmClient(m_rpcClient);
	m_rpcClient->registerReceiver(m_prm300WidgetController);
}

void SpectrumWidgetController::onCorrelationStateChanged(const bool isEnabled)
{
	emit onCorrelationStateChangedSignal(isEnabled);
}

void SpectrumWidgetController::onCorrelationStateChangedSlot(const bool isEnabled)
{
	correlationFlag = isEnabled;

	if(correlationFlag) {
		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	}
	else {
		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
	}
}

void SpectrumWidgetController::updateDBAreas()
{
	//m_graphicsWidget->ClearAllDetectedAreas();

	QList<StationsFrequencyAndBandwith> list;
	bool ret = m_dbStationController->getFrequencyAndBandwidthByCategory("Black", list );
	setDetectedAreas(2, list);

	ret = m_dbStationController->getFrequencyAndBandwidthByCategory("White", list );
	setDetectedAreas(1, list);

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
	if(m_stopFlag == false)
	{
		return;
	}

	m_mux.lock();
	m_bandwidth = bandwidth;
	m_pointCount = PointCount;
	m_isComplex = isComplex;
	m_mux.unlock();

	setFFTSetup(spectrum, spectrum_peak_hold);
}

void SpectrumWidgetController::setFFTSetup(float* spectrum, float* spectrum_peak_hold)
{
	if(m_stopFlag == false)
	{
		return;
	}

	m_rpcClient->requestFrequency();
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

	QVariant value = m_dbManager->getPropertyValue(getSpectrumName(), DB_FREQUENCY_PROPERTY);
	setZeroFrequency(value.toDouble());

	m_mux.unlock();

}
void SpectrumWidgetController::setSignal(float *spectrum, float *spectrum_peak_hold)
{
	if(!m_spectrumShow)
		return;

	m_mux.lock();
	float maxv = 0.0;
	float minv = 0.0;

	m_graphicsWidget->SetAutoscaleY(false);
	m_graphicsWidget->SetAlign(0);
	m_mux.unlock();

	if(m_controlPanelMode == 3)
	{
		m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);
		m_graphicsWidget->ZoomOutFull();
		return;
	}


	if((m_threshold != -1000) && (m_rett == -100))
	{
		QList<StationsFrequencyAndBandwith> list;

		if(m_controlPanelMode == 2)
		{
			bool ret = m_dbStationController->getFrequencyAndBandwidthByCategory("White", list );
		}

		for(int i = 0; i < m_pointCount; i++)
		{
			double overthreshold = ((m_current_frequency / TO_MHZ) -10) + ((m_bandwidth / TO_MHZ)/m_pointCount)*i;

			if((spectrum[i] > m_threshold) && (m_rett != -99))
			{
				if(m_controlPanelMode == 2)
				{
					foreach (StationsFrequencyAndBandwith st, list) {
						if((overthreshold > (st.frequency - st.bandwidth/2)) && (overthreshold < (st.frequency + st.bandwidth/2))){
							m_overthreshold.append(overthreshold);
						}
					}
				}else {
					m_overthreshold.append(overthreshold);
				}
				m_rett = 0;
			}
		}
	}

	//TODO: Write signals values over m_threshold. Task 6186
	if(!m_overthreshold.isEmpty()) {

		setSpectrumShow(false);
		m_stopFlag = false;

		m_sigDialog->setFrequency(m_name, m_overthreshold);

		int result = m_sigDialog->exec();

		m_overthreshold.clear();
		m_rett = -100;

		if(result == QDialog::Accepted){
			setSpectrumShow(true);
			m_stopFlag = true;

		}
		else {
			setSpectrumShow(false);
			m_stopFlag = false;
			return;
		}
	}

	m_graphicsWidget->PermanentDataSetup(spectrum, spectrum_peak_hold, minv, maxv);

	m_graphicsWidget->ZoomOutFull();
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

	//m_graphicsWidget->ClearAllDetectedAreas();
	QVector<QPointF>::iterator it;
	for(it = vec.begin(); it != vec.end(); ++it){
		m_graphicsWidget->SetDetectedAreas(3, (*it).x()*TO_MHZ /*+ m_current_frequency*/, 0, (*it).y()*TO_MHZ /*+ m_current_frequency*/, 0, false);
	}
}

void SpectrumWidgetController::setSpectrumShow(bool state)
{
	m_spectrumShow = state;
	//emit signalSpectrumEnable(state);

	m_view->slotSetEnableSpactrum(state);
	m_stopFlag = state;
}

void SpectrumWidgetController::setDetectedAreas(int mode, const QList<StationsFrequencyAndBandwith>& list)
{
	double zeroFreq = m_current_frequency - m_bandwidth/2;

	foreach (StationsFrequencyAndBandwith value, list) {
		double beginFreq = (value.frequency - value.bandwidth/2)*TO_MHZ;
		double endFreq = (value.frequency + value.bandwidth/2)*TO_MHZ;
		if(endFreq < zeroFreq)
			continue;
		if(beginFreq > zeroFreq + m_bandwidth)
			continue;

		if(beginFreq < zeroFreq)
			beginFreq = zeroFreq;
		if(endFreq > zeroFreq + m_bandwidth)
			endFreq = zeroFreq + m_bandwidth;

		m_graphicsWidget->SetDetectedAreas(mode, beginFreq, 0, endFreq, 0, false);
	}
}

void SpectrumWidgetController::setZeroFrequency(double val)
{
	if(m_stopFlag == false)
	{
		return;
	}
	//m_graphicsWidget->ClearAllDetectedAreas();
	updateDBAreas();

	if(m_current_frequency == val*TO_MHZ) {
		return;
	}

	m_current_frequency = val*TO_MHZ;
	double zeroFreq = m_current_frequency - m_bandwidth/2;

	//If not activated spectrum - laggy counting of zero frequency ???
	if(m_graphicsWidget->isActivated()) {
		m_graphicsWidget->SetZeroFrequencyHz(zeroFreq);
	}

	QList<StationsFrequencyAndBandwith> list;
	bool ret = m_dbStationController->getFrequencyAndBandwidthByCategory("Black", list );
	setDetectedAreas(2, list);

	ret = m_dbStationController->getFrequencyAndBandwidthByCategory("White", list );
	setDetectedAreas(1, list);
}

void SpectrumWidgetController::setVisible(const bool isVisible)
{
	emit signalVisible(isVisible);
}

void SpectrumWidgetController::onVisible(const bool b) {
	m_view->setVisible(b);
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
	double start_x, start_y, end_x, end_y;
	m_graphicsWidget->GetCurrentViewport(start_x, start_y, end_x, end_y);
	if( (start*TO_MHZ > start_x) && (end*TO_MHZ < end_x) ) {
		m_graphicsWidget->SetSelection(start*TO_MHZ, 0, end*TO_MHZ, 0);
	}
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

void SpectrumWidgetController::setControlPanelController(ControlPanelController *controller)
{
	m_controlPanelController = controller;
	connect(m_controlPanelController, SIGNAL(signalSetMode(int)), this, SLOT(slotControlPanelMode(int)));

	connect(m_controlPanelController, SIGNAL(onSignalWorkMode(int,bool)), this, SIGNAL(onSignalSetWorkMode(int,bool)));
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

	connect(m_graphicsContextMenu->actions().at(0),SIGNAL(triggered()),SLOT(addToWhiteList()));
	connect(m_graphicsContextMenu->actions().at(1),SIGNAL(triggered()),SLOT(addToBlackList()));
	connect(m_graphicsContextMenu->actions().at(2),SIGNAL(triggered()),SLOT(recognizeSignal()));
	connect(m_graphicsContextMenu->actions().at(3),SIGNAL(triggered()),SLOT(toggleCorrelation()));
	connect(m_graphicsContextMenu->actions().at(4),SIGNAL(triggered()),SLOT(clearLabels()));
	connect(m_graphicsContextMenu, SIGNAL(aboutToShow()), this, SLOT(slotIsShowContextMenu()));

	m_graphicsWidget->setContextMenu(m_graphicsContextMenu);

	connect(m_graphicsWidget, SIGNAL(SelectionCleared()), this, SLOT(slotSelectionCleared()));
	connect(m_graphicsWidget, SIGNAL(SelectionFinished(double,double,double,double)), this, SLOT(slotSelectionFinished(double,double,double,double)));
	connect(m_graphicsWidget, SIGNAL(selectionFinishedRedLine(double)), this, SLOT(slotSelectionFinishedRedLine(double)));
	connect(m_graphicsWidget, SIGNAL(DoubleClicked(double,double)), this, SLOT(slotDoubleClicked(double, double)));
	connect(this, SIGNAL(signalCurSelChanged(int)), m_graphicsWidget, SLOT(slotCurSelectionChanged(int)));

	connect(m_view, SIGNAL(setPanoramaSignal(bool)), this, SLOT(slotSetEnablePanorama(bool)));
	connect(m_view, SIGNAL(setAutoSearchSignal(bool)), this, SLOT(slotAutoSearch(bool)));
	connect(m_view, SIGNAL(selectionTypeChangedSignal(bool)), this, SLOT(slotSelectiontypeChanged(bool)));
	connect(m_view, SIGNAL(requestDataSignal(bool)), this, SLOT(slotRequestData(bool)));
	connect(this, SIGNAL(signalSpectrumEnable(bool)), m_view, SLOT(slotEnableKM(bool)));

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
	if(m_spectrumShow == state)
		return;

	setSpectrumShow(state);


	//	int data[4] = {0, 1, 2, 3};
	//	if(state){
	//		emit signalRequestData(m_id, 0, &data[0], 4);
	//	}
}

/// add selection to white list
void SpectrumWidgetController::addToWhiteList()
{
	StationData data;
	data.stationName = m_view->getSpectrumName();
	data.port = 155;
	data.category = "White";
	data.signalType = "Unknown";
	data.frequency = m_centerFreqSelTemp;
	data.bandwidth= m_bandwidhtTemp;

	StationData dataExist = m_dbStationController->
			getStationData( data.stationName, data.port, data.frequency, data.bandwidth );

	if( !dataExist.stationName.isNull() ) {
		StationHelper::showStationExistsWarning( dataExist, m_view );
		return;
	}

	int result = m_dbStationController->addStationData(data);

	if( result < 1 ) {
		StationHelper::showStationWarning( result, m_view );
		return;
	}

	emit signalAddSelToLists(1);

	m_tab->updateDbStationsLists();
}

/// add selection to black list
void SpectrumWidgetController::addToBlackList()
{
	StationData data;
	data.stationName = m_view->getSpectrumName();
	data.port = 155;
	data.category = "Black";
	data.signalType = "Unknown";
	data.frequency = m_centerFreqSelTemp;
	data.bandwidth= m_bandwidhtTemp;

	StationData dataExist = m_dbStationController->
			getStationData( data.stationName, data.port, data.frequency, data.bandwidth );

	if( !dataExist.stationName.isNull() ) {
		StationHelper::showStationExistsWarning( dataExist, m_view );
		return;
	}

	int result = m_dbStationController->addStationData(data);

	if( result < 1 ) {
		StationHelper::showStationWarning( result, m_view );
		return;
	}

	emit signalAddSelToLists(2);
	m_tab->updateDbStationsLists();

}

/// signal for flakon to recognize signal
void SpectrumWidgetController::recognizeSignal()
{
	m_tab->recognize();
}

/// signal for flakon to recognize signal
void SpectrumWidgetController::toggleCorrelation()
{
	/// TODO: recheck new Message to memory leak
	//	m_enableCorrelation = !m_enableCorrelation;
	m_tab->enableCorrelation( !correlationFlag );


	//	if(m_enableCorrelation)
	//		m_graphicsContextMenu->actions().at(3)->setText(tr("Disable correlation"));
	//	else
	//		m_graphicsContextMenu->actions().at(3)->setText(tr("Enable correlation"));
}

void SpectrumWidgetController::clearLabels()
{
	m_graphicsWidget->ClearAllLabels();
}

void SpectrumWidgetController::slotSelectionCleared()
{
	double coordinateZero = (double)0;
	SpectrumSelection selection;
	selection.start = QPointF(coordinateZero, coordinateZero);
	selection.end = QPointF(coordinateZero, coordinateZero);

	//Setting null selection
	m_graphicsWidget->SetSelection(selection.start.x(), selection.start.y(), selection.end.x(), selection.end.y());
	slotSelectionFinished(selection.start.x(), selection.start.y(), selection.end.x(), selection.end.y());

	m_tab->setSelectedArea(selection);
	m_threshold = -1000;
}

void SpectrumWidgetController::slotSelectionFinished(double x1, double y1, double x2, double y2)
{
	///TODO: fix

	double x1count = x1 / TO_MHZ;
	double x2count = x2 / TO_MHZ;

	/// To HZ
	x1 /= TO_MHZ2;
	x2 /= TO_MHZ2;

	m_centerFreqSelTemp = (x1count + x2count)/2;
	m_bandwidhtTemp = qAbs(x2count-x1count);

	SpectrumSelection selection;
	selection.start = QPointF(x1, y1);
	selection.end = QPointF(x2, y2);

	//if no selection
	if(x1 == 0 && x2 ==0 && y1 == 0 && y2 == 0) {
		if(!nextClearState) {
			nextClearState = true;
		}
		else {
			double coordinateZero = (double)0;
			tmpSelection.start = QPointF(coordinateZero, coordinateZero);
			tmpSelection.end = QPointF(coordinateZero, coordinateZero);
		}
	}
	//if any selection exist
	else {
		tmpSelection = selection;
		nextClearState = false;
	}

	m_tab->setSelectedArea(selection);
}

void SpectrumWidgetController::slotSelectionFinishedRedLine(double y)
{
	m_threshold = y;
	emit signalChoosedThreshold(y);
}

void SpectrumWidgetController::slotIsShowContextMenu()
{
	QList<QPointF> selectionList = m_graphicsWidget->GetAllHorSelections();
	if(selectionList.isEmpty()
			|| (selectionList.count() == 1 && selectionList.first().x() == 0
				&& selectionList.first().y() == 0))
		m_graphicsWidget->contextMenu()->setEnabled(false);
	else
		m_graphicsWidget->contextMenu()->setEnabled(true);
}

void SpectrumWidgetController::slotDoubleClicked(double a, double b)
{
	//Create selection back when doubleClicked
	m_graphicsWidget->SetSelection(tmpSelection.start.x()*1000, 0, tmpSelection.end.x()*1000, 0);
	slotSelectionFinished(tmpSelection.start.x()*1000, 0, tmpSelection.end.x()*1000, 0);
	nextClearState = false;

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
		case true:
			m_view->getPrm300Widget()->show();
			break;

		default:
			m_view->getPrm300Widget()->hide();
			break;
	}
}

void SpectrumWidgetController::slotControlPanelMode(int mode)
{
	m_controlPanelMode = mode;
}
