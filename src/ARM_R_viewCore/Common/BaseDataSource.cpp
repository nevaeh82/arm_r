#include "BaseDataSource.h"

BaseDataSource::BaseDataSource(QObject *parent) :
	QObject(parent)
{
}

void BaseDataSource::registerListener(IDataSourceListener* listener)
{
	m_listeners.append(listener);
}

void BaseDataSource::deregisterListener(IDataSourceListener* listener)
{
	int index = m_listeners.indexOf(listener);

	if (index < 0){
		return;
	}

	m_listeners.removeAt(index);
}

void BaseDataSource::onDataReceived(const QString &method, const QVariant& arg, const QList< QVector<QPointF> >& argListVector)
{
	foreach(IDataSourceListener* listener, m_listeners){
		listener->onDataArrived(method, arg, argListVector);
	}
}
