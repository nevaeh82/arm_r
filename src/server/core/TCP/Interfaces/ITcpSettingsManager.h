#ifndef ITCPSETTINGSMANAGER_H
#define ITCPSETTINGSMANAGER_H

#include <QString>
#include <QByteArray>
#include <QMap>

class ITcpSettingsManager
{

public:
	virtual ~ITcpSettingsManager(){}

	virtual void setIniFile(const QString& filePath)	= 0;

	virtual QString getFlakonHost()		= 0;
	virtual QString getFlakonPort()		= 0;

	virtual QMap<QString, int> getAllInfo()		= 0;
};

#endif // ITCPSETTINGSMANAGER_H
