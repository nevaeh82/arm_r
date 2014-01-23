#ifndef ISETTINGSMANAGER_H
#define ISETTINGSMANAGER_H

#include "ICommonSettingsManager.h"

class ISettingsManager : public ICommonSettingsManager
{
public:
	virtual ~ISettingsManager(){}

	virtual void setIniFile(const QString&)			= 0;

	virtual QString getCommandServiceRpcPort()		= 0;
	virtual QString getFPGAServiceRpcPort()			= 0;
	virtual QString getTelemetryServiceRpcPort()	= 0;
	virtual QString getAgentServiceRpcPort()		= 0;
	virtual QString getUpdateServiceRpcPort()		= 0;

	virtual QString getCommandServiceComPort()		= 0;
	virtual QString getTelemetryServiceComPort()	= 0;

	virtual QString getRestartTimeout()				= 0;
	virtual QString getTempFolderName()				= 0;

	virtual QString getDigitalCameraModel()			= 0;

	virtual void setDigitalCameraModel(const QString& value) = 0;

};

#endif // ISETTINGSMANAGER_H
