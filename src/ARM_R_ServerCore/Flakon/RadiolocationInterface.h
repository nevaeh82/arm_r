#ifndef RADIOLOCATIONINTERFACE_H
#define RADIOLOCATIONINTERFACE_H

#include <QTime>
#include <QVector>

//возможные состояния движения самолета: летит, стоит, неизвестно
enum State {FLY,STAND,UNKNOWN};

///////////////   D A T A  F R O M  R A D I O L O C A T I O N   ///////////////

//Структура для выходных данных
struct DataFromRadioLocation
{
    //вектор координат
    QVector<QPointF> coordLatLon;
    //вектор высот
    QVector<double> heigh;
    //вектор СКО координат
    QVector<QPointF> latLonStdDev;
    //вектор СКО высоты
    QVector<double> heighStdDev;
    //вектор времени получения измерений
    QVector<QTime> timeHMSMs;
    //вектор скорсти
    QVector<double> airspeed;
    //вектор СКО скорости
    QVector<double> airSpeedStdDev;
    //вектор курсовых уголов
    QVector<double> relativeBearing;
    //Вектор состояния движения
    QVector<State> StateMassive_;
};


//////////////////////   D A T A  F R O M  F L A C O N   //////////////////////

//получаемые данные
struct DataFromFlacon
{
    //вектор разностей расстояний
    QVector<double> ranges_;
    //Номер опорного датчика
    int numOfReferenceDetector_;
    //время получения измерений
    QTime time_;

};


#endif // RADIOLOCATIONINTERFACE_H
