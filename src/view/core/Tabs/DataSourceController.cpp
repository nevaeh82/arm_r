#include "TabManager.h"

#include "DataSourceController.h"

DataSourceController::DataSourceController(TabManager *tabManager, IDbManager* dbManager,
										   RpcFlakonClientWrapper *rpcClient, PanoramaFreqControl *panoramaFreqControl,
										   QObject* parent) :
	QObject(parent),
	m_tabManager(tabManager),
	m_dbManager(dbManager),
	m_rpcFlakonClient(rpcClient),
	m_panoramaFreqControl(panoramaFreqControl),
	m_controlPanelController(NULL)
{

}

DataSourceController::~DataSourceController()
{

}

void DataSourceController::deregisterAll()
{
	foreach (SpectrumDsView ds, m_spectrumDataSourceList) {
		m_rpcFlakonClient->deregisterReceiver( ds.ds );
	}

	foreach( CorrelationWidgetDataSource *ds, m_correlationDataSourcesList ) {
		m_rpcFlakonClient->deregisterReceiver( ds );
	}

	foreach (AnalysisWidgetDataSource* ds, m_analysisDataSourcesList) {
		m_rpcFlakonClient->deregisterReceiver( ds );
	}

	foreach (QThread* thread, m_threadSrcList) {
		thread->exit();
		//delete thread;
	}

	m_spectrumDataSourceList.clear();
	m_analysisDataSourcesList.clear();
	m_correlationDataSourcesList.clear();

}

void DataSourceController::setLocationController(LocationSetupWidgetController *controller)
{
	m_locationController = controller;
}

void DataSourceController::setControlPanelController(ControlPanelController *controller)
{
	m_controlPanelController = controller;
}

void DataSourceController::enablePanorama(bool on, int start, int end)
{
	foreach (SpectrumDsView ds, m_spectrumDataSourceList) {
		ds.ds->setPanorama( on, start, end );
	}
}

void DataSourceController::initSpectrumDataSource(QMap<QString, ITabWidget* > tabWidgetsMap )
{
	foreach (QString name, tabWidgetsMap.keys()) {

		ISpectrumWidget* spectrumWidget = tabWidgetsMap.value(name)->getSpectrumWidget();

		SpectrumWidgetDataSource* spectrumDataSource = new SpectrumWidgetDataSource(spectrumWidget);
		spectrumDataSource->setDBManager(m_dbManager);
		spectrumDataSource->setName(name);
		spectrumDataSource->setPanoramaFreqControl(m_panoramaFreqControl);
		spectrumDataSource->setId(spectrumWidget->getId());
		spectrumDataSource->registerReceiver(spectrumWidget);

		spectrumDataSource->setTabManager((ITabManager*)m_tabManager);
		spectrumDataSource->setLocationController(m_locationController);
		spectrumDataSource->setControlPanelController(m_controlPanelController);


		//Maybe it will better perfomance
		QThread* thread = new QThread;
		connect(spectrumDataSource, SIGNAL(destroyed()), thread, SLOT(terminate()));
		connect(thread, SIGNAL(finished()), spectrumDataSource, SLOT(deleteLater()), Qt::DirectConnection);
		connect(thread, SIGNAL(destroyed()), spectrumDataSource, SLOT(deleteLater()));

		spectrumDataSource->moveToThread(thread);
		thread->start();

		m_threadSrcList.append(thread);

		//---------------------------------------

		if (m_rpcFlakonClient != NULL) {
			m_rpcFlakonClient->registerReceiver( spectrumDataSource );
		}

		SpectrumDsView dsView;
		dsView.wgt = spectrumWidget;
		dsView.ds = spectrumDataSource;
		m_spectrumDataSourceList.append( dsView );

	}
}

