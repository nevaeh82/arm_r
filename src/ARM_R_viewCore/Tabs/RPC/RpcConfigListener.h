#ifndef RPCCONFIGLISTENER_H
#define RPCCONFIGLISTENER_H

#include <QObject>
#include <QVariant>
#include <QDataStream>
#include <QList>

#include <Interfaces/IRpcListener.h>

#include "Rpc/RpcDefines.h"
#include "Info/AtlantConfiguraton.h"
#include "Info/StationConfiguration.h"

class RpcConfigListener : public QObject, IRpcListener
{
	Q_OBJECT
public:
	explicit RpcConfigListener(QObject* parent = NULL);

	// IRpcListener interface
public:
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

signals:
	void onStationListReceived(const QByteArray& data);
	void onAtlantConfigReceived(const QByteArray& data);
};

#endif // RPCCONFIGLISTENER_H
