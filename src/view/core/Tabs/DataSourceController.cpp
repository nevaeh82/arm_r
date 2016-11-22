#include "TabManager.h"

#include "DataSourceController.h"

DataSourceController::DataSourceController(TabManager *tabManager, IDbManager* dbManager,
										   RpcFlakonClient *rpcClient, PanoramaFreqControl *panoramaFreqControl,
										   QObject* parent) :
	QObject(parent),
	m_tabManager(tabManager),
	m_dbManager(dbManager),
	m_rpcFlakonClient(rpcClient),
	m_panoramaFreqControl(panoramaFreqControl)
{

}

DataSourceController::~DataSourceController()
{

}

void DataSourceController::deregisterAll()
{
	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		if (m_rpcFlakonClient != NULL) {
			m_rpcFlakonClient->deregisterReceiver(correlationWidgetDataSource);
		}
	}

	foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
		//correlationWidgetDataSource->deregisterCorrelationReceiver( (ICorrelationListener*) m_spectrumWidget );

		correlationWidgetDataSource->deleteLater();
	}

	foreach (AnalysisWidgetDataSource* analysisDataSource, m_analysisDataSourcesList){
		analysisDataSource->deleteLater();
	}

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
		//thread->deleteLater();
	}

}

void DataSourceController::initSpectrumDataSource(QMap<QString, ITabWidget* > tabWidgetsMap )
{
	foreach (QString name, tabWidgetsMap.keys()) {

		ISpectrumWidget* spectrumWidget = tabWidgetsMap.value(name)->getSpectrumWidget();

		SpectrumWidgetDataSource* spectrumDataSource = new SpectrumWidgetDataSource(spectrumWidget, this);
		spectrumDataSource->setDBManager(m_dbManager);
		spectrumDataSource->setName(name);
		spectrumDataSource->setPanoramaFreqControl(m_panoramaFreqControl);
		spectrumDataSource->setId(spectrumWidget->getId());
		spectrumDataSource->registerReceiver(spectrumWidget);

		spectrumDataSource->setTabManager((ITabManager*)m_tabManager);

		//For correlation state messages
//		foreach (CorrelationWidgetDataSource* correlationWidgetDataSource, m_correlationDataSourcesList){
//			//correlationWidgetDataSource->registerCorrelationReceiver(dynamic_cast<ICorrelationListener*>(spectrumWidget));
//		}

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
		correlationDataSource->registerReceiver(correlationControllers->get(i));

		if( extCorrelationControllers && extCorrelationControllers->get(i) ) {
			correlationDataSource->registerReceiver(extCorrelationControllers->get(i));
		}


		//Maybe it will better perfomance
		QThread* thread = new QThread;
		connect(correlationDataSource, SIGNAL(destroyed()), thread, SLOT(terminate()));
		connect(thread, SIGNAL(finished()), correlationDataSource, SLOT(deleteLater()), Qt::DirectConnection);
		connect(thread, SIGNAL(destroyed()), correlationDataSource, SLOT(deleteLater()));

		correlationDataSource->moveToThread(thread);
		thread->start();

		m_threadSrcList.append(thread);

		//---------------------------------------

		if( m_rpcFlakonClient != NULL ) {
			m_rpcFlakonClient->registerReceiver( correlationDataSource );
		}

		m_correlationDataSourcesList.append(correlationDataSource);
	}
}

void DataSourceController::initAnalysisDataSource(QMap<int, IAnalysisWidget*> analysisControllers)
{
	for(int i = 0; i < analysisControllers.count(); i++) {
		IAnalysisWidget* analysisWidget = analysisControllers.value(i);

		AnalysisWidgetDataSource* analysisDataSource = new AnalysisWidgetDataSource(analysisWidget, m_tabManager, analysisWidget->getType(), 0);
		analysisDataSource->registerReceiver(analysisWidget);


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

