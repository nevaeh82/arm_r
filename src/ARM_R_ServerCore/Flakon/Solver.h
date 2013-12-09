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

//получаемые данные
struct DataFromFlacon;
struct DataFromRadioLocation;
struct CopyDataFromRadioLocation;

enum SolverType{AUTO_HEIGH,MANUAL_HEIGH,MANUAL_AND_AUTO_HEIGH};
class SOLVERSHARED_EXPORT Solver:public QObject
{
    Q_OBJECT
public:
    Solver(const SolverType type=MANUAL_AND_AUTO_HEIGH);

    //задать зону ответственности
    void SetAreaOfResponsibility(const QPointF& bottomLeft,
                                 const QPointF& topRight,const double H=-1.0);

    //задать длину выходных данных
    void SetOutDataLength(const int length);

    //задать количество отсчетов, необходимое дл€ определени€ того,
    //движетс€ источник или стоит
    void SetStateAnalizeCount(const int count);

    //«адать высоту, на которой летить цель
    void SetHeighApriori(double heigh);

    //ќчистить солвер
    void Clear();

    ~Solver();

public slots:
    //ѕолучить данные
    void GetData(const DataFromFlacon& data);
    //посчитать одну порцию данных
    void GetOneData(const DataFromFlacon& data);

signals:
    //ќтправка рассчитанных данных траектории
    void signal_sendDataFromRadioLocation(const DataFromRadioLocation& allData);
    //ќтправка рассчитанных данных траектории с автоматическим определением высоты
    void signal_sendDataFromRadioLocationManualHeigh(const DataFromRadioLocation& allData);
    //отправка одной порции расчитанных данных
    void signal_sendOneDataFromRadioLocation(const DataFromRadioLocation& oneData);

private slots:
    //ѕолучение данных расчета траектории с 2d солвера с ручным заданием высоты
    void getDataFromRadioLocation2dManualHeigh(const DataFromRadioLocation& allData);
    //ѕолучение данных расчета траектории с 2d солвера с автоматическим заданием высоты
    void getDataFromRadioLocation2dAutoHeigh(const DataFromRadioLocation& allData);
    //ѕолучение данных расчета траектории  с 3d солвера
    void getDataFromRadioLocation3d(const DataFromRadioLocation& allData);
    //ѕолучение одной порции данных c расчета без истории с 2d солвера
    void getOneDataFromRadioLocation2d(const DataFromRadioLocation& oneData);
    //ѕолучение одной порции данных c расчета без истории с 3d солвера
    void getOneDataFromRadioLocation3d(const DataFromRadioLocation& oneData);
private:
    mutable SolverType solverType_;//тип солвера
    SolverGeo3d* solverGeo3d_;//солвер дл€ трехмерной задачи
    //солвер дл€ двумерной задачи с ручным заданием высоты
    SolverGeo2d* solverGeo2dManualHeigh_;
    //солвер дл€ двумерной задачи с автоматическим определением высоты
    SolverGeo2d* solverGeo2dAutoHeigh_;
    //рассчитанные данные с использованием истории, с ручным заданием высоты
    CopyDataFromRadioLocation* dataManualHeigh_;
    //рассчитанные данные с использованием истории, с автоматическим определением высоты
    CopyDataFromRadioLocation* dataAutoHeigh_;
    std::tuple<QTime,double,double> lastHeighTuple_;
    DataFromRadioLocation* one_data_;//рассчитанные данные без использовани€ истории
    double heighApriori_;
};

#endif // SOLVER_H
