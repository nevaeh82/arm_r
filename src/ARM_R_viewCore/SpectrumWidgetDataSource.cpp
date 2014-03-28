#include "SpectrumWidgetDataSource.h"
#include "Rpc/RpcDefines.h"

#define TO_KHZ 1000
#define TO_HZ				1000000
#define BANDWIDTH_SINGLE	20000000

SpectrumWidgetDataSource::SpectrumWidgetDataSource(IGraphicWidget* spectrumWidget, QObject *parent) :
	BaseDataSource(parent)
{
	m_spectrumWidget = spectrumWidget;

	m_bandwidth = 0;

	m_bandwidthSingleSample = 0;
	m_isPanoramaStart = false;
	m_needSetupSpectrum = true;

	m_spectrum = new float[1];
	m_spectrumPeakHold = new float[1];
}

SpectrumWidgetDataSource::~SpectrumWidgetDataSource()
{
	delete[] m_spectrum;
	delete[] m_spectrumPeakHold;
}

Q_DECLARE_METATYPE(float*)
void SpectrumWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	bool isComplex = false;

	if (RPC_SLOT_SERVER_SEND_POINTS == method) {
		if(!m_spectrumWidget->isGraphicVisible() && !m_needSetupSpectrum)
			return;

		isComplex = true;

		QByteArray vecFFTBA = data.toByteArray();

		QVector<QPointF> vecFFT;
		QDataStream stream(vecFFTBA);

		unsigned int id; // m_header.id;
		stream >> id;
		if (id != m_spectrumWidget->getId()) {
			return;
		}

		stream >> vecFFT;

		dataProccess(vecFFT, isComplex);

		QList<QVariant> list;

//		QVariant headerId(id); // m_header.id;

		QVariant spectrumVariant = QVariant::fromValue(m_spectrum);
		QVariant peaksSpectrumVariant = QVariant::fromValue(m_spectrumPeakHold);

//		list.append(headerId); // m_header.id;

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
}

void SpectrumWidgetDataSource::dataProccess(QVector<QPointF>& vecFFT, bool)
{
	m_pointCount = vecFFT.size();

	qreal startx = vecFFT.at(0).x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = (endx - startx)*TO_KHZ;

	if(m_bandwidthSingleSample != bandwidth && m_isPanoramaStart == false)
	{
		setBandwidth(bandwidth);
		m_bandwidthSingleSample = bandwidth;
		m_needSetup = true;
		if(m_spectrumWidget)
		{
			qDebug() << "ZERO FREQ = " << vecFFT.at(0).x();
			m_spectrumWidget->setZeroFrequency((vecFFT.at(0).x())*TO_KHZ);
		}
	}

	int index = findIndex(startx);


	for(int i = 0; i < vecFFT.size(); i++)
	{
		m_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();
		if((m_startx != startx) || (m_spectrum[i] > m_spectrumPeakHold[i]) || (m_spectrumPeakHold[i] == 0))
		{
			m_spectrumPeakHold[i] = m_spectrum[i];
		}
	}

	if(m_startx != startx)
	{
		m_startx = startx;
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

void SpectrumWidgetDataSource::setBandwidth(double bandwidth)
{
	if(m_bandwidth != bandwidth)
	{
		m_bandwidth = bandwidth;
		int div = m_bandwidth / BANDWIDTH_SINGLE;
		//qDebug() << div << m_pointCount;
		m_pointCountWhole = m_pointCount*div;
		//qDebug() << "m_pointCountWhole" << m_pointCountWhole;

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
		qDebug() << "panorama stopped";
		setBandwidth(m_bandwidthSingleSample);
		return;
	}

	qDebug() << "panorama started = " << start << end;

	if (start > end)
		return;

	double bandwidth = end - start;
	if(bandwidth < 20) {
		bandwidth = 20;
	}

	bandwidth *= TO_HZ;
	setBandwidth(bandwidth);
}

bool SpectrumWidgetDataSource::isPanoramaEnabled()
{
	return m_isPanoramaStart;
}

void SpectrumWidgetDataSource::sendCommand(int)
{
}
