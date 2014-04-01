#ifndef PRM300CONTROLWIDGETCONTROLLER_H
#define PRM300CONTROLWIDGETCONTROLLER_H

#include <QObject>

#include <PwLogger/PwLogger.h>

#include <QMessageBox>

#include "Interfaces/IController.h"
#include "Prm300ControlWidget.h"

#include "ServiceHandler/ServiceHandler.h"

#include "Tabs/RPC/RPCClient.h"

class Prm300ControlWidgetController : public QObject, public IController<Prm300ControlWidget>, public IRpcListener
{
	Q_OBJECT

private:
	/// logger
	Pw::Logger::ILogger* m_logger;

	Prm300ControlWidget* m_view;

	RPCClient* m_rpcClient;

	IDbManager* m_dbManager;

	QString m_stationName;

public:
	explicit Prm300ControlWidgetController(const QString& name, QObject *parent = 0);
	virtual ~Prm300ControlWidgetController();

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	void appendView(Prm300ControlWidget *view);

	void setRpcClient(RPCClient* rpcClient);

	virtual void setDbManager(IDbManager*);

private:
	void init();

signals:

private slots:
	void slotSetAtt1(int value);
	void slotSetAtt2(int value);
	void slotSetFilter(int index);
};

#endif // PRM300CONTROLWIDGETCONTROLLER_H
