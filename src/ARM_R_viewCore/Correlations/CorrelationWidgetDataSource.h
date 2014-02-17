#ifndef CORRELATIONWIDGETDATASOURCE_H
#define CORRELATIONWIDGETDATASOURCE_H

#include <QObject>

#include "Interfaces/IRpcListener.h"
#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RPCClient.h"

class CorrelationWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT
private:
	RPCClient* m_rpcController;

public:
	explicit CorrelationWidgetDataSource(RPCClient *rpcClient, QObject *parent = 0);

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);
};

#endif // CORRELATIONWIDGETDATASOURCE_H
