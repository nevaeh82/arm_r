#ifndef RADIOLOCATIONINTERFACE_H
#define RADIOLOCATIONINTERFACE_H

#include <QTime>
#include <QVector>

//возможные состояния движения самолета: летит, стоит, неизвестно
enum State {FLY,STAND,UNKNOWN};

///////////////   D A T A  F R O M  R A D I O L O C A T I O N   ///////////////

//Структура для выходных данных
struct DataFromRadioLocation {
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


///////////////   D A T A  F R O M  R A D I O L O C A T I O N   ///////////////

//Структура для выходных данных расчета одной порции данных
struct OneDataFromRadioLocation {
    //координаты [Lat; Lon], град
    QPointF coordLatLon;
    //высота над уровнем моря, м
    double heigh;
    //Среднекватратичное отклонение по широте и долготе соответственно, [м; м]
    QPointF latLonStdDev;
    //Среднекватратичное отклонение по высоте, м
    double heighStdDev;
    //время получения измерений
    QTime timeHMSMs;
};


//////////////////////   D A T A  F R O M  F L A C O N   //////////////////////

//получаемые данные
struct DataFromFlacon {
	//вектор разностей расстояний
	QVector<double> ranges_;
	//Номер опорного датчика
	int numOfReferenceDetector_;
	//время получения измерений
	QTime time_;
};

////////   H Y P E R B O L E S   F R O M   R A D I O L O C A T I O N   ////////

//Структура для выходных данных расчета гипербол
struct HyperbolesFromRadioLocation {
	//Список гипербол, которые представлены векторами точек [Lat,Lon]
	QList<QVector<QPointF> > hyperboles_list;
	//время получения измерений, для которых рассчитаны гиперболы
	QTime timeHMSMs;
};


/**
 * @enum ErrorType
 * Возможные типы ошибок решателя
 * @param SETUP_ERROR - ошибка, возникшая при настройке солвера
 * @param INPUT_DATA_ERROR - ошибка, возникшая при неудачной попытке
                             обработать входные данные
 * @param INTERNAL_ERROR - внутренняя ошибка солвера
 */
enum ErrorType { SETUP_ERROR, INPUT_DATA_ERROR, INTERNAL_ERROR };

/**
 * @enum SolveResult
 * Результат расчета 
 * @param SOLVED - данные благополучно обработаны 
 * @param ERROR_OCCURED - в ходе вычислений появились ошибки
 * @param NOT_ENOUGH_DATA - в солвер добавлено недостаточно данных для анализа их точности
 * @param THERE_IS_NO_SOLUTION - решения не существует
 * @param CANT_DETERMINE_REAL_TRAJECTORY - солвер не может определить реальную траекторию из двух возможных
 * @param TOO_FEW_RANGES - слишком мало корректных разностей расстояний для расчета 
 * @param TOO_LOW_SOLUTION_ACCURACY - слишком низкая точность получаемого решения
 * @param TOO_LOW_DATA_ACCURACY - слишком низкая точность входных данных
 * @param COORDS_DOES_NOT_HIT_IN_AREA - рассчитанные координаты не попадают в зону ответственности
 * @param COORDS_DOES_NOT_HIT_IN_STROB -  рассчитанные координаты не попадают в строб захвата
 */
enum SolveResult { SOLVED, ERROR_OCCURED, NOT_ENOUGH_DATA, THERE_IS_NO_SOLUTION,
				   CANT_DETERMINE_REAL_TRAJECTORY, TOO_LOW_DATA_ACCURACY,
	               TOO_FEW_RANGES, TOO_LOW_SOLUTION_ACCURACY, 
				   COORDS_DOES_NOT_HIT_IN_AREA, COORDS_DOES_NOT_HIT_IN_STROB };


#endif // RADIOLOCATIONINTERFACE_H
