#ifndef RADIOLOCATIONINTERFACE_H
#define RADIOLOCATIONINTERFACE_H

#include <QTime>
#include <QVector>

//��������� ��������� �������� ��������: �����, �����, ����������
enum State {FLY,STAND,UNKNOWN};

///////////////   D A T A  F R O M  R A D I O L O C A T I O N   ///////////////

//��������� ��� �������� ������
struct DataFromRadioLocation
{
    //������ ���������
    QVector<QPointF> coordLatLon;
    //������ �����
    QVector<double> heigh;
    //������ ��� ���������
    QVector<QPointF> latLonStdDev;
    //������ ��� ������
    QVector<double> heighStdDev;
    //������ ������� ��������� ���������
    QVector<QTime> timeHMSMs;
    //������ �������
    QVector<double> airspeed;
    //������ ��� ��������
    QVector<double> airSpeedStdDev;
    //������ �������� ������
    QVector<double> relativeBearing;
    //������ ��������� ��������
    QVector<State> StateMassive_;
};


//////////////////////   D A T A  F R O M  F L A C O N   //////////////////////

//���������� ������
struct DataFromFlacon
{
    //������ ��������� ����������
    QVector<double> ranges_;
    //����� �������� �������
    int numOfReferenceDetector_;
    //����� ��������� ���������
    QTime time_;

};


#endif // RADIOLOCATIONINTERFACE_H
