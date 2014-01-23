#ifndef ICOMMONSETTINGSPROVIDER_H
#define ICOMMONSETTINGSPROVIDER_H

#include "ICommonSettingsManager.h"

#include <QString>

class ICommonSettingsProvider
{
public:
	virtual ~ICommonSettingsProvider(){}
	virtual void setCommonSettingsProvider(ICommonSettingsManager*) = 0;
	virtual ICommonSettingsManager* getCommonSettingsProvider() = 0;
};

#endif // ICOMMONSETTINGSPROVIDER_H
