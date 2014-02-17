#ifndef SPECTRUMWIDGETDATASOURCE_H
#define SPECTRUMWIDGETDATASOURCE_H

#include <QObject>

#include "Interfaces/IRpcListener.h"
#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RPCClient.h"

class SpectrumWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT
private:
	RPCClient* m_rpcController;

public:
	explicit SpectrumWidgetDataSource(RPCClient*, QObject *parent = 0);

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);
};

#endif // SPECTRUMWIDGETDATASOURCE_H
