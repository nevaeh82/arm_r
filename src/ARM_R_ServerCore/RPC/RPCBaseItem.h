#ifndef RPCBASEITEM_H
#define RPCBASEITEM_H

#include <QString>
#include <QVariant>

//enum RPCBaseItemType
//{
//	TypeObject,
//	TypeProperty,
//	TypeUnknown
//};

typedef struct RPCBaseItem
{
	RPCBaseItem()
	{
		id = 0;
		name = QString();
	}

	int id;
	QString name;

//protected:
//	BaseItemType type;

}RPCBaseItem;

#endif // RPCBASEITEM_H
