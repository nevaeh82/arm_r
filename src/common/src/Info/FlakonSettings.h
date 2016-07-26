#ifndef FLAKONSETTINGS_H
#define FLAKONSETTINGS_H

#include <QDataStream>
#include <QList>

#include "BaseSettings.h"

typedef struct FlakonSettings : public BaseSetting
{
	FlakonSettings() : BaseSetting()
	{
		type = TypeFlakon;
	}

	BaseSettingsType& getType()
	{
		return type;
	}

    unsigned int id;
    unsigned int zone;
    unsigned int typeRds;

}FlakonSettings;

inline QDataStream& operator<<(QDataStream& out, const FlakonSettings &object)
{
	return out << object.name << object.host << object.port;
}

inline QDataStream& operator>>(QDataStream& in, FlakonSettings &object)
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

inline QByteArray FlakonSettingsToByteArray(const FlakonSettings& obj)
{
	QByteArray outBA;
	QDataStream stream(&outBA, QIODevice::Append);
	stream << obj;

	return outBA;
}

inline FlakonSettings FlakonSettingsFromByteArray(QByteArray& inBA)
{
	FlakonSettings obj;

	QDataStream stream(inBA);
	stream >> obj;

	return obj;
}

#endif // FLAKONSETTINGS_H
