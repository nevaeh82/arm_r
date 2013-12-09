#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include <QObject>
#include <QVector>
#include <QTime>
#include "Solver_global.h"

class SolverGeo2d;
class SolverGeo3d;

//���������� ������
struct DataFromFlacon;
struct DataFromRadioLocation;
struct CopyDataFromRadioLocation;
class QPointF;

class SOLVERSHARED_EXPORT Solver:public QObject
{
    Q_OBJECT
public:
    Solver();

    //������ ���� ���������������
    void SetAreaOfResponsibility(const QPointF& bottomLeft,
                                 const QPointF& topRight,const double H=-1.0);

    //������ ����� �������� ������
    void SetOutDataLength(const int length);

    //������ ���������� ��������, ����������� ��� ����������� ����,
    //�������� �������� ��� �����
    void SetStateAnalizeCount(const int count);

    //�������� ������
    void Clear();

    ~Solver();

public slots:
    //�������� ������
    void GetData(DataFromFlacon& data);
    //��������� ���� ������ ������
    void GetOneData(DataFromFlacon& data);

signals:
    //�������� ������������ ������ ����������
    void signal_sendDataFromRadioLocation(const DataFromRadioLocation& allData);
    //�������� ����� ������ ����������� ������
    void signal_sendOneDataFromRadioLocation(const DataFromRadioLocation& oneData);

private slots:
    //��������� ������ ������� ���������� � 2d �������
    void getDataFromRadioLocation2d(const DataFromRadioLocation& allData);
    //��������� ������ ������� ����������  � 3d �������
    void getDataFromRadioLocation3d(const DataFromRadioLocation& allData);
    //��������� ����� ������ ������ c ������� ��� ������� � 2d �������
    void getOneDataFromRadioLocation2d(const DataFromRadioLocation& oneData);
    //��������� ����� ������ ������ c ������� ��� ������� � 3d �������
    void getOneDataFromRadioLocation3d(const DataFromRadioLocation& oneData);
private:
    //����������� ������������ ������ �� ��������� ������
    void CopyData2d(const DataFromRadioLocation& allData);
    //����������� ������������ ������ �� ���������� ������
    void CopyData3d(const DataFromRadioLocation& allData);
    //��������������� ���������� � ������� ������
    void CorrectCoordinatesByHeigh();
    SolverGeo3d* solverGeo3d_;//������ ��� ���������� ������
    SolverGeo2d* solverGeo2d_;//������ ��� ��������� ������
    CopyDataFromRadioLocation* data_;//������������ ������ � �������������� �������
    //������������ ������ � �������������� �������, � �������������� ��������� �� ������
    CopyDataFromRadioLocation* data_corrected_heigh_;
    DataFromRadioLocation* one_data_;//������������ ������ ��� ������������� �������
};

#endif // SOLVER_H
