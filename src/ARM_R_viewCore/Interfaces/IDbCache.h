#ifndef IDBCACHE_H
#define IDBCACHE_H

#include <QMap>
#include <QString>
#include <QVariant>

#include "Interfaces/IDbChangedListener.h"
//#include "Interfaces/ILandClientSettingsRpcController.h"

class IDbCache
{
public:
	virtual ~IDbCache(){}

	//virtual void setSettingsRpcController(ILandClientSettingsRpcController* rpcController) = 0;

	virtual void registerDbChangedListener(IDbChangedListener* listener) = 0;

	virtual void getCommonSettings() = 0;
	virtual void cleanSettings() = 0;

	virtual void updateProperty(const Property& property) = 0;

	virtual QString getIpProperty(const QString& objectName) = 0;
};

#endif // IDBCACHE_H
