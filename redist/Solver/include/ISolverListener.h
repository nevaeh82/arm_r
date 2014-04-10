#ifndef ISOLVERLISTENER_H
#define ISOLVERLISTENER_H

#include "RadiolocationInterface.h"



/**
 *@class ISolverListener
 * ��������� ��������� �������
 */
class ISolverListener
{
public:
	/* �������� ������������ ������ ���������� � �������������� ������������ ������ */
	virtual void onSendDataFromRadioLocation
		(const SolveResult& result, const DataFromRadioLocation& allData)=0;

	/* �������� ������������ ������ ���������� � �������������� ������������ ������ */
    virtual void onSendDataFromRadioLocationManualHeigh
		(const SolveResult& result, const DataFromRadioLocation& allData)=0;

	/** 
	 * �������� ���� ��������� ��������� ������� ������������ ������ 
	 * @note � ������ ���� �������� ���������� ���� �������, �� oneData_1 = oneData_2
	 */
	virtual void onSendOneDataFromRadioLocation
		(const SolveResult& result, const OneDataFromRadioLocation& oneData_1,
		 const OneDataFromRadioLocation& oneData_2)=0;

	/* �������� ��������, ������������ �� ��������� ���������� */
	virtual void onSendHyperbolesFromRadioLocation
		(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb)=0;

	/* �������� ��������� �� ������ */
	virtual void onErrorOccured(const ErrorType& error_type, 
								const QString& str)=0;

	virtual ~ISolverListener(){}
};

#endif // ISOLVERLISTENER_H