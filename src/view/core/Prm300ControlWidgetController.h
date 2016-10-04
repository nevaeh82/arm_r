#ifndef PRM300CONTROLWIDGETCONTROLLER_H
#define PRM300CONTROLWIDGETCONTROLLER_H

#include <QObject>
#include <QMessageBox>
#include <QDataStream>

#include "Prm300ControlWidget.h"
#include "Interfaces/IController.h"
#include "ServiceHandler/ServiceHandler.h"
#include "Tabs/RPC/RpcPrmClient.h"
#include "Tabs/RPC/RpcFlakonClientWrapper.h"

class Prm300ControlWidgetController :
		public QObject, public IController<Prm300ControlWidget>, public IRpcListener
{
	Q_OBJECT

private:
	Prm300ControlWidget* m_view;

	RpcPrmClient* m_rpcPrmClient;
    RpcFlakonClientWrapper* m_rpcFlakonClient;

	IDbManager* m_dbManager;

	QString m_stationName;
    uint m_stationId;
    uint m_platformId; // TODO receive it!!!!
	uint m_channelId;

public:
	explicit Prm300ControlWidgetController(const QString& name, QObject *parent = 0);
	virtual ~Prm300ControlWidgetController();

	virtual void onMethodCalled(const QString& method, const QVariant& argument);

	void appendView(Prm300ControlWidget *view);

	void setRpcPrmClient(RpcPrmClient* client);
    void setRpcFlakonClient(RpcFlakonClientWrapper* client);

	virtual void setDbManager(IDbManager*);

	void setName(const QString& name);
    void setId(const uint id);
	void setChannel(const uint id);
	void setPlatform( const uint id );

	void setChannelState(bool);

private:
	void init();

signals:

private slots:
	void slotSetAtt1();
	void slotSetAtt2(int value);
	void slotSetFilter(int index);

    void slotEnableReceiver(bool);
};

#endif // PRM300CONTROLWIDGETCONTROLLER_H
