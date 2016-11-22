#include "AnalysisWidgetDataSource.h"

#include "Rpc/RpcDefines.h"

#include "Interfaces/IAnalysisWidget.h"

#include "Logger/Logger.h"

#define TO_KHZ 1000

AnalysisWidgetDataSource::AnalysisWidgetDataSource(IGraphicWidget* correlationWidget, ITabManager* tabManager, int id, QObject *parent)
	: BaseDataSource(parent)
	, m_needSetup(false)
	, m_type(id)
{
	m_correlationWidget = correlationWidget;

	m_tabManager = tabManager;

	m_mapPeaksCorrelation = new float[1];
	m_mapSpectrumCorelation = new float[1];
	m_mapBandwidthCorelation = 0;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));

}

AnalysisWidgetDataSource::~AnalysisWidgetDataSource()
{
	//log_debug("<<<<<<<<<<<<<<");
}

void AnalysisWidgetDataSource::onMethodCalled(const QString& method, const QVariant& data)
{
	emit onMethodCalledSignal(method, data);
}

Q_DECLARE_METATYPE(float*)
void AnalysisWidgetDataSource::setCorData(int type, const QVector<QPointF>& points)
{
	float* spCorrelation = m_mapSpectrumCorelation;
	float* peaksCorrelation = m_mapPeaksCorrelation;
	double bCor = m_mapBandwidthCorelation;
	int pointCount = points.size();

	qreal startx = points.at(0).x();
	qreal endx = points.at(points.size() - 1).x();
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


	for(int i = 0; i < points.size(); i++) {
		spCorrelation[i] = points.at(i).y();

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

	list.append(type);

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

	QVariant data(list);
	onDataReceived(RPC_SLOT_SERVER_SEND_ANALYSIS, data);
}

void AnalysisWidgetDataSource::onMethodCalledSlot(QString method, QVariant data)
{
//	if( method == RPC_METHOD_CONFIG_RDS_ANSWER ) {

//		RdsProtobuf::Packet pkt;
//		QByteArray bData = data.toByteArray();
//		pkt.ParseFromArray( bData.data(), bData.size() );

//		if( !isAnalysisAbsPhaseFreq(pkt) ) {
//			return;
//		}

//		RdsProtobuf::AnalysisAbsPhaseFreq msg = pkt.from_server().data().analysis_abs_phase_freq();

//		double startFreq = 0;
//		double stepFreq = 0;
//		double freq = 0;
//		QVector<QPointF> vec;

//		//abs plot, 0 type
//		if(m_type == 0) {
//			startFreq = msg.plot_abs().axis_x_start();
//			stepFreq = msg.plot_abs().axis_x_step();

//			freq = startFreq;

//			for(int i = 0; i < msg.plot_abs().data().size(); i++) {
//				double val = msg.plot_abs().data(i);
//				vec.append( QPointF(freq*TO_KHZ, val) );
//				freq += stepFreq;
//			}

//			setCorData(0, vec);
//		}

//		//phase plot
//		if( m_type == 1 ) {
//			startFreq = msg.plot_phase().axis_x_start();
//			stepFreq = msg.plot_phase().axis_x_step();

//			vec.clear();
//			freq = startFreq;

//			for(int i = 0; i < msg.plot_phase().data().size(); i++) {
//				double val = msg.plot_phase().data(i);
//				vec.append( QPointF(freq*TO_KHZ, val) );
//				freq += stepFreq;
//			}

//			setCorData(1, vec);
//		}

//		//freq plot
//		if( m_type == 2 ) {
//			startFreq = msg.plot_freq().axis_x_start();
//			stepFreq = msg.plot_freq().axis_x_step();

//			vec.clear();
//			freq = startFreq;

//			for(int i = 0; i < msg.plot_freq().data().size(); i++) {
//				double val = msg.plot_freq().data(i);
//				vec.append( QPointF(freq*TO_KHZ, val) );
//				freq += stepFreq;
//			}

//			setCorData(2, vec);
//		}
//	}
}

void AnalysisWidgetDataSource::sendCommand(int)
{
}

