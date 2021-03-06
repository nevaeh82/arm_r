#ifndef PRM300CONTROLWIDGETCONTROLLER_H
#define PRM300CONTROLWIDGETCONTROLLER_H

#include <QObject>
#include <QMessageBox>

#include "Prm300ControlWidget.h"
#include "Interfaces/IController.h"
#include "ServiceHandler/ServiceHandler.h"
#include "Tabs/RPC/RpcPrmClient.h"

class Prm300ControlWidgetController :
		public QObject, public IController<Prm300ControlWidget>, public IRpcListener
{
	Q_OBJECT

private:
	Prm300ControlWidget* m_view;

	RpcPrmClient* m_rpcPrmClient;

	IDbManager* m_dbManager;

	QString m_stationName;

public:
	explicit Prm300ControlWidgetController(const QString& name, QObject *parent = 0);
	virtual ~Prm300ControlWidgetController();

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	void appendView(Prm300ControlWidget *view);

	void setRpcPrmClient(RpcPrmClient* client);

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
