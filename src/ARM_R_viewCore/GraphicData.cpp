#include "GraphicData.h"

#include <QDebug>

#define BANDWIDTH_SINGLE	20000000
#define TO_KHZ				1000
#define TO_HZ				1000000

GraphicData::GraphicData(IGraphicWidget *gr_widget, ICommonComponents* common_correlations, ITabManager* tab_manager, int id)
{
	_PointCount = 0;
	m_pointCountWhole = 0;
    _common_correlations = common_correlations;
    _id = id;
    _tab_manager = tab_manager;
    _gr_widget = gr_widget;
//    _map_correlation_widget = map_correlation_widget;
    _spectrum = new float[1];
    _spectrum_peak_hold = new float[1];
    _bandwidth = 0;
	m_bandwidthSingleSample = 0;
	m_isPanaramaStart = false;
    _needSetup = true;
	m_needSetupSpectrum = true;
	connect(this, SIGNAL(signalSetData(QVector<QPointF>,bool)), this, SLOT(m_slotSetData(QVector<QPointF>,bool)));
	connect(this, SIGNAL(signalSetDefModulation(QString)), this, SLOT(m_slotSetDefModulation(QString)));

	connect(this, SIGNAL(signalSetCorData(quint32,QVector<QPointF>,bool)), this, SLOT(m_slotSetCorData(quint32,QVector<QPointF>,bool)));

	connect(this, SIGNAL(signalSetBandwidth(double)), this, SLOT(m_slotSetBandwidth(double)));

	connect(this, SIGNAL(signalPanoramaStart(double,double)), this, SLOT(m_slotPanoramaStart(double,double)));
	connect(this, SIGNAL(signalPanoramaStop()), this, SLOT(m_slotPanoramaStop()));
	//    QMap<int, IGraphicWidget *>::iterator it;
//    for(it = _map_correlation_widget->begin(); it != _map_correlation_widget->end(); ++it)
//    {
//        float *sp = new float[1];
//        float *peak = new float[1];
//        double b = 0;
//        _map_peaks_correlation.insert(it.key(), peak);
//        _map_spectrum_corelation.insert(it.key(), sp);
//        _map_bandwidth_corelation.insert(it.key(), b);
//    }

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

void GraphicData::set_data(QVector<QPointF> vecFFT, bool isComplex)
{
    emit signalSetData(vecFFT, isComplex);
}

void GraphicData::set_data(quint32 point2, QVector<QPointF> points, bool isComplex)
{
    emit signalSetCorData(point2, points, isComplex);
}

void GraphicData::set_def_modulation(QString modulation)
{
	emit signalSetDefModulation(modulation);
}

void GraphicData::set_bandwidth(double bandwidth)
{
	emit signalSetBandwidth(bandwidth);
}

void GraphicData::set_panorama(double start, double end)
{
	emit signalPanoramaStart(start, end);
}

void GraphicData::set_panorama_stop()
{
	emit signalPanoramaStop();
}

int GraphicData::_find_index(qreal startx)
{
	int list_count = _list_startx.size();
	int index = -1;
	if(_list_startx.isEmpty())
	{
		_list_startx.push_back(startx);
		return 0;
	}
	else
	{
		if(startx < _list_startx.front() - 1)
		{
			_list_startx.push_front(startx);
			return 0;
		}
		if(startx > _list_startx.back() + 1)
		{
			_list_startx.push_back(startx);
			return _list_startx.size() - 1;
		}
	}

//	qDebug() << _list_startx.size();
	if(_list_startx.size() == list_count)
	{
		QList<qreal>::iterator it;
		for(it = _list_startx.begin(); it != _list_startx.end(); ++it)
		{
			/// startx is in E interval
			if((*it+1 > startx) && (*it-1 < startx))
			{
				index = _list_startx.indexOf(*it);
				_list_startx.replace(index, startx);
				break;
			}
		}
	}

	return index;
}

void GraphicData::m_dataProccess(QVector<QPointF> vecFFT, bool isComplex)
{
	_PointCount = vecFFT.size();

	qreal startx = vecFFT.at(0).x();
	qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	double bandwidth = (endx - startx)*TO_KHZ;

	if(m_bandwidthSingleSample != bandwidth && m_isPanaramaStart == false)
	{
		m_slotSetBandwidth(bandwidth);
		m_bandwidthSingleSample = bandwidth;
		_needSetup = true;
	}

	int index = _find_index(startx);


//	qDebug() << vecFFT.size();

	for(int i = 0; i < vecFFT.size(); i++)
	{
		_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();

//		qDebug() << index << i << _spectrum[index*vecFFT.size() + i];

		if((_startx != startx) || (_spectrum[i] > _spectrum_peak_hold[i]) || (_spectrum_peak_hold[i] == 0))
		{
			_spectrum_peak_hold[i] = _spectrum[i];
		}
	}

	if(_startx != startx)
	{
		_startx = startx;
	}


}

void GraphicData::m_slotSetData(QVector<QPointF> vecFFT, bool isComplex)
{
    if(!_gr_widget->isGraphicVisible())
        return;

	m_dataProccess(vecFFT, isComplex);

	if(m_needSetupSpectrum)
	{
		_gr_widget->setSignalSetup(_spectrum, _spectrum_peak_hold, m_pointCountWhole/*vecFFT.size()*/, _bandwidth, isComplex);
		m_needSetupSpectrum = false;
	}
	else
	{
		_gr_widget->setSignal(_spectrum, _spectrum_peak_hold);
	}
}

void GraphicData::m_slotSetCorData(quint32 point2, QVector<QPointF> vecFFT, bool isComplex)
{
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
    else
    {
//        emit signalData(_spectrum, _spectrum_peak_hold);

        gr_correlation->setSignal(sp_correlation, peaks_correlation);
    }

    gr_correlation->setLabelName(base, second);

}

void GraphicData::m_slotSetDefModulation(QString modulation)
{
	_gr_widget->setDefModulation(modulation);
}

void GraphicData::m_slotSetBandwidth(double bandwidth)
{
	if(_bandwidth != bandwidth)
	{
		_bandwidth = bandwidth;
		int div = _bandwidth/BANDWIDTH_SINGLE;
		qDebug() << div << _PointCount;
		m_pointCountWhole = _PointCount*div;
		qDebug() << "m_pointCountWhole" << m_pointCountWhole;

		delete[] _spectrum;
		_spectrum = new float[m_pointCountWhole] ();

		delete[] _spectrum_peak_hold;
		_spectrum_peak_hold = new float[m_pointCountWhole] ();
		m_needSetupSpectrum = true;
	}
}

void GraphicData::m_slotPanoramaStart(double start, double end)
{
	qDebug() << "panorama started = " << start << end;

	if(start > end)
		return;

	double bandwidth = end - start;
	if(bandwidth < 20)
	{
		bandwidth = 20;
	}

	bandwidth *= TO_HZ;

	m_isPanaramaStart = true;
	set_bandwidth(bandwidth);
}

void GraphicData::m_slotPanoramaStop()
{
	qDebug() << "panorama stopped";
	m_isPanaramaStart = false;

	set_bandwidth(m_bandwidthSingleSample);
}
