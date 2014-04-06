#ifndef ATLANTCONFIGURATON_H
#define ATLANTCONFIGURATON_H

#include <QtGlobal>

struct AtlantConfiguration
{
	AtlantConfiguration()
	{
		hostByRpc		= "127.0.0.1";
		portByRpc		= 0;
	}

	AtlantConfiguration(const AtlantConfiguration& object)
	{
		hostByRpc		= object.hostByRpc;
		portByRpc		= object.portByRpc;
	}

	QString		hostByRpc;
	quint32		portByRpc;
};

inline QDataStream& operator<<(QDataStream& out, const AtlantConfiguration& object)
{
	return out << object.hostByRpc
			   << object.portByRpc;
}

inline QDataStream& operator>>(QDataStream& in, AtlantConfiguration& object)
{
	in >> object.hostByRpc
	   >> object.portByRpc;

	return in;
}

#endif // ATLANTCONFIGURATON_H
