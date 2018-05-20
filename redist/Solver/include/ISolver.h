#ifndef ISOLVER_H
#define ISOLVER_H

#include "RadiolocationInterface.h"


/* Предъобявление реализации слушателя */
class ISolverListener;

/**
 * @enum SolverType
 * Возможные типы решателя
 * @param AUTO_HEIGH - решатель вычисляющий траекторию с автоматическим определением
					   высоты, генерирует сигналы signal_sendDataFromRadioLocation
 * @param MANUAL_HEIGH - решатель вычисляющий траекторию с ручным определением высоты,
                         генерирует сигналы signal_sendDataFromRadioLocationManualHeigh
 * @param ONE_DATA - решатель вычисляющий одинарночные отметки с координатами, 
                     генерирует сигналы signal_sendOneDataFromRadioLocation
 * @param HYPERBOLES - решатель вычисляющий гиперболы для получаемых разностей расстояний
                       определением высоты, генерирует сигналы signal_sendHyperbolesFromRadioLocation
 */
enum SolverType { AUTO_HEIGH, MANUAL_HEIGH, ONE_DATA, HYPERBOLES };



/**
 *@class Solver
 * класс решателя задачи местоопределения по разностно-дальномерному методу
 * @note принимает на вход данные типа DataFromFlacon через метод GetData
 * @note выдает результаты расчетов по средствам интерфейса ISolverListener и регистрации слушателей
 * @note то, какие сигналы будет генерировать решатель задается с помощью SolverType 
 */
class ISolver
{
public:

	/* деструктор */
	virtual ~ISolver(){}

	/**
	 * Задать зону ответственности
	 * @param bottomLeft - левая нижняя точка зоны ответсвенности [Lat, Lon]
	 * @param topRight - правая верхняя точка зоны ответсвенности [Lat, Lon]
	 * @param H - максимально возможная высота цели над уровнем моря [м]
	 */
    virtual void SetAreaOfResponsibility(const QPointF& bottomLeft,
										 const QPointF& topRight,
										 const double H=9000.0) = 0;

	/* Добавить какой-либо тип решателя */
	virtual void AddSolverType(const SolverType& type) = 0;

	/* Убрать какой-либо имеющийся тип решателя */
	virtual void RemoveSolverType(const SolverType& type) = 0;

	/**
	 * Задать длину выходных данных
	 * @param length - максимальный размер контейнеров в DataFromRadioLocation
	 */
    virtual void SetOutDataLength(const size_t length) = 0;

	/**
	 * Задать высоту, на которой летить цель
	 * @param heigh - заранее известная высота цели над уровнем моря [м]
	 */
	virtual void SetHeighApriori(const double heigh) = 0;

    /**
	  * Очистить историю наблюдений солвера 
	  * @note конфигурация датчиков при этом остается нетронутой
	  */
    virtual void Clear() = 0;
	
	/**
	 * Добавить станцию решателю
	 * @param lat - широта станции 
	 * @param lon - долгота станции 
	 * @param alt - высота станции 
	 * @return id - внутренний идентификатор станции 
	 * @note в случае неуспеха возвращает -1
	 */
	virtual int AddStation(const double lat, const double lon, const double alt) = 0;
	
	/**
	 * Задать высоту, на которой летить цель
	 * @param id - внутренний идентификатор станции 
	 * @return признак успеха операции 
	 */
	virtual bool RemoveStation(const int id) = 0;
	
	/**
	 * Зарегистрировать слушателя
	 * @param listener - слушатель 
	 * @return признак успеха операции 
	 */
	virtual bool RegisterListener(ISolverListener* listener) = 0;
	
	/**
	 * Дерегистрировать слушателя
	 * @param listener - слушатель 
	 * @return признак успеха операции 
	 */
	virtual bool DeregisterListener(ISolverListener* listener) = 0;

	/**
	 * Получить данные
	 * @param data - данные с разностями расстояний
	 */
    virtual void GetData(const DataFromFlacon& data) = 0;
};

#endif // ISOLVER_H
