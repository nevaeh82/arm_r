#include "SpectrumWidgetDataSource.h"
#include "Rpc/RpcDefines.h"

#include <stddef.h>

#define TO_KHZ 1000
#define TO_HZ				1000000
#define BANDWIDTH_SINGLE	20000000

#define SLEEP_MODE_TIMEOUT 10000

SpectrumWidgetDataSource::SpectrumWidgetDataSource(IGraphicWidget *widget, QObject *parent)
	: BaseDataSource(parent)
	, m_pointCountWhole(0)
	, m_needSetup(0)
	, m_currentFreq(0)
	, m_endFreq(0)
	, m_responseFreq(0)
	, m_startx(0)
	, m_tabManager(NULL)
	, m_spectrumCounter(0)
	, m_dbmanager(NULL)
	, m_workFreq(0)
	, m_panoramaFreqControl(NULL)
	, m_id(-1)
	, m_sleepModeProcess(true)
{
	m_spectrumWidget = widget;

	m_sonogramTime = QTime::currentTime();

	m_bandwidth = 0;
	m_pointCount = 0;
	m_bandwidthSingleSample = 0;
	m_isPanoramaStart = false;
	m_needSetupSpectrum = true;

	m_spectrum = new float[1];
	m_spectrumPeakHold = new float[1];

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)), Qt::QueuedConnection);
	//connect(&m_timerChangeFreqPanorama, SIGNAL(timeout()), this, SLOT(slotChangeFreq()));
	//connect(&m_timerRepeatSetFreq, SIGNAL(timeout()), this, SLOT(slotRepeatSetFrequency()));

	m_sleepModeTimer = new QTimer(this);
	m_sleepModeTimer->setInterval(SLEEP_MODE_TIMEOUT);
	m_sleepModeTimer->setSingleShot(true);
	connect(m_sleepModeTimer, SIGNAL(timeout()), SLOT(onSleepModeSlot()));
}

SpectrumWidgetDataSource::~SpectrumWidgetDataSource()
{
	delete[] m_spectrum;
	delete[] m_spectrumPeakHold;
}

Q_DECLARE_METATYPE(float*)
void SpectrumWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	emit onMethodCalledSignal(method, data);
}

int SpectrumWidgetDataSource::dataProccess(QVector<QPointF>& vecFFT, bool)
{
	m_pointCount = vecFFT.size();
	QPointF vecFFT_0 = vecFFT.at(0);

	qreal startx = vecFFT_0.x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = (endx - startx)*TO_KHZ;

	if(m_startx != m_responseFreq)
	{
		m_startx = m_responseFreq;
	}

	if(m_bandwidthSingleSample != bandwidth && m_isPanoramaStart == false)
	{
		setBandwidth(bandwidth);
		m_bandwidthSingleSample = bandwidth;
		m_needSetup = true;
	}

	int index;
	if(m_isPanoramaStart)
	{
		index = findIndex(m_startx);
		log_debug(QString("PANORAMA INDEX %1").arg(index));
	}
	else
	{
		index = 0;
	}

	if( index < 0 ) {
		return -1;
	}

	for(int i = 0; i < vecFFT.size(); i++)
	{
		m_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();

		if((m_startx != m_responseFreq) || (m_spectrum[index*vecFFT.size()+i] >
											m_spectrumPeakHold[index*vecFFT.size()+i]) ||
				(m_spectrumPeakHold[index*vecFFT.size()+i] == 0))
		{
			m_spectrumPeakHold[index*vecFFT.size()+i] =
					m_spectrum[index*vecFFT.size()+i];
		}
	}

	return index;
}

int SpectrumWidgetDataSource::findIndex(qreal startx)
{
	return m_listStartx.indexOf( startx );
}

bool SpectrumWidgetDataSource::startPanorama(bool start)
{
	if(start)
	{
		//if(m_spectrumWidget != NULL)
			// m_spectrumWidget->setZeroFrequency(m_startFreq);

			m_currentFreq = m_startFreq;

		log_info(QString("Current frequency is %1").arg(m_currentFreq));
		if(m_dbmanager != NULL) {
			//m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);
		}
		return true;
	}

	return false;
	/// start timer for loop panorama frequency
}

