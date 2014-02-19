#ifndef IPLAYERDATASOURCELISTENER_H
#define IPLAYERDATASOURCELISTENER_H

class QString;
class QVariant;
class QPointF;

#include <QVector>
#include <QList>

class IDataSourceListener
{
	public:
		virtual ~IDataSourceListener(){}

		virtual void onDataArrived(const QString& method, const QVariant& arg) = 0;
};

#endif // IPLAYERDATASOURCELISTENER_H
