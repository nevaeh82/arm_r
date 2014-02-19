#include "CorrelationWidgetDataSource.h"

#include "Rpc/RpcDefines.h"

CorrelationWidgetDataSource::CorrelationWidgetDataSource(IGraphicWidget* correlationWidget, ITabManager* tabManager, int id, QObject *parent) :
	BaseDataSource(parent)
{
	m_correlationWidget = correlationWidget;

	m_id = id;
	m_tabManager = tabManager;

	m_mapPeaksCorrelation = new float[1];
	m_mapSpectrumCorelation = new float[1];
	m_mapBandwidthCorelation = 0;
}

void CorrelationWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	if (RPC_SLOT_SERVER_SEND_CORRELATION == method){

		if(!m_correlationWidget->isGraphicVisible()) {
			return;
		}

		QList<QVariant> list = data.toList();
		quint32 point2 = list.at(1).toUInt();

		if (point2 - 1 != m_id){
			return;
		}

		setCorData(point2, list.at(0).toByteArray(), true);
	}
}

Q_DECLARE_METATYPE(float*)
void CorrelationWidgetDataSource::setCorData(quint32 point2, QByteArray& vecFFTBA, bool isComplex)
{
	QVector<QPointF> vecFFT;
	QDataStream stream(vecFFTBA);
	stream >> vecFFT;

	QString base = m_tabManager->getStationName(m_id);
	QString second = m_tabManager->getStationName(point2);

	float* spCorrelation = m_mapSpectrumCorelation;
	float* peaksCorrelation = m_mapPeaksCorrelation;
	double bCor = m_mapBandwidthCorelation;
	int pointCount = vecFFT.size();

	qreal startx = vecFFT.at(0).x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = endx - startx;

	if(bCor != bandwidth)
	{
		bCor = bandwidth;
		m_mapBandwidthCorelation = bCor;
		delete[] spCorrelation;
		spCorrelation = new float[pointCount];

		delete[] peaksCorrelation;
		peaksCorrelation = new float[pointCount];
		m_needSetup = true;
	}


	for(int i = 0; i < vecFFT.size(); i++)
	{
		spCorrelation[i] = vecFFT.at(i).y();

		if((m_startxCor != startx) || (spCorrelation[i] > peaksCorrelation[i]))
		{
			peaksCorrelation[i] = spCorrelation[i];
		}
	}

	m_mapSpectrumCorelation = spCorrelation;
	m_mapPeaksCorrelation = peaksCorrelation;

	if(m_startxCor != startx)
	{
		m_startxCor = startx;
	}

	QList<QVariant> list;
	QVariant spCorrelationVariant = QVariant::fromValue(spCorrelation);
	QVariant peaksCorrelationVariant = QVariant::fromValue(peaksCorrelation);
	list.append(spCorrelationVariant);
	list.append(peaksCorrelationVariant);

	if(m_needSetup)
	{
		QVariant pointCountVariant(pointCount);
		QVariant bCorVariant(bCor);
		QVariant isComplexVariant(true);

		list.append(pointCountVariant);
		list.append(bCorVariant);
		list.append(isComplexVariant);

		m_needSetup = false;
	}

	QVariant labelBase(base);
	QVariant labelSecond(second);

	list.append(labelBase);
	list.append(labelSecond);

	QVariant data(list);
	onDataReceived(RPC_SLOT_SERVER_SEND_CORRELATION, data);
}

void CorrelationWidgetDataSource::sendCommand(int)
{
}
