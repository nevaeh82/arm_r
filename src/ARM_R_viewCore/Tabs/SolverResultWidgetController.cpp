#include "SolverResultWidgetController.h"

SolverResultWidgetController::SolverResultWidgetController(QObject* parent):
	QObject(parent)
{
	m_view = NULL;
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
	QByteArray data = argument.toByteArray();

	if( method == RPC_SLOT_SERVER_SEND_BPLA_RESULT ) {
		m_view->appendSolverResult(data);
		return;
	}

}

void SolverResultWidgetController::slotShowWidget()
{
	if(m_view != NULL)
	{
		m_view->show();
	}
}
