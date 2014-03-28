#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include "RadiolocationInterface.h"
#include "solver_global.h"
#include <tuple>
#include <set>

/* Предъобявление реализации солвера */
class SolverImpl;

/**
 * @enum SolverType
 * Возможные типы решателя
 * @param AUTO_HEIGH - решатель вычисляющий траекторию с автоматическим определением
					   высоты, генерирует сигналы signal_sendDataFromRadioLocation
 * @param MANUAL_HEIGH - решатель вычисляющий траекторию с ручным определением высоты,
                         генерирует сигналы signal_sendDataFromRadioLocationManualHeigh
 * @param ONE_DATA - решатель вычисляющий одинарночные отметки с координатами, 
                     генерирует сигналы signal_sendOneDataFromRadioLocation
 */
enum SolverType { AUTO_HEIGH, MANUAL_HEIGH, ONE_DATA };


/**
 *@class Solver
 * класс решателя задачи местоопределения по разностно-дальномерному методу
 * @note принимает на вход данные типа DataFromFlacon через слот GetData
 * @note выдает результаты расчетов с по средствам сигналов 
		 signal_sendDataFromRadioLocation, signal_sendDataFromRadioLocationManualHeigh и
		 signal_sendOneDataFromRadioLocation
 * @note то, какие сигналы будет генерировать решатель задается с помощью SolverType 
 */
class SOLVERSHARED_EXPORT Solver : public QObject
{
    Q_OBJECT
public:

	/**
	 * Конструктор по умолчанию
	 * @note используются все возможные типы решателей
	 */
	Solver();
	/**
	 * Конструктор
	 * @param types - список типов решателей, которые будут использоваться
	 */
	Solver(const std::set<SolverType>& types);

	/**
	 * Задать зону ответственности
	 * @param bottomLeft - левая нижняя точка зоны ответсвенности [Lat, Lon]
	 * @param topRight - правая верхняя точка зоны ответсвенности [Lat, Lon]
	 * @param H - максимально возможная высота цели над уровнем моря [м]
	 */
    void SetAreaOfResponsibility(const QPointF& bottomLeft,
                                 const QPointF& topRight,const double H=9000.0);

	/* Добавить какой-либо тип решателя */
	void AddSolverType(const SolverType& type);

	/* Убрать какой-либо имеющийся тип решателя */
	void RemoveSolverType(const SolverType& type);

	/**
	 * Задать длину выходных данных
	 * @param length - максимальный размер контейнеров в DataFromRadioLocation
	 */
    void SetOutDataLength(const size_t length);

	/**
	 * Задать высоту, на которой летить цель
	 * @param heigh - заранее известная высота цели над уровнем моря [м]
	 */
    void SetHeighApriori(const double heigh);

    /* Очистить историю наблюдений солвера */
    void Clear();

	/* деструктор */
	~Solver(){}
private:
	/* инициализирующий метод */
	void Init();

public slots:

	/**
	 * Получить данные
	 * @param data - данные с разностями расстояний
	 */
    void GetData(const DataFromFlacon& data);

signals:

	/* Отправка рассчитанных данных траектории с автоматическим определением высоты */
    void signal_sendDataFromRadioLocation(const DataFromRadioLocation& allData);

    /* Отправка рассчитанных данных траектории с автоматическим определением высоты */
    void signal_sendDataFromRadioLocationManualHeigh(const DataFromRadioLocation& allData);

	/* Отправка одной отметки рассчитанных данных */
    void signal_sendOneDataFromRadioLocation(const OneDataFromRadioLocation& oneData);

private:
	/* реализация солвера */
	std::shared_ptr<SolverImpl> solverImpl_;

};

#endif // SOLVER_H
