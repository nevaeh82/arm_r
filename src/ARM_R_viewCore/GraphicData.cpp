#include "GraphicData.h"

#include <QDebug>

GraphicData::GraphicData(IGraphicWidget *gr_widget, ICommonComponents* common_correlations, ITabManager* tab_manager, int id)
{
	_PointCount = 0;
	_pointCountWhole = 0;
    _common_correlations = common_correlations;
    _id = id;
    _tab_manager = tab_manager;
    _gr_widget = gr_widget;
//    _map_correlation_widget = map_correlation_widget;
    _spectrum = new float[1];
    _spectrum_peak_hold = new float[1];
    _bandwidth = 0;
    _needSetup = true;
    connect(this, SIGNAL(signalSetData(QVector<QPointF>,bool)), this, SLOT(_slotSetData(QVector<QPointF>,bool)));
    connect(this, SIGNAL(signalSetDefModulation(QString)), this, SLOT(_slotSetDefModulation(QString)));

    connect(this, SIGNAL(signalSetCorData(quint32,QVector<QPointF>,bool)), this, SLOT(_slotSetCorData(quint32,QVector<QPointF>,bool)));

	connect(this, SIGNAL(signalSetBandwidth(double)), this, SLOT(_slotSetBandwidth(double)));
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
			return _list_startx.size();
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

void GraphicData::_slotSetData(QVector<QPointF> vecFFT, bool isComplex)
{
    if(!_gr_widget->isGraphicVisible())
        return;
	if(_bandwidth == 0)
		return;

	if(_pointCountWhole == 0)
		return;

	_PointCount = vecFFT.size();
//    float* spectrum = new float[PointCount];

    qreal startx = vecFFT.at(0).x();
    qreal endx = vecFFT.at(vecFFT.size() - 1).x();
	//double bandwidth = (endx - startx)*1000;

//    if(_bandwidth != bandwidth)
//    {
//        _bandwidth = bandwidth;
//        delete[] _spectrum;
//        _spectrum = new float[PointCount];

//        delete[] _spectrum_peak_hold;
//        _spectrum_peak_hold = new float[PointCount];
//        _needSetup = true;
//    }


//	QMap<qreal, QList<qreal> >::iterator it = _map_spectrum.lowerBound(startx);

	int index = _find_index(startx);


//	QList<qreal> list_alt;
	for(int i = 0; i < vecFFT.size(); i++)
	{
		_spectrum[index*vecFFT.size() + i] = vecFFT.at(i).y();
//		list_alt.append(vecFFT.at(i).y());

		if((_startx != startx) || (_spectrum[i] > _spectrum_peak_hold[i]))
		{
			_spectrum_peak_hold[i] = _spectrum[i];
		}
	}

//	_map_spectrum.remove(it.key());
//	_map_spectrum.insert(startx, list_alt);



    if(_startx != startx)
    {
        _startx = startx;
    }

    if(_needSetup)
    {
//        emit signalDataS(_spectrum, _spectrum_peak_hold);
		_gr_widget->setSignalSetup(_spectrum, _spectrum_peak_hold, vecFFT.size(), _bandwidth, isComplex);
        _needSetup = false;
    }
    else
    {
//        emit signalData(_spectrum, _spectrum_peak_hold);

        _gr_widget->setSignal(_spectrum, _spectrum_peak_hold);
    }
}

void GraphicData::_slotSetCorData(quint32 point2, QVector<QPointF> vecFFT, bool isComplex)
{
    int cor_id = point2;
    if(point2 > _id)
    {
        cor_id -= 1;
    }

    IGraphicWidget* gr_correlation = _common_correlations->get(cor_id);//_map_correlation_widget->value(point2);
    if(!gr_correlation->isGraphicVisible() || gr_correlation == NULL)
        return;

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

void GraphicData::_slotSetDefModulation(QString modulation)
{
	_gr_widget->setDefModulation(modulation);
}

void GraphicData::_slotSetBandwidth(double bandwidth)
{
	if(_bandwidth != bandwidth)
	{
		_bandwidth = bandwidth;
		double div = _bandwidth/20000;
		double _pointCountWhole = _PointCount*div;
		delete[] _spectrum;
		_spectrum = new float[_pointCountWhole] ();

		delete[] _spectrum_peak_hold;
		_spectrum_peak_hold = new float[_pointCountWhole] ();
		_needSetup = true;
	}
}
