#ifndef SOLVERRESULTWIDGETCONTROLLER_H
#define SOLVERRESULTWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "SolverResultWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

#include "SolverPacket1.pb.h"

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
	SolverResultWidget* getView();

public slots:
	void slotShowWidget();


signals:
	void onMethodCalledSignal(QString, QVariant);

	void solverResult(QByteArray);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);

private:
	QString getSolverResultToString(const SolveResult &result);
	void addResultToLog(const QByteArray &data);
	void addResultToLog1(const QByteArray &data);

};

#endif // SOLVERRESULTWIDGETCONTROLLER_H
