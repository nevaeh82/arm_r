#include "ISolver.h"
#include "SolverResultWidgetController.h"

SolverResultWidgetController::SolverResultWidgetController(QObject* parent):
	QObject(parent)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));
}

SolverResultWidgetController::~SolverResultWidgetController()
{
	if(m_view != NULL)
	{
		delete m_view;
	}
}

void SolverResultWidgetController::appendView(SolverResultWidget *view)
{
	m_view = view;
}

void SolverResultWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

SolverResultWidget*SolverResultWidgetController::getView()
{
	return m_view;
}

void SolverResultWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void SolverResultWidgetController::addResultToLog(const QByteArray& inData)
{
	QByteArray data = inData;

	QDataStream ds(&data, QIODevice::ReadOnly);
	int sourceType;
	int inResult;

	ds >> sourceType;
	ds >> inResult;

	SolveResult result = (SolveResult)inResult;

	QString source;

	switch(sourceType)
	{
		case AUTO_HEIGH:
			source = tr("AUTO");
			break;
		case MANUAL_HEIGH:
			source = tr("MANUAL");
			break;
		case ONE_DATA:
			source = tr("ALONE");
			break;
		case HYPERBOLES:
			source = tr("HYPERBOLES");
			break;
		default:
			break;
	}

	QString log = source + " - " + getSolverResultToString(result);

	m_view->appendSolverResult(log);

}

void SolverResultWidgetController::addResultToLog1(const QByteArray& inData)
{
	QByteArray data = inData;

	QDataStream ds(&data, QIODevice::ReadOnly);
	int sourceType;
	QByteArray solverData;

	ds >> sourceType;
	ds >> solverData;

	SolverProtocol::EstimateQuality quality = SolverProtocol::EstimateQuality(sourceType);

	QString source;

	switch(quality)
	{
		case SolverProtocol::GOOD_QUALITY:
			source = tr("Good quality");
			break;
		case SolverProtocol::BAD_QUALITY:
			source = tr("Bad quality");
			break;
		default:
			break;
	}

	QString log = source;

	m_view->appendSolverResult(QDateTime::currentDateTime().toString() + tr(" Found solve ") + log);

	emit solverResult(solverData);
}

void SolverResultWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
	if( method == RPC_SLOT_SERVER_SEND_BPLA_RESULT ) {
		addResultToLog(argument.toByteArray());
		return;
	} else if( method == RPC_SLOT_SERVER_SEND_BPLA_RESULT_1 ) {
		addResultToLog1(argument.toByteArray());
		return;
	}
//	else if( method == RPC_SLOT_SERVER_SEND_ANSWER_RESULT_1 ) {
//		addResultToLog1(argument.toByteArray());
//		return;
//	}
}

QString SolverResultWidgetController::getSolverResultToString(const SolveResult &result)
{
	QString resultString;
	switch(result)
	{
		case SOLVED:
			resultString = tr("Successed solved");
			break;
		case ERROR_OCCURED:
			resultString = tr("Some errors during solve");
			break;
		case NOT_ENOUGH_DATA:
			resultString = tr("Not enough data for solve");
			break;
		case THERE_IS_NO_SOLUTION:
			resultString = tr("There is no solution");
			break;
		case CANT_DETERMINE_REAL_TRAJECTORY:
			resultString = tr("No solve cause 2 trajectory");
			break;
		case TOO_FEW_RANGES:
			resultString = tr("Not enough distances for solve");
			break;
		case TOO_LOW_SOLUTION_ACCURACY:
			resultString = tr("There is not enough accuracy for solve");
			break;
		case TOO_LOW_DATA_ACCURACY:
			resultString = tr("There is not enough input accuracy for solve");
			break;
		case COORDS_DOES_NOT_HIT_IN_AREA:
			resultString = tr("Solves data is out of range of resposible zone");
			break;
		case COORDS_DOES_NOT_HIT_IN_STROB:
			resultString = tr("Coords is out of catching strobe");
			break;
		default:
			break;
	}
	return resultString;

}
