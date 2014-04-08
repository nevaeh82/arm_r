#ifndef SOLVERLISTENER_H
#define SOLVERLISTENER_H

#include <QObject>
#include "ISolverListener.h"

class SolverListener : public QObject, public ISolverListener
{
	Q_OBJECT
public:
	SolverListener(QObject* parent = 0);

public:
	void onSendDataFromRadioLocation(const SolveResult& result, const DataFromRadioLocation& allData);
	void onSendDataFromRadioLocationManualHeigh(const SolveResult& result, const DataFromRadioLocation& allData);
	void onSendOneDataFromRadioLocation(const SolveResult& result, const OneDataFromRadioLocation& oneData_1, const OneDataFromRadioLocation& oneData_2);
	void onSendHyperbolesFromRadioLocation(const SolveResult& result, const HyperbolesFromRadioLocation& hyperb);
	void onErrorOccured(const ErrorType& error_type, const QString& str);
};

#endif // SOLVERLISTENER_H
