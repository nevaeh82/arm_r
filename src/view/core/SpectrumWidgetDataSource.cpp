#include "SpectrumWidgetDataSource.h"
#include "Rpc/RpcDefines.h"

#include <stddef.h>

#define TO_KHZ 1000
#define TO_HZ				1000000
#define BANDWIDTH_SINGLE	20000000

SpectrumWidgetDataSource::SpectrumWidgetDataSource(IGraphicWidget *widget, QObject *parent)
	: BaseDataSource(parent)
	, m_pointCountWhole(0)
	, m_needSetup(0)
	, m_currentFreq(0)
	, m_endFreq(0)
	, m_responseFreq(0)
	//	, m_rpcPrmClient(NULL)
	, m_startx(0)
	, m_tabManager(NULL)
	, m_spectrumCounter(0)
	, m_dbmanager(NULL)
	, m_workFreq(0)
	, m_panoramaFreqControl(NULL)
	, m_id(-1)
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
	connect(&m_timerChangeFreqPanorama, SIGNAL(timeout()), this, SLOT(slotChangeFreq()));
	connect(&m_timerRepeatSetFreq, SIGNAL(timeout()), this, SLOT(slotRepeatSetFrequency()));
}

SpectrumWidgetDataSource::~SpectrumWidgetDataSource()
{
	delete[] m_spectrum;
	delete[] m_spectrumPeakHold;
}

Q_DECLARE_METATYPE(float*)
void SpectrumWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
//	QByteArray aData = data.toByteArray();
//	RdsProtobuf::Packet pkt;
//	pkt.ParseFromArray(aData.data(), aData.size());

//	if( isAnalysisSpectrogram(pkt) ) {
//		log_debug(QString("Datasource sonogram:::: %1 id-%2").arg(m_sonogramTime.msecsTo(QTime::currentTime())).arg((int)this->thread()->currentThreadId()));
//		m_sonogramTime = QTime::currentTime();
//	}
	emit onMethodCalledSignal(method, data);
}

void SpectrumWidgetDataSource::dataProccess(QVector<QPointF>& vecFFT, bool)
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
		setBandwidth(bandwidth); // Old Flakon
		//setBandwidth1(vecFFT.size()); //new Protobuf
		m_bandwidthSingleSample = bandwidth;
		m_needSetup = true;
		//		if(m_spectrumWidget)
		//		{
		//			qDebug() << "ZERO FREQ = " << vecFFT_0.x();
		//			m_spectrumWidget->setZeroFrequency((vecFFT_0.x())*TO_KHZ);
		//		}
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

	//log_debug( QString("Data process : %1  : %2 index: %3").arg(index*vecFFT.size()).arg(m_pointCountWhole).arg(index) );

	int t1 = index*vecFFT.size();
	int t2 = m_pointCountWhole;

	if( index < 0 ) {
		return;
	}

	for(int i = 0; i < vecFFT.size(); i++)
	{
		m_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();
		//		if((m_startx != m_responseFreq) || (m_spectrum[i] > m_spectrumPeakHold[i]) || (m_spectrumPeakHold[i] == 0))
		//		{
		//			m_spectrumPeakHold[i] = m_spectrum[i];
		//		}

		if((m_startx != m_responseFreq) || (m_spectrum[index*vecFFT.size()+i] >
											m_spectrumPeakHold[index*vecFFT.size()+i]) ||
				(m_spectrumPeakHold[index*vecFFT.size()+i] == 0))
		{
			m_spectrumPeakHold[index*vecFFT.size()+i] =
					m_spectrum[index*vecFFT.size()+i];
		}
	}
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
			m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);
		}

		m_timerRepeatSetFreq.start(7000);

		return true;
	}

	m_timerRepeatSetFreq.stop();
	return false;
	/// start timer for loop panorama frequency
}

