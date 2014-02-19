#include "FlakonCoordinateCounter.h"

#include <QDebug>

FlakonCoordinateCounter::FlakonCoordinateCounter(IRouter* router) :
    _corr_threshold(3),
    _count_corr(5),
    _alt(0)
{
    _router = router;
    _subscriber = _router->get_subscriber();
    _id = -1;
    _type = -1;
    _prev_station = 0;
    qRegisterMetaType<DataFromFlacon> ("DataFromFlacon");

    connect(this, SIGNAL(signalData(rpc_flakon_msg)), this, SLOT(_slotGetData(rpc_flakon_msg)));
}

FlakonCoordinateCounter::~FlakonCoordinateCounter()
{
    emit signalFinished();
}

void FlakonCoordinateCounter::set_id(int id)
{
    _id = id;
}

void FlakonCoordinateCounter::send_data(QSharedPointer<IMessageOld> msg_ptr)
{
    _slotGetData(msg_ptr);
//    emit signalData(msg_ptr);

}

int FlakonCoordinateCounter::get_id()
{
    return _id;
}

void FlakonCoordinateCounter::set_type(int type)
{
    _type = type;
}

int FlakonCoordinateCounter::get_type()
{
    return _type;
}


void FlakonCoordinateCounter::_slotGetData(QSharedPointer<IMessageOld> msg_ptr)
{
    int type1 = 1;
    int id = 0;
	IMessageOld *f = (msg_ptr.data());
    QByteArray* dd = f->get(id, type1);
    QDataStream ds(*dd);
    QVector<QPointF> points;
    int point1, point2;
    QTime cor_time;
    double cor_benchmark;
    double cor_max;
    double alt;
    int track_length = 0;
    switch(type1)
    {
    case FLAKON_CORRELATION:
        ds >> point1;
        ds >> point2;
//        ds >> cor_max;
//        ds >> cor_benchmark;
        ds >> points;
        _main_point = point1;
        _map_main_stations.insert(point2, point1);
        _map_vec_corr.insert(point2, points);
//        _map_maxs.insert(point2, cor_max);

////        _map_benchmarks.insert(point2, cor_benchmark);
        if((_prev_station > point2) && (_map_vec_corr.count() > 4))
        {

            QVector< QVector<QPointF> > vec_p;
            QMap<int, QVector<QPointF> >::iterator it;
            for(it = _map_vec_corr.begin(); it != _map_vec_corr.end(); ++it)
            {
                vec_p.append(it.value());
            }
            QVector<double> aDR;
            aDR.clear();
            ZDR mZDR;
            mZDR.getDataFromFlackon(_main_point, vec_p, _corr_threshold, aDR);
            _map_vec_corr.clear();
            if (aDR.size() == 5)
            {
                //Заполнение данных и отправка на вычисление координат
//                DataFromFlacon aData;
                _aData.numOfReferenceDetector_= _main_point; //Номер опорного
                _aData.time_ = QTime::currentTime();    //Время
                _aData.ranges_ = aDR;                   //Откорректированные разности расстояний (максимумы графиков корреляции)

//                qDebug() << "Отправляю в солвер " << aData.ranges_.size();
//                emit signalSendDataFromFlackon(aData);
                _solver->GetData(_aData);

            }
////           _solver->GetData(data);
////           }


////            emit signalSendDataFromFlackon(data);
        }
        _prev_station = point2;
        break;
    case SET_SOLVER:
        ds >> id;
        ds >> track_length;
        ds >> alt;
        _solver->SetHeighApriori(alt);
        _alt = alt;
//        _solver->SetOutDataLength(track_length);
        break;
    case SET_SOLVER_CLEAR:
        _solver->Clear();
        break;
    default:
        break;
    }

}

/// getting coordinates from solver dll
//void FlakonCoordinateCounter::_slotGetDataFromSolver(const DataFromRadioLocation& aData)
//{

//}

void FlakonCoordinateCounter::_slotCatchDataFromRadioLocationAuto(const DataFromRadioLocation &aData)
{
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ AUTO";
    int aLastItem=aData.timeHMSMs.size()-1;

//    QVector<QPointF> vec;
//    vec.push_back(aData.coordLatLon.at(0));

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << aData.timeHMSMs.at(aLastItem);
    ds << 1/*aData.StateMassive_.at(aLastItem)*/;
    ds << aData.latLonStdDev.at(aLastItem);
    ds << aData.coordLatLon;
    ds << aData.airspeed.at(aLastItem);
    ds << aData.heigh.at(aLastItem);
    ds << aData.relativeBearing.at(aLastItem);

	QSharedPointer<IMessageOld> msg(new MessageOld(_id, FLAKON_BPLA_AUTO, ba));
    _subscriber->data_ready(FLAKON_BPLA_AUTO, msg);
}

