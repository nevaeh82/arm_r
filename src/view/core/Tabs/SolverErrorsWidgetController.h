#ifndef SOLVERERRORSWIDGETCONTROLLER_H
#define SOLVERERRORSWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "SolverErrorsWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

class SolverErrorsWidgetController : public QObject, public IController<SolverErrorsWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit SolverErrorsWidgetController(QObject *parent = 0);
	virtual ~SolverErrorsWidgetController();


private:
	SolverErrorsWidget* m_view;

public:
	void appendView(SolverErrorsWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
	SolverErrorsWidget* getView();

public slots:
	void slotShowWidget();


signals:
	void onMethodCalledSignal(QString, QVariant);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);

private:
	void addResultToLog(const QByteArray &data);

};

#endif // SOLVERERRORSWIDGETCONTROLLER_H
