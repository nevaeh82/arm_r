#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include <QObject>
#include <QVector>
#include <QTime>
#include "Solver_global.h"

class SolverGeo2d;
class SolverGeo3d;

//получаемые данные
struct DataFromFlacon;
struct DataFromRadioLocation;
struct CopyDataFromRadioLocation;
class QPointF;

class SOLVERSHARED_EXPORT Solver:public QObject
{
    Q_OBJECT
public:
    Solver();

    //задать зону ответственности
    void SetAreaOfResponsibility(const QPointF& bottomLeft,
                                 const QPointF& topRight,const double H=-1.0);

    //задать длину выходных данных
    void SetOutDataLength(const int length);

    //задать количество отсчетов, необходимое для определения того,
    //движется источник или стоит
    void SetStateAnalizeCount(const int count);

    //Очистить солвер
    void Clear();

    ~Solver();

public slots:
    //Получить данные
    void GetData(DataFromFlacon& data);
    //посчитать одну порцию данных
    void GetOneData(DataFromFlacon& data);

signals:
    //Отправка рассчитанных данных траектории
    void signal_sendDataFromRadioLocation(const DataFromRadioLocation& allData);
    //отправка одной порции расчитанных данных
    void signal_sendOneDataFromRadioLocation(const DataFromRadioLocation& oneData);

private slots:
    //Получение данных расчета траектории с 2d солвера
    void getDataFromRadioLocation2d(const DataFromRadioLocation& allData);
    //Получение данных расчета траектории  с 3d солвера
    void getDataFromRadioLocation3d(const DataFromRadioLocation& allData);
    //Получение одной порции данных c расчета без истории с 2d солвера
    void getOneDataFromRadioLocation2d(const DataFromRadioLocation& oneData);
    //Получение одной порции данных c расчета без истории с 3d солвера
    void getOneDataFromRadioLocation3d(const DataFromRadioLocation& oneData);
private:
    //скопировать рассчитанные данные из двумерной задачи
    void CopyData2d(const DataFromRadioLocation& allData);
    //скопировать рассчитанные данные из трехмерной задачи
    void CopyData3d(const DataFromRadioLocation& allData);
    //скорректировать координаты с помощью высоты
    void CorrectCoordinatesByHeigh();
    SolverGeo3d* solverGeo3d_;//солвер для трехмерной задачи
    SolverGeo2d* solverGeo2d_;//солвер для двумерной задачи
    CopyDataFromRadioLocation* data_;//рассчитанные данные с использованием истории
    //рассчитанные данные с использованием истории, с корректировкой координат по высоте
    CopyDataFromRadioLocation* data_corrected_heigh_;
    DataFromRadioLocation* one_data_;//рассчитанные данные без использования истории
};

#endif // SOLVER_H