void FlakonCoordinateCounter::_slotCatchDataFromRadioLocationManual(const DataFromRadioLocation &aData)
{
    int aLastItem=aData.timeHMSMs.size()-1;

    QByteArray *ba = new QByteArray();
    QDataStream ds(ba, QIODevice::ReadWrite);

//    QVector<QPointF> vec;
//    vec.push_back(aData.coordLatLon.at(0));
//    QVector<QPointF> vec;
//    vec.push_back(aData.coordLatLon.at(0));

    ds << aData.timeHMSMs.at(aLastItem);
    ds << 1/*aData.StateMassive_.at(aLastItem)*/;
    ds << aData.latLonStdDev.at(aLastItem);
    ds << aData.coordLatLon;
    ds << aData.airspeed.at(aLastItem);
    ds << _alt;//aData.heigh.at(aLastItem);
    ds << aData.relativeBearing.at(aLastItem);

	QSharedPointer<IMessageOld> msg(new MessageOld(_id, FLAKON_BPLA, ba));
    _subscriber->data_ready(FLAKON_BPLA, msg);
//    QSharedPointer<IMessage> msg(new Message(_id, FLAKON_BPLA, aData));
//    _subscriber->data_ready(FLAKON_BPLA, msg);
}

void FlakonCoordinateCounter::_find_max(int main_point, int second_point)
{
}

void FlakonCoordinateCounter::_checkData()
{
    if(_router->get_station_property() == NULL)
    {
        return;
    }
    QMap<int, int>::iterator it;
    for(it = _map_main_stations.begin(); it != _map_main_stations.end(); ++it)
    {
        if(it.value() != _main_point)
        {
            return;
        }
    }
    if(_map_main_stations.contains(_main_point))
    {
        _map_main_stations.remove(_main_point);
    }

    double aDistance;
    double temp_distance = 0;

    if(!_router->get_station_property()->contains(_main_point))
    {
        return;
    }

    StationProperty* st1 = _router->get_station_property()->value(_main_point);

    StationProperty* st2;

    for (int i = 0; i < _map_main_stations.size() /*+ 1*/; i++) //Номер пункта, с которым сворачивается основной
    {

        if(!_router->get_station_property()->contains(i))
        {
            return;
        }
        st2 = _router->get_station_property()->value(i);

        temp_distance = st2->get_direction_distance(st1, st2);
        aDistance = qAbs(temp_distance);    ///getPointsDistance(0, 1) - выдает расстояние в метрах, между нулевым и 1м пунктами
        if ((_map_maxs.value(i) < -aDistance * 1.2) || (_map_maxs.value(i) > aDistance * 1.2))
        {
            _map_maxs.insert(i, std::numeric_limits<double>::quiet_NaN());
        }
        if (_map_benchmarks.value(i) < _corr_threshold)
        {
            _map_maxs.insert(i, std::numeric_limits<double>::quiet_NaN());
        }
    }
}


void FlakonCoordinateCounter::_setSolverDataSize(int aSize)
{
    if ((aSize>10) && (aSize<2000)) _solver->SetOutDataLength(aSize);
}

void FlakonCoordinateCounter::_setSolverAnalyzeSize(int aSize)
{
    if ((aSize>10) && (aSize<200)) _solver->SetStateAnalizeCount(aSize);
}


void FlakonCoordinateCounter::initSolver()
{
    //Solver
    _solver = new Solver();

    //Размер приходящих данных
    _setSolverDataSize(100);

    //Кол-во данных для определения движения
    _setSolverAnalyzeSize(60);


    connect(this, SIGNAL(signalSendDataFromFlackon(const DataFromFlacon&)), _solver, SLOT(GetData(const DataFromFlacon&)));
//    connect(_solver, SIGNAL(signal_sendDataFromRadioLocation(const DataFromRadioLocation&)), this, SLOT(_slotGetDataFromSolver(const DataFromRadioLocation&)));
    connect(_solver,SIGNAL(signal_sendDataFromRadioLocation(const DataFromRadioLocation&)), this, SLOT(_slotCatchDataFromRadioLocationAuto(const DataFromRadioLocation&)));
    connect(_solver,SIGNAL(signal_sendDataFromRadioLocationManualHeigh(const DataFromRadioLocation&)), this, SLOT(_slotCatchDataFromRadioLocationManual(const DataFromRadioLocation&)));
}
