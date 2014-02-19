#include "GraphicData.h"

#include <QDebug>

#include "Rpc/RpcDefines.h"

#define BANDWIDTH_SINGLE	20000000
#define TO_KHZ				1000
#define TO_HZ				1000000

GraphicData::GraphicData(IGraphicWidget *gr_widget, ICommonComponents* common_correlations, ITabManager* tab_manager, int id, QObject* parent):
	QObject(parent)
{
	_common_correlations = common_correlations;
	_id = id;
	_tab_manager = tab_manager;

	connect(this, SIGNAL(signalSetCorData(quint32,QByteArray,bool)), this, SLOT(m_slotSetCorData(quint32,QByteArray,bool)));

	/// TODO need to correct architecture
	if(common_correlations == NULL)
	{
		return;
	}

	for(int i = 0; i < common_correlations->count(0); i++)
	{
		float *sp = new float[1];
		float *peak = new float[1];
		double b = 0;
		_map_peaks_correlation.insert(i, peak);
		_map_spectrum_corelation.insert(i, sp);
		_map_bandwidth_corelation.insert(i, b);
	}
}

GraphicData::~GraphicData()
{
	emit signalFinished();
}

void GraphicData::set_data(quint32 point2, const QByteArray& points, bool isComplex)
{
	emit signalSetCorData(point2, points, isComplex);
}

void GraphicData::m_slotSetCorData(quint32 point2, QByteArray vecFFTBA, bool isComplex)
{
	QVector<QPointF> vecFFT;
	QDataStream stream(vecFFTBA);
	stream >> vecFFT;

	int cor_id = point2;
	if(point2 > _id)
	{
		cor_id -= 1;
	}

	IGraphicWidget* gr_correlation = _common_correlations->get(cor_id);//_map_correlation_widget->value(point2);

	if(!gr_correlation->isGraphicVisible() || gr_correlation == NULL) {
		return;
	}

	QString base = _tab_manager->getStationName(_id);
	QString second = _tab_manager->getStationName(point2);

	float* sp_correlation = _map_spectrum_corelation.value(cor_id);
	float* peaks_correlation = _map_peaks_correlation.value(cor_id);
	double b_cor = _map_bandwidth_corelation.value(cor_id);
	int PointCount = vecFFT.size();
	//    float* spectrum = new float[PointCount];

	qreal startx = vecFFT.at(0).x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = endx - startx;


	if(b_cor != bandwidth)
	{
		b_cor = bandwidth;
		_map_bandwidth_corelation.insert(cor_id, b_cor);
		delete[] sp_correlation;
		sp_correlation = new float[PointCount];

		delete[] peaks_correlation;
		peaks_correlation = new float[PointCount];
		_needSetup = true;
	}


	for(int i = 0; i < vecFFT.size(); i++)
	{
		sp_correlation[i] = vecFFT.at(i).y();

		if((_startx_cor != startx) || (sp_correlation[i] > peaks_correlation[i]))
		{
			peaks_correlation[i] = sp_correlation[i];
		}
	}
	//return;

	_map_spectrum_corelation.insert(cor_id, sp_correlation);
	_map_peaks_correlation.insert(cor_id, peaks_correlation);


	if(_startx_cor != startx)
	{
		_startx_cor = startx;
	}

	if(_needSetup)
	{
		//        emit signalDataS(_spectrum, _spectrum_peak_hold);
		gr_correlation->setSignalSetup(sp_correlation, peaks_correlation, PointCount, b_cor, /*isComplex*/true);
		_needSetup = false;
	}
	else {
		//        emit signalData(_spectrum, _spectrum_peak_hold);
		gr_correlation->setSignal(sp_correlation, peaks_correlation);
	}

	gr_correlation->setLabelName(base, second);
}

void GraphicData::onMethodCalled(const QString &method, const QVariant &arg)
{
	if (RPC_SLOT_SERVER_SEND_CORRELATION == method){
		//QList<QVariant> list = arg.toList();
		//set_data(list.at(1).toUInt(), list.at(0).toByteArray(), true);


		/*QList<QVariant> list;
		QVariant pointsVariant(points);
		QVariant point2Variant(point2);

		list.append(pointsVariant);
		list.append(point2Variant);

		QVariant data(list);*/

		//correlation
		//TODO: point2 from rpc
		//int point2 = 0;
		//set_data(point2, arg.toByteArray(), true);
	}
}
