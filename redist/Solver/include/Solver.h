#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include "ISolver.h"
#include "ISolverListener.h"
#include "solver_global.h"
#include <tuple>
#include <set>

/* Предъобявление реализации солвера */
class SolverImpl;

/**
 *@class Solver
 * класс решателя задачи местоопределения по разностно-дальномерному методу
 * @note принимает на вход данные типа DataFromFlacon через слот GetData
 * @note выдает результаты расчетов с по средствам сигналов 
		 signal_sendDataFromRadioLocation, signal_sendDataFromRadioLocationManualHeigh,
		 signal_sendOneDataFromRadioLocation и signal_sendHyperbolesFromRadioLocation
 * @note то, какие сигналы будет генерировать решатель задается с помощью SolverType 
 */
class SOLVERSHARED_EXPORT Solver : public QObject, public ISolver
{
	Q_OBJECT
public:

	/**
	 * Конструктор по умолчанию
	 * @note создается пустой солвер
	 */
	Solver(QObject* parent = 0);

	/**
	 * Задать зону ответственности
	 * @param bottomLeft - левая нижняя точка зоны ответсвенности [Lat, Lon]
	 * @param topRight - правая верхняя точка зоны ответсвенности [Lat, Lon]
	 * @param H - максимально возможная высота цели над уровнем моря [м]
	 */
    virtual void SetAreaOfResponsibility(const QPointF& bottomLeft,
										 const QPointF& topRight,
										 const double H=9000.0);

	/* Добавить какой-либо тип решателя */
	virtual void AddSolverType(const SolverType& type);

	/* Убрать какой-либо имеющийся тип решателя */
	virtual void RemoveSolverType(const SolverType& type);

	/**
	 * Задать длину выходных данных
	 * @param length - максимальный размер контейнеров в DataFromRadioLocation
	 */
    virtual void SetOutDataLength(const size_t length);

	/**
	 * Задать высоту, на которой летить цель
	 * @param heigh - заранее известная высота цели над уровнем моря [м]
	 */
	virtual void SetHeighApriori(const double heigh);

    /* Очистить историю наблюдений солвера */
    virtual void Clear();
	
	/**
	 * Добавить станцию решателю
	 * @param lat - широта станции 
	 * @param lon - долгота станции 
	 * @param alt - высота станции 
	 * @note в случае неуспеха возвращает -1
	 * @return id - внутренний идентификатор станции 
	 */
	virtual int AddStation(const double lat, const double lon, const double alt);
	
	/**
	 * Удалить станцию решателю
	 * @return признак успеха операции 
	 */
	virtual bool RemoveStation(const int id) ;
	
	/**
	 * Зарегистрировать слушателя
	 * @param listener - слушатель 
	 * @note одновременно может подключиться только один слушатель
	 * @return признак успеха операции 
	 */
	virtual bool RegisterListener(ISolverListener* listener);
	
	/**
	 * Дерегистрировать слушателя
	 * @param listener - слушатель 
	 * @return признак успеха операции 
	 */
	virtual bool DeregisterListener(ISolverListener* listener);

	/**
	 * Получить данные
	 * @param data - данные с разностями расстояний
	 */
    virtual void GetData(const DataFromFlacon& data);

	/* деструктор */
	~Solver(){}

private:

	/* реализация солвера */
	std::shared_ptr<SolverImpl> solverImpl_;

};

#endif // SOLVER_H
