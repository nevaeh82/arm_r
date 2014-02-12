#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QMap>
#include <QVariant>
#include <QString>

#include "Settings/SettingsNode.h"
#include "Interfaces/IDbChangedListener.h"
#include "BaseSubject.h"

class IDbManager: public BaseSubject<IDbChangedListener>
{
public:
	virtual ~IDbManager(){}

	virtual SettingsNode getSettingsNode(const QString& objectName) = 0;
	virtual bool updateProperty(const Property& property) = 0;
};

#endif // IDBMANAGER_H
