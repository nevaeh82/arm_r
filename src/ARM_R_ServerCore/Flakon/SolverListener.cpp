#include "SolverListener.h"

SolverListener::SolverListener(QObject* parent):
	QObject(parent)
{
}

void SolverListener::onSendDataFromRadioLocation(const SolveResult &result, const DataFromRadioLocation &allData)
{

}

void SolverListener::onSendDataFromRadioLocationManualHeigh(const SolveResult &result, const DataFromRadioLocation &allData)
{

}

void SolverListener::onSendOneDataFromRadioLocation(const SolveResult &result, const OneDataFromRadioLocation &oneData_1, const OneDataFromRadioLocation &oneData_2)
{

}

void SolverListener::onSendHyperbolesFromRadioLocation(const SolveResult &result, const HyperbolesFromRadioLocation &hyperb)
{

}

void SolverListener::onErrorOccured(const ErrorType &error_type, const QString &str)
{

}
