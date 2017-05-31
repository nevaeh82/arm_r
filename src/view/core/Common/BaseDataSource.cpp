#include "BaseDataSource.h"

BaseDataSource::BaseDataSource(QObject *parent) :
	QObject(parent),
	m_locationController(NULL),
	m_panelController(NULL)
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

void BaseDataSource::setControlPanelController(ControlPanelController *controller)
{
	m_panelController = controller;
    connect(this, SIGNAL(dopplerStatus(QString,double)),
            m_panelController, SLOT(slotDopplerStatus(QString,double)));
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
