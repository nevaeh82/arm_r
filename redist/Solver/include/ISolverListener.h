#ifndef ISOLVERLISTENER_H
#define ISOLVERLISTENER_H

#include "RadiolocationInterface.h"

/**
 *@class ISolverListener
 * интерфейс слушателя солвера
 */
class ISolverListener
{
public:
	/* Отправка рассчитанных данных траектории с автоматическим определением высоты */
	virtual void onSendDataFromRadioLocation
		(const SolveResult& result, const DataFromRadioLocation& allData)=0;

	/* Отправка рассчитанных данных траектории с автоматическим определением высоты */
    virtual void onSendDataFromRadioLocationManualHeigh
		(const SolveResult& result, const DataFromRadioLocation& allData)=0;

	/** 
	 * Отправка двух возможных одинарных отметок рассчитанных данных 
	 * @note в случае если возможно рассчитать одну отметку, то oneData_1 = oneData_2
	 */
	virtual void onSendOneDataFromRadioLocation
		(const SolveResult& result, const OneDataFromRadioLocation& oneData_1,
		 const OneDataFromRadioLocation& oneData_2)=0;

	/* Отправка гипербол, рассчитанных по разностям расстояний */
	virtual void onSendHyperbolesFromRadioLocation
		(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb)=0;

	/* Отправка сообщения об ошибке */
	virtual void onErrorOccured(const ErrorType& error_type, 
								const QString& str)=0;

        virtual void onSolver1ProtoData( const int& result, const QByteArray& data ) = 0;
        virtual void onSolver1SetupAnswer(const QByteArray &data) = 0;

	virtual ~ISolverListener(){}
};

#endif // ISOLVERLISTENER_H