void SpectrumWidgetDataSource::setBandwidth(double bandwidth)
{
	//log_debug( QString("BANDWIDTH %1 :::::::").arg(bandwidth) );

	if(m_bandwidth != bandwidth)
	{
		m_spectrumWidget->setAlarm(true);

		m_bandwidth = bandwidth;
		int div = m_bandwidth / BANDWIDTH_SINGLE;
		if( !div ) {
			div += 1;
		}
		m_pointCountWhole = m_pointCount*div;

		delete[] m_spectrum;
		m_spectrum = new float[m_pointCountWhole] ();

        delete[] m_spectrumPeakHold;
        m_spectrumPeakHold = new float[m_pointCountWhole] ();
        m_needSetupSpectrum = true;
        clearPeak();

	}
}

void SpectrumWidgetDataSource::clearPeak()
{
    for(int i = 0; i<m_pointCountWhole; i++) {
        m_spectrumPeakHold[i] = 0;
    }
}

void SpectrumWidgetDataSource::setBandwidth1(int size)
{
	delete[] m_spectrum;
	m_spectrum = new float[size] ();

	delete[] m_spectrumPeakHold;
	m_spectrumPeakHold = new float[size] ();
	m_needSetupSpectrum = true;
}

void SpectrumWidgetDataSource::setPanorama(bool enabled, double start, double end)
{
	m_isPanoramaStart = enabled;

	if (!m_isPanoramaStart){
		startPanorama(enabled);
        //log_debug("panorama stopped");
		setBandwidth(m_bandwidthSingleSample);
		return;
	}

	log_debug(QString("panorama started = %1 %2").arg(start).arg(end));

	if (start > end) {
		return;
	}

	double bandwidth = end - start + 20;
	if(bandwidth < 20) {
		bandwidth = 20;
	}

	bandwidth *= TO_HZ;
	setBandwidth(bandwidth);


	m_currentFreq = start;
	m_startFreq = start;
	m_endFreq = end;

	m_listStartx.clear();

	//Fill startX list for indexes
	for(int i = m_startFreq; i <= m_endFreq; i+=20) {
		m_listStartx.append( i );
	}

	m_panoramaFreqControl->init( m_startFreq, m_endFreq );
	m_spectrumCounter = 0;

	startPanorama(enabled);
}

bool SpectrumWidgetDataSource::isPanoramaEnabled()
{
	return m_isPanoramaStart;
}

void SpectrumWidgetDataSource::setTabManager(ITabManager *manager)
{
	m_tabManager = manager;
}

void SpectrumWidgetDataSource::setDBManager(IDbManager *manager)
{
	m_dbmanager = manager;

}

void SpectrumWidgetDataSource::setName(QString name)
{
	m_name = name;
}

void SpectrumWidgetDataSource::setId(int id)
{
	m_id = id;
}

void SpectrumWidgetDataSource::setPanoramaFreqControl(PanoramaFreqControl* control)
{
	m_panoramaFreqControl = control;
}