void SpectrumWidgetDataSource::setBandwidth(double bandwidth)
{
	log_debug( QString("BANDWIDTH %1 :::::::").arg(bandwidth) );

	if(m_bandwidth != bandwidth)
	{
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
		log_debug("panorama stopped");
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

//void SpectrumWidgetDataSource::setPrmRpcClient(RpcPrmClient *rpcClient)
//{
//	m_rpcPrmClient = rpcClient;
//}

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
		return;
	}

	//		if(m_isPanoramaStart && m_spectrumCounter == 0)
	//		{
	//			return;
	//		}


	bool isComplex = true;

//	QByteArray vecFFTBA = data.toByteArray();

//
//	QDataStream stream(vecFFTBA);

//	unsigned int id; // m_header.id;

//	unsigned int zone;
//	stream >> zone;
//	unsigned int typeRds;
//	stream >> typeRds;

//	stream >> id;

	int id = -1;
	float cf = 0;
	QVector<QPointF> vecFFT;

	for(int i=0; i<data.spectrum_plot_size(); i++) {
		if( data.spectrum_plot(i).index() == m_spectrumWidget->getId() ) {
			id = m_spectrumWidget->getId();
		}
	}

	if(id < 0) {
		return;
	}

	cf = data.central_frequency();

	double freq = data.spectrum_plot(id).axis_x_start();
	double stepFreq = data.spectrum_plot(id).axis_x_step();

	for(int i = 0; i < data.spectrum_plot(id).data().size(); i++) {
		double val = data.spectrum_plot(id).data(i);
		vecFFT.append( QPointF(freq*TO_KHZ, val) );
		freq += stepFreq;
	}

	if(!m_isPanoramaStart)
	{
		m_spectrumWidget->setZeroFrequency(cf);
		if(m_responseFreq != cf) {
			clearPeak();
		}
	}

	m_responseFreq = cf;

	if (!vecFFT.size()) return;

	dataProccess(vecFFT, isComplex);

	QList<QVariant> list;

	QVariant spectrumVariant = QVariant::fromValue(m_spectrum);
	QVariant peaksSpectrumVariant = QVariant::fromValue(m_spectrumPeakHold);

	list.append(spectrumVariant);
	list.append(peaksSpectrumVariant);

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

	QVariant dataOut(list);
	onDataReceived(RPC_SLOT_SERVER_SEND_POINTS, dataOut);


	// if(m_workFreq != cf) {
	if(m_isPanoramaStart)
	{
		m_responseFreq = cf;
		m_spectrumCounter++;
		m_timerRepeatSetFreq.stop();
		m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, cf);
	}
	//  }
	m_workFreq = cf;


	if(m_isPanoramaStart)
	{
		//			if(m_responseFreq != m_currentFreq)
		//			{
		//				m_spectrumCounter = 0;
		//				return;
		//			}
		if(m_spectrumCounter > 3 /*&& m_responseFreq != m_currentFreq*/)
		{
			m_currentFreq = m_responseFreq;
			m_panoramaFreqControl->setChannelReady(m_id);
			m_spectrumCounter = 0;
			//slotChangeFreq();
		}
		m_spectrumCounter++;
	}
	return;
}

void SpectrumWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
//    log_debug("SpectrumWidgetDataSource");
	if (RPC_SLOT_SERVER_SEND_POINTS == method) {

	}

	if(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION == method) {
		onDataReceived(method, data);
		return;
	}

	if(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH == method) {
		onDataReceived(method, data);
		return;
	}

	if(RPC_SLOT_SERVER_PRM_STATUS == method)
	{
		QList<QVariant> list = data.toList();
		quint16 freq = list.at(0).toUInt();
		m_responseFreq = freq;
		////		quint32 filter = list.at(1).toUInt();
		////		quint32 att1 = list.at(2).toUInt();
		////		quint32 att2 = list.at(3).toUInt();

		//        if(m_isPanoramaStart)
		//        {
		//            m_timerRepeatSetFreq.stop();
		//        }

	}
	if(method == RPC_PRM_FREQUENCY_CHANGED)
	{
		m_responseFreq = m_currentFreq;
		if(m_isPanoramaStart)
		{
			m_spectrumCounter++;
			m_timerRepeatSetFreq.stop();
		}
	}

	//Receiving rds raw protobuf
	if( method == RPC_METHOD_CONFIG_RDS_ANSWER ) {

		RdsProtobuf::Packet pkt;
		QByteArray bData = data.toByteArray();
		pkt.ParseFromArray( bData.data(), bData.size() );\

		if(!pkt.has_from_server()) {
			return;
		}

		RdsProtobuf::ServerMessage sMsg = pkt.from_server();


		if( isServerLocationShot(sMsg) ) {
			setupPoints( getServerLocationShot( sMsg ) );
		}

		//onDataReceived(RPC_METHOD_CONFIG_RDS_ANSWER, data);
	}
}

void SpectrumWidgetDataSource::slotChangeFreq()
{
//	//	if(m_rpcPrmClient == NULL)
//	//	{
//	//		return;
//	//	}
//	if(m_currentFreq == m_responseFreq)
//	{
//		if(m_currentFreq >= m_endFreq)
//		{
//			m_currentFreq = m_startFreq;
//		}
//		else
//		{
//			m_currentFreq += freqStep;
//		}
//	}

//	log_info(QString("Current frequency is %1").arg(m_currentFreq));
//	if(m_dbmanager != NULL) {
//		m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);
//	}

//	m_timerRepeatSetFreq.start(7000);

}

void SpectrumWidgetDataSource::slotRepeatSetFrequency()
{
	if(m_dbmanager != NULL)
		m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);
}

void SpectrumWidgetDataSource::sendCommand(int)
{
}
