#ifndef ATLANTSETTINGS_H
#define ATLANTSETTINGS_H

#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct AtlantSettings : public BaseSetting
{
	AtlantSettings() : BaseSetting()
	{
		type = TypeAtlant;
	}

}AtlantSettings;

inline QDataStream& operator<<(QDataStream& out, const AtlantSettings &object)
{
	return out << object.name << object.host << object.port;
}

inline QDataStream& operator>>(QDataStream& in, AtlantSettings &object)
{
	quint32 type;
	QString name;
	QString host;
	quint32 port;

	in >> type >> name >> host >> port;
	object.name = name;
	object.host = host;
	object.port = port;

	return in;
}

inline QByteArray AtlantSettingsToByteArray(const AtlantSettings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline AtlantSettings AtlantSettingsSettingsFromByteArray(QByteArray& inBA)
{
	AtlantSettings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // ATLANTSETTINGS_H