void DataSourceController::initCorrelationDataSource( 	ICorrelationControllersContainer*  correlationControllers,
														ICorrelationControllersContainer*  extCorrelationControllers )
{
	int id1 = 0;
	int id2 = 0;
	int listPos = 0;
	int inc = 1;

	int chCnt = m_tabManager->getChannelCount();

	QList<int> ids;
	for(int k = 0; k < chCnt; k++) {
		ids.append( k );
	}

	for(int i = 0; i < correlationControllers->count(); i++) {

		if((listPos+1) > (chCnt-1)) {
			listPos=inc;
			inc+=1;
		}

		id1 = ids.at( inc-1 );
		id2 = ids.at( listPos+1 );

		listPos++;

		CorrelationWidgetDataSource* correlationDataSource = new CorrelationWidgetDataSource(m_tabManager, id1, id2, 0);
		CorrelationWidgetDataSource* correlationDataSourceExt = new CorrelationWidgetDataSource(m_tabManager, id1, id2, 0);

		correlationDataSource->registerReceiver(correlationControllers->get(i));
		correlationDataSource->setLocationController(m_locationController);
		correlationDataSource->setControlPanelController(m_controlPanelController);

		correlationDataSource->setCorrGraphWidget(correlationControllers->get(i));

		if( extCorrelationControllers && extCorrelationControllers->get(i) ) {
			correlationDataSourceExt->registerReceiver(extCorrelationControllers->get(i));
			correlationDataSourceExt->setLocationController(m_locationController);
			correlationDataSourceExt->setControlPanelController(NULL);
			correlationDataSourceExt->setCorrGraphWidget(extCorrelationControllers->get(i));
		}


		//Maybe it will better perfomance
		QThread* thread = new QThread;
		connect(correlationDataSource, SIGNAL(destroyed()), thread, SLOT(terminate()));
		connect(thread, SIGNAL(finished()), correlationDataSource, SLOT(deleteLater()), Qt::DirectConnection);
		connect(thread, SIGNAL(destroyed()), correlationDataSource, SLOT(deleteLater()));

		correlationDataSource->moveToThread(thread);
		thread->start();

		m_threadSrcList.append(thread);

		//Maybe it will better perfomance
		QThread* threadExt = new QThread;
		connect(correlationDataSourceExt, SIGNAL(destroyed()), threadExt, SLOT(terminate()));
		connect(threadExt, SIGNAL(finished()), correlationDataSource, SLOT(deleteLater()), Qt::DirectConnection);
		connect(threadExt, SIGNAL(destroyed()), correlationDataSource, SLOT(deleteLater()));

		correlationDataSourceExt->moveToThread(threadExt);
		threadExt->start();

		m_threadSrcList.append(threadExt);

		//---------------------------------------

		if( m_rpcFlakonClient != NULL ) {
			m_rpcFlakonClient->registerReceiver( correlationDataSource );
			m_rpcFlakonClient->registerReceiver( correlationDataSourceExt );
		}

		m_correlationDataSourcesList.append(correlationDataSource);
		m_correlationDataSourcesList.append(correlationDataSourceExt);
	}
}

void DataSourceController::initAnalysisDataSource(QMap<int, IAnalysisWidget*> analysisControllers)
{
	for(int i = 0; i < analysisControllers.count(); i++) {
		IAnalysisWidget* analysisWidget = analysisControllers.value(i);

		AnalysisWidgetDataSource* analysisDataSource = new AnalysisWidgetDataSource(analysisWidget, m_tabManager, analysisWidget->getType(), 0);
		analysisDataSource->registerReceiver(analysisWidget);
		analysisDataSource->setLocationController(m_locationController);
		analysisDataSource->setControlPanelController(NULL);

		//Maybe it will improve perfomance
		QThread* thread = new QThread;
		connect(analysisDataSource, SIGNAL(destroyed()), thread, SLOT(terminate()));
		connect(thread, SIGNAL(finished()), analysisDataSource, SLOT(deleteLater()), Qt::DirectConnection);
		connect(thread, SIGNAL(destroyed()), analysisDataSource, SLOT(deleteLater()));

		analysisDataSource->moveToThread(thread);
		thread->start();

		m_threadSrcList.append(thread);

		//---------------------------------------

		if( m_rpcFlakonClient != NULL ) {
			m_rpcFlakonClient->registerReceiver( analysisDataSource );
		}

		m_analysisDataSourcesList.append(analysisDataSource);
	}
}

