#ifndef BASESETTINGS_H
#define BASESETTINGS_H

#include <QString>

enum BaseSettingsType
{
	TypeRetranslator = -1,
	TypeFlakon = 1,
	TypePRM300 = 2,
	TypeAtlant = 3,
	TypeRDS = 6,
	typeUnknown = 0
};

typedef struct BaseSetting
{
	BaseSetting()
	{
		name = QString();
		host = "127.0.0.1";
		port = 1024;
	}

	QString name;
	QString host;
	quint32 port;
	quint32 reconnectInterval;

protected:
	BaseSettingsType type;

}BaseSetting;

#endif // BASESETTINGS_H