void SpectrumWidgetDataSource::setupPoints(const RdsProtobuf::ServerMessage_OneShotData_LocationData& data)
{
	if(!m_spectrumWidget->isGraphicVisible() && !m_needSetupSpectrum) {
		emit onDrawComplete();
		return;
	}

	bool isComplex = true;
	int id = -1;
	int protoId = 0;
	float cf = 0;
	QVector<QPointF> vecFFT;
	QList<QVariant> list;

	cf = data.central_frequency();
	m_workFreq = cf;

	m_panelController->setCentralFreqValue(cf);

	if(cf != m_workFreq) {
		m_sleepModeProcess = true;
		m_sleepModeTimer->stop();
	}

	if( m_panelController->sleepMode() && !m_sleepModeProcess )
	{
		emit onDrawComplete();
		if(!m_sleepModeTimer->isActive()) {
			m_sleepModeTimer->start();
		}
		return;
	}


	if( data.detector_status_size() > m_spectrumWidget->getId() )
	{
		if( !data.detector_status(m_spectrumWidget->getId()) ) { //So it is no signal :)
			QVariant dataOut(list);
			m_spectrumWidget->setAlarm(false);
			onDataReceived(RPC_SLOT_SERVER_SEND_POINTS, dataOut);
			return;
		}
	}

	for(int i=0; i<data.spectrum_plot_size(); i++) {
		if( data.spectrum_plot(i).index() == m_spectrumWidget->getId() ) {
			id = m_spectrumWidget->getId();
			protoId = i;
			break;
		}
	}

	if( id < 0 ) {
		//emit onDrawComplete();
		return;
	}


	// =================================== Creating Spectrum vector Points ========================================

	double freq = data.spectrum_plot(protoId).axis_x_start();
	double stepFreq = data.spectrum_plot(protoId).axis_x_step();

	int val1 = -(qrand()%100);
	for(int i = 0; i < data.spectrum_plot(protoId).data().size(); i++) {
		double val = data.spectrum_plot(protoId).data(i);

		//Just Test
//		if(m_id == 1 && i > 1000 && i < 2000) {
//			val = val1;
//		}

		vecFFT.append( QPointF(freq*TO_KHZ, val) );

		freq += stepFreq;
	}


	if(!m_isPanoramaStart)
	{
		m_spectrumWidget->setZeroFrequency(cf);
		if(m_responseFreq != cf) {
			clearPeak();
		}
	} else {
		m_spectrumWidget->setZeroFrequency(m_startFreq);
	}

	if(m_responseFreq != cf) {
		m_spectrumWidget->clearSonogram();
	}

	m_responseFreq = cf;

	if (!vecFFT.size()) return;

	int index = dataProccess(vecFFT, isComplex);

	if(index < 0) {
		emit onDrawComplete();
		return;
	}

	QVariant spectrumVariant = QVariant::fromValue(m_spectrum);
	QVariant peaksSpectrumVariant = QVariant::fromValue(m_spectrumPeakHold);

	list.append(spectrumVariant);
	list.append(peaksSpectrumVariant);
	list.append(index);

	if(m_needSetupSpectrum)
	{
		QVariant pointsCountVariant(m_pointCountWhole);
		QVariant bandwidthVariant(m_bandwidth);
		QVariant isComplexVariant(isComplex);
		list.append(pointsCountVariant);
		list.append(bandwidthVariant);
		list.append(isComplexVariant);

		m_needSetupSpectrum = false;
	}

	m_sleepModeProcess = false;

	QVariant dataOut(list);

	m_spectrumWidget->setAlarm(false);

	onDataReceived(RPC_SLOT_SERVER_SEND_POINTS, dataOut);

	// ====================================== Creating Spectrum DetectorHumps ===========================================
	QList<QPointF> detectHumpsList;
	for(int i = 0; i < data.hump_size(); i++) {
		if(m_id == data.hump(i).detector_index()) {
			detectHumpsList.append( QPointF(data.hump(i).range().start(), data.hump(i).range().end()) );
		}
	}

	QByteArray humpData;
	QDataStream humpStream( &humpData, QIODevice::WriteOnly );
	humpStream << detectHumpsList;

	if( !detectHumpsList.isEmpty() ) {
		onDataReceived( RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH, QVariant(humpData) );
	}

	// ======================================= Some Panorama actions ====================================================

	if(m_isPanoramaStart)
	{
		m_responseFreq = cf;
		m_spectrumCounter++;
		//m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, cf);
	}
	m_workFreq = cf;


	if(m_isPanoramaStart)
	{
		if(m_spectrumCounter > 1)
		{
			m_currentFreq = m_responseFreq;
			m_panoramaFreqControl->setChannelReady(m_currentFreq);
			m_spectrumCounter = 0;
		}
		m_spectrumCounter++;
	}
	return;
}

void SpectrumWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
	//Receiving rds raw protobuf
	if( method == RPC_METHOD_CONFIG_RDS_ANSWER ) {

		RdsProtobuf::ServerMessage sMsg;
		if( !parseServerMessage(data.toByteArray(), sMsg) ) {
			return;
		}

		if( isServerLocationShot(sMsg) ) {
			setupPoints( getServerLocationShot( sMsg ) );
		}

	}
}

void SpectrumWidgetDataSource::slotChangeFreq()
{

}

void SpectrumWidgetDataSource::onSleepModeSlot()
{
	m_sleepModeProcess = true;
}

void SpectrumWidgetDataSource::slotRepeatSetFrequency()
{
	if(m_dbmanager != NULL) {
//		m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);
	}
}

void SpectrumWidgetDataSource::sendCommand(int)
{
}
