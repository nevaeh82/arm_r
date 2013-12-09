#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QTime>
#include "Solver_global.h"
#include <tuple>

class SolverGeo2d;
class SolverGeo3d;

//���������� ������
struct DataFromFlacon;
struct DataFromRadioLocation;
struct CopyDataFromRadioLocation;

enum SolverType{AUTO_HEIGH,MANUAL_HEIGH,MANUAL_AND_AUTO_HEIGH};
class SOLVERSHARED_EXPORT Solver:public QObject
{
    Q_OBJECT
public:
    Solver(const SolverType type=MANUAL_AND_AUTO_HEIGH);

    //������ ���� ���������������
    void SetAreaOfResponsibility(const QPointF& bottomLeft,
                                 const QPointF& topRight,const double H=-1.0);

    //������ ����� �������� ������
    void SetOutDataLength(const int length);

    //������ ���������� ��������, ����������� ��� ����������� ����,
    //�������� �������� ��� �����
    void SetStateAnalizeCount(const int count);

    //������ ������, �� ������� ������ ����
    void SetHeighApriori(double heigh);

    //�������� ������
    void Clear();

    ~Solver();

public slots:
    //�������� ������
    void GetData(const DataFromFlacon& data);
    //��������� ���� ������ ������
    void GetOneData(const DataFromFlacon& data);

signals:
    //�������� ������������ ������ ����������
    void signal_sendDataFromRadioLocation(const DataFromRadioLocation& allData);
    //�������� ������������ ������ ���������� � �������������� ������������ ������
    void signal_sendDataFromRadioLocationManualHeigh(const DataFromRadioLocation& allData);
    //�������� ����� ������ ����������� ������
    void signal_sendOneDataFromRadioLocation(const DataFromRadioLocation& oneData);

private slots:
    //��������� ������ ������� ���������� � 2d ������� � ������ �������� ������
    void getDataFromRadioLocation2dManualHeigh(const DataFromRadioLocation& allData);
    //��������� ������ ������� ���������� � 2d ������� � �������������� �������� ������
    void getDataFromRadioLocation2dAutoHeigh(const DataFromRadioLocation& allData);
    //��������� ������ ������� ����������  � 3d �������
    void getDataFromRadioLocation3d(const DataFromRadioLocation& allData);
    //��������� ����� ������ ������ c ������� ��� ������� � 2d �������
    void getOneDataFromRadioLocation2d(const DataFromRadioLocation& oneData);
    //��������� ����� ������ ������ c ������� ��� ������� � 3d �������
    void getOneDataFromRadioLocation3d(const DataFromRadioLocation& oneData);
private:
    mutable SolverType solverType_;//��� �������
    SolverGeo3d* solverGeo3d_;//������ ��� ���������� ������
    //������ ��� ��������� ������ � ������ �������� ������
    SolverGeo2d* solverGeo2dManualHeigh_;
    //������ ��� ��������� ������ � �������������� ������������ ������
    SolverGeo2d* solverGeo2dAutoHeigh_;
    //������������ ������ � �������������� �������, � ������ �������� ������
    CopyDataFromRadioLocation* dataManualHeigh_;
    //������������ ������ � �������������� �������, � �������������� ������������ ������
    CopyDataFromRadioLocation* dataAutoHeigh_;
    std::tuple<QTime,double,double> lastHeighTuple_;
    DataFromRadioLocation* one_data_;//������������ ������ ��� ������������� �������
    double heighApriori_;
};

#endif // SOLVER_H
