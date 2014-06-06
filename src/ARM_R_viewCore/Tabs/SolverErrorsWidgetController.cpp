#include "ISolver.h"
#include "SolverErrorsWidgetController.h"

SolverErrorsWidgetController::SolverErrorsWidgetController(QObject* parent):
	QObject(parent)
{
	m_view = NULL;

	connect(this, SIGNAL(onMethodCalledSignal(QString,QVariant)), this, SLOT(onMethodCalledSlot(QString,QVariant)));
}

SolverErrorsWidgetController::~SolverErrorsWidgetController()
{
	if(m_view != NULL)
	{
		delete m_view;
	}
}

void SolverErrorsWidgetController::appendView(SolverErrorsWidget *view)
{
	m_view = view;
}

void SolverErrorsWidgetController::onMethodCalled(const QString &method, const QVariant &argument)
{
	emit onMethodCalledSignal(method, argument);
}

SolverErrorsWidget*SolverErrorsWidgetController::getView()
{
	return m_view;
}

void SolverErrorsWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}

void SolverErrorsWidgetController::addResultToLog(const QByteArray& inData)
{
	QByteArray data = inData;

	QDataStream ds(&data, QIODevice::ReadOnly);
	QString errorText;
	ds >> errorText;

	m_view->appendSolverError(errorText);

}

void SolverErrorsWidgetController::onMethodCalledSlot(QString method, QVariant argument)
{
	if( method == RPC_SLOT_SERVER_SEND_SOLVER_ERRORS ) {
		addResultToLog(argument.toByteArray());
		return;
	}
}
