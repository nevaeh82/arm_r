#ifndef RPCMESSAGESTRUCT_H
#define RPCMESSAGESTRUCT_H

#include <QString>
#include <QByteArray>
#include <QDataStream>

typedef struct RpcMessageStruct
{
	QString name;
	QByteArray data;
}RpcMessageStruct;

inline QDataStream& operator<<(QDataStream& out, const RpcMessageStruct &object)
{
	return out << (QString)object.name << (QByteArray)object.data;
}

inline QDataStream& operator>>(QDataStream& in, RpcMessageStruct &object)
{
	QString name;
	QByteArray data;

	in >> object.name >> object.data;

	return in;
}

#endif // RPCMESSAGESTRUCT_H
