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

void SpectrumWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	bool isComplex = false;

	if (RPC_SLOT_SERVER_SEND_POINTS == method) {
		if(!m_spectrumWidget->isGraphicVisible())
			return;

		isComplex = true;

		data.data();

		QByteArray vecFFTBA = data.toByteArray();

		QVector<QPointF> vecFFT;
		QDataStream stream(vecFFTBA);
		stream >> vecFFT;

		dataProccess(vecFFT, isComplex);

		if(m_needSetupSpectrum)
		{
			m_spectrumWidget->setSignalSetup(m_spectrum, m_spectrumPeakHold, m_pointCountWhole/*vecFFT.size()*/, m_bandwidth, isComplex);
			m_needSetupSpectrum = false;
		}
		else
		{
			//m_spectrumWidget->setSignal(m_spectrum, m_spectrumPeakHold);
		}

		QList< QList<QPointF> > proccessedDataList;

		foreach (QList<QPointF> list, m_pointsList){
			proccessedDataList.append(list);
		}

		proccessedDataList.append(m_spectrumPeakHoldList);

		//proccessedDataList.append();

		QVariant< QList< QList< QPointF > > > proccessedData(proccessedDataList);

		//QList< QList<QPointF> > proccessedDataList;
		QList< QVector<QPointF> > proccessedDataList2;
		onDataReceived(method, proccessedData, proccessedDataList2);
		return;
		//set_data(arg.toByteArray(), true); //spectrum
	}




	/*else if(RPC_SLOT_SERVER_SEND_DETECTED_BANDWIDTH == method) {
		//setDetectedAreas(arg.toByteArray());
	} else if(RPC_SLOT_SERVER_SEND_RESPONSE_MODULATION == method) {
		//set_def_modulation(arg.toString()); //spectrum
	} else if (RPC_SLOT_SERVER_SEND_CORRELATION == method){
		//correlation
		//TODO: point2 from rpc
		int point2 = 0;
		//set_data(point2, arg.toByteArray(), true);
		onDataReceived(method, data);
	}*/
}

void SpectrumWidgetDataSource::dataProccess(QVector<QPointF>& vecFFT, bool isComplex)
{
	QList<QPointF> vecFFTList = vecFFT.toList();
	m_pointCount = vecFFTList.size();

	qreal startx = vecFFTList.at(0).x();
	qreal endx = vecFFTList.at(vecFFTList.size() - 1).x();
	double bandwidth = (endx - startx)*TO_KHZ;

	if(m_bandwidthSingleSample != bandwidth && m_isPanoramaStart == false)
	{
		setBandwidth(bandwidth);
		m_bandwidthSingleSample = bandwidth;
		m_needSetup = true;
		if(m_spectrumWidget)
		{
			qDebug() << "ZERO FREQ = " << vecFFTList.at(0).x();
			m_spectrumWidget->setZeroFrequency((vecFFTList.at(0).x())*TO_KHZ);
		}
	}

	int index = findIndex(startx);
	int vecCount = m_pointsList.count();

	//qDebug() << "index = " << index;
	//qDebug() << "vecCount1 = " << vecCount;

	if (vecCount == 0){
		m_spectrumPeakHoldList.reserve(m_pointCount);
	}

	if (vecCount < index + 1){
		m_pointsList.append(vecFFTList);
	} else {
		m_pointsList.replace(index, vecFFTList);
	}
	//qDebug() << "vecCount2 = " << pointsVectorsList.count();

	for(int i = 0; i < vecFFT.size(); i++)
	{
		//m_spectrum[index*vecFFT.size() + i] = pointsVectorsList.at(index).at(i).y();

		m_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();
		/*if((m_startx != startx) || (m_spectrum[i] > m_spectrumPeakHold[i]) || (m_spectrumPeakHold[i] == 0))
		{
			m_spectrumPeakHold[i] = m_spectrum[i];
		}*/
		if((m_startx != startx) || (m_pointsList.at(0).at(i).y() > m_spectrumPeakHold[i]) || (m_spectrumPeakHold[i] == 0))
		{
			m_spectrumPeakHoldList.replace(i, QPointF(m_pointsList.at(0).at(i).x(), m_pointsList.at(0).at(i).y()));
			//m_spectrumPeakHoldVector.at(i).setY(m_pointsVectorsList.at(0).at(i).y());
			//m_spectrumPeakHold[i] = m_pointsVectorsList.at(0).at(i).y();
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
	if(bandwidth < 20){
		bandwidth = 20;
	}

	bandwidth *= TO_HZ;
	setBandwidth(bandwidth);
}

void SpectrumWidgetDataSource::sendCommand(int)
{
}
