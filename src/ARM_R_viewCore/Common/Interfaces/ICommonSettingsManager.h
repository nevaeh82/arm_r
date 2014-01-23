#ifndef ICOMMONSETTINGSMANAGER_H
#define ICOMMONSETTINGSMANAGER_H

class QString;

class ICommonSettingsManager
{
public:
	virtual ~ICommonSettingsManager(){}

	virtual QString getPacketWaitTimeout()	= 0;
	virtual QString getIsLogComPortData()	= 0;
};
#endif // ICOMMONSETTINGSMANAGER_H
