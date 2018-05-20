#ifndef SOLVERGENERAL_H
#define SOLVERGENERAL_H



///////////////////////////////   S O L V E R   ///////////////////////////////

#include "ISolver.h"
#include "ISolverListener.h"
#include "solver_global.h"
#include <tuple>
#include <set>

/* ѕредъоб€вление реализации солвера */
class SolverImpl;
 
/**
 * @class Solver
 * класс решател€ задачи местоопределени€ по разностно-дальномерному методу
 * @note принимает на вход данные типа DataFromFlacon через слот GetData
 * @note выдает результаты расчетов с по средствам сигналов 
		 signal_sendDataFromRadioLocation, signal_sendDataFromRadioLocationManualHeigh,
		 signal_sendOneDataFromRadioLocation и signal_sendHyperbolesFromRadioLocation
 * @note то, какие сигналы будет генерировать решатель задаетс€ с помощью SolverType 
 */
class SOLVERSHARED_EXPORT Solver : public QObject, public ISolver
{
	Q_OBJECT
public:

	/**
	 *  онструктор по умолчанию
	 * @note создаетс€ пустой солвер
	 */
	Solver(QObject* parent = 0);

	/**
	 * «адать зону ответственности
	 * @param bottomLeft - лева€ нижн€€ точка зоны ответсвенности [Lat, Lon]
	 * @param topRight - права€ верхн€€ точка зоны ответсвенности [Lat, Lon]
	 * @param H - максимально возможна€ высота цели над уровнем мор€ [м]
	 */
    virtual void SetAreaOfResponsibility(const QPointF& bottomLeft,
										 const QPointF& topRight,
										 const double H=9000.0);

	/* ƒобавить какой-либо тип решател€ */
	virtual void AddSolverType(const SolverType& type);

	/* ”брать какой-либо имеющийс€ тип решател€ */
	virtual void RemoveSolverType(const SolverType& type);

	/**
	 * «адать длину выходных данных
	 * @param length - максимальный размер контейнеров в DataFromRadioLocation
	 */
    virtual void SetOutDataLength(const size_t length);

	/**
	 * «адать высоту, на которой летить цель
	 * @param heigh - заранее известна€ высота цели над уровнем мор€ [м]
	 */
	virtual void SetHeighApriori(const double heigh);

    /* ќчистить историю наблюдений солвера */
    virtual void Clear();
	
	/**
	 * ƒобавить станцию решателю
	 * @param lat - широта станции 
	 * @param lon - долгота станции 
	 * @param alt - высота станции 
	 * @note в случае неуспеха возвращает -1
	 * @return id - внутренний идентификатор станции 
	 */
	virtual int AddStation(const double lat, const double lon, const double alt);
	
	/**
	 * ”далить станцию решателю
	 * @return признак успеха операции 
	 */
	virtual bool RemoveStation(const int id) ;
	
	/**
	 * «арегистрировать слушател€
	 * @param listener - слушатель 
	 * @note одновременно может подключитьс€ только один слушатель
	 * @return признак успеха операции 
	 */
	virtual bool RegisterListener(ISolverListener* listener);
	
	/**
	 * ƒерегистрировать слушател€
	 * @param listener - слушатель 
	 * @return признак успеха операции 
	 */
	virtual bool DeregisterListener(ISolverListener* listener);

	/**
	 * ѕолучить данные
	 * @param data - данные с разност€ми рассто€ний
	 */
    virtual void GetData(const DataFromFlacon& data);

	/* деструктор */
	~Solver(){}

	/* вернуть указатель на реализацию */
	const SolverImpl* GetImpl()const { return solverImpl_.get(); }

private:

	/* реализаци€ солвера */
	std::shared_ptr<SolverImpl> solverImpl_;

};

#endif // SOLVER_H
