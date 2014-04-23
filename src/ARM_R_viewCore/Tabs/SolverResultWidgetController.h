#ifndef SOLVERRESULTWIDGETCONTROLLER_H
#define SOLVERRESULTWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "SolverResultWidget.h"
#include "Interfaces/IRpcListener.h"

class SolverResultWidgetController : public QObject, public IController<SolverResultWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit SolverResultWidgetController(QObject *parent = 0);
	virtual ~SolverResultWidgetController();


private:
	SolverResultWidget* m_view;

public:
	void appendView(SolverResultWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

public slots:
	void slotShowWidget();

};

#endif // SOLVERRESULTWIDGETCONTROLLER_H
