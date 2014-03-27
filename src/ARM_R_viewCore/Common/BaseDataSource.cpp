#include "BaseDataSource.h"

BaseDataSource::BaseDataSource(QObject *parent) :
	QObject(parent)
{
}

void BaseDataSource::onDataReceived(const QString &method, const QVariant& arg)
{
	foreach(IDataSourceListener* receiver, m_receiversList){
		receiver->onDataArrived(method, arg);
	}
}
