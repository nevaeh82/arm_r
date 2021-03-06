#include "SpectrumWidgetDataSource.h"
#include "Rpc/RpcDefines.h"

#define TO_KHZ 1000
#define TO_HZ				1000000
#define BANDWIDTH_SINGLE	20000000

SpectrumWidgetDataSource::SpectrumWidgetDataSource(IGraphicWidget* spectrumWidget, QObject *parent)
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
{
	m_spectrumWidget = spectrumWidget;

	m_bandwidth = 0;
	m_pointCount = 0;
	m_bandwidthSingleSample = 0;
	m_isPanoramaStart = false;
	m_needSetupSpectrum = true;

	m_spectrum = new float[1];
	m_spectrumPeakHold = new float[1];

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));
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
		setBandwidth(bandwidth);
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
    }
    else
    {
        index = 0;
    }


	for(int i = 0; i < vecFFT.size(); i++)
	{
		m_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();
		if((m_startx != m_responseFreq) || (m_spectrum[i] > m_spectrumPeakHold[i]) || (m_spectrumPeakHold[i] == 0))
		{
			m_spectrumPeakHold[i] = m_spectrum[i];
		}
	}
}

int SpectrumWidgetDataSource::findIndex(qreal startx)
{
	int list_count = m_listStartx.size();
	if(m_listStartx.isEmpty())
	{
		m_listStartx.push_back(startx);
		return 0;
	}
	else
	{
		if(startx < m_listStartx.front() - 1)
		{
			m_listStartx.push_front(startx);
			return 0;
		}
		if(startx > m_listStartx.back() + 1)
		{
			m_listStartx.push_back(startx);
			return m_listStartx.size() - 1;
		}
	}

	int index = -1;

	if(m_listStartx.size() != list_count){
		return index;
	}

	foreach (qreal s, m_listStartx){
		index++;
		if(s + 1 > startx)
		{
			m_listStartx.replace(index, startx);
			break;
		}
	}

	return index;
}

bool SpectrumWidgetDataSource::startPanorama(bool start)
{
	if(start)
	{
        if(m_spectrumWidget != NULL)
            m_spectrumWidget->setZeroFrequency(m_startFreq);

        slotChangeFreq();
		return true;
	}

    m_timerRepeatSetFreq.stop();
	return false;
	/// start timer for loop panorama frequency
}

void SpectrumWidgetDataSource::setBandwidth(double bandwidth)
{
	if(m_bandwidth != bandwidth)
	{
		m_bandwidth = bandwidth;
		int div = m_bandwidth / BANDWIDTH_SINGLE;
		m_pointCountWhole = m_pointCount*div;

		delete[] m_spectrum;
		m_spectrum = new float[m_pointCountWhole] ();

		delete[] m_spectrumPeakHold;
		m_spectrumPeakHold = new float[m_pointCountWhole] ();
		m_needSetupSpectrum = true;
	}
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

void SpectrumWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
	if (RPC_SLOT_SERVER_SEND_POINTS == method) {
		if(!m_spectrumWidget->isGraphicVisible() && !m_needSetupSpectrum) {
			return;
		}
        if(m_isPanoramaStart && m_spectrumCounter == 0)
        {
            return;
        }


		bool isComplex = true;

		QByteArray vecFFTBA = data.toByteArray();

		QVector<QPointF> vecFFT;
		QDataStream stream(vecFFTBA);

		unsigned int id; // m_header.id;
		stream >> id;
		if (id != m_spectrumWidget->getId()) {
			return;
		}

		stream >> vecFFT;
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

		QVariant data(list);
		onDataReceived(RPC_SLOT_SERVER_SEND_POINTS, data);

        if(m_isPanoramaStart)
        {
            if(m_responseFreq != m_currentFreq)
            {
                m_spectrumCounter = 0;
                return;
            }
            if(m_spectrumCounter > 3)
            {
                m_spectrumCounter = 0;
                slotChangeFreq();
            }
            m_spectrumCounter++;
        }
		return;
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
}

void SpectrumWidgetDataSource::slotChangeFreq()
{
//	if(m_rpcPrmClient == NULL)
//	{
//		return;
//	}
	if(m_currentFreq == m_responseFreq)
	{
        if(m_currentFreq >= m_endFreq)
		{
            m_currentFreq = m_startFreq;
		}
        else
        {
            m_currentFreq += freqStep;
        }
	}

    log_info(QString("Current frequency is %1").arg(m_currentFreq));
    if(m_dbmanager != NULL)
        m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);

    m_timerRepeatSetFreq.start(7000);

}

void SpectrumWidgetDataSource::slotRepeatSetFrequency()
{
    if(m_dbmanager != NULL)
        m_dbmanager->updatePropertyValue(m_name, DB_FREQUENCY_PROPERTY, m_currentFreq);
}

void SpectrumWidgetDataSource::sendCommand(int)
{
}
