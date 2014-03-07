#ifndef BLASIMULATOR_H
#define BLASIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QPointF>
#include <QDataStream>
#include <QByteArray>
#include <math.h>

#include "../../Common/Router.h"
#include "../../Common/ISubscriber.h"
#include "../../RPC/Message.h"


////Структура для выходных данных
//struct DataFromRadioLocation
//{
//    //вектор координат
//    QVector<QPointF> coordLatLon;
//    //вектор высот
//    QVector<double> heigh;
//    //вектор СКО координат
//    QVector<double> coordStdDev;
//    //вектор времени получения измерений
//    QVector<QTime> timeHMSMs;
//    //вектор скорсти
//    QVector<double> airspeed;
//    //вектор СКО скорости
//    QVector<double> airSpeedStdDev;
//    //вектор курсовых уголов
//    QVector<double> relativeBearing;
//    //Вектор состояния движения
//    QVector<State> StateMassive_;
//};

class BLASimulator : public QObject
{
    Q_OBJECT
public:
    BLASimulator(IRouter* router, double radius, bool red);
    ~BLASimulator();

public:
    void start();
    void stop();

private slots:
    void _slot_simulation();

private:
    int             _id;
    QTimer*         _timer;
    IRouter*        _router;
    ISubscriber*    _subscriber;

    double          _radius;
    bool            _red;
};

#endif // BLASIMULATOR_H
