#pragma once

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "SolverSetupWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

#include "SolverPacket1.pb.h"

class SolverSetupWidgetController : public QObject, public IController<SolverSetupWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit SolverSetupWidgetController(QObject *parent = 0);
	virtual ~SolverSetupWidgetController();


private:
	SolverSetupWidget* m_view;

	void addPreambula(QByteArray &data);
public:
	void appendView(SolverSetupWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
	SolverSetupWidget* getView();

public slots:
	void slotShowWidget();


signals:
	void onMethodCalledSignal(QString, QVariant);

	void onSendSolverCommandSettings(QByteArray data);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);

	void slotSendSettings();
	void slotGetVersion();

};

