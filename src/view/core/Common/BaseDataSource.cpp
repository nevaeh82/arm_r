#include "BaseDataSource.h"

BaseDataSource::BaseDataSource(QObject *parent) :
	QObject(parent)
{
}

BaseDataSource::~BaseDataSource()
{
	log_debug("<<<<<<<<<<<<<<<");
}

void BaseDataSource::setLocationController(LocationSetupWidgetController *controller)
{
	m_locationController = controller;

	connect(this, SIGNAL(onDrawComplete()), m_locationController, SLOT(slotPlotDrawComplete()));
}

void BaseDataSource::onDataReceived(const QString &method, const QVariant& arg)
{
	int t = m_receiversList.size();
	foreach(IDataSourceListener* receiver, m_receiversList){
		if(receiver) {
			receiver->onDataArrived(method, arg);
		}
	}
}
