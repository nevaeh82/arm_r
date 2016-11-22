#pragma once

#include <QObject>

#include "SpectrumWidgetDataSource.h"
#include "Correlations/CorrelationWidgetDataSource.h"
#include "Analysis/AnalysisWidgetDataSource.h"

#include "Interfaces/ITabWidget.h"
#include "SpectrumWidget.h"
#include "Controls/PanoramaFreqControl.h"

class TabManager;

class DataSourceController : public QObject
{

	struct SpectrumDsView
	{
		ISpectrumWidget* wgt;
		SpectrumWidgetDataSource* ds;
	};

public:
	DataSourceController(TabManager *tabManager, IDbManager *dbManager, RpcFlakonClient *rpcClient, PanoramaFreqControl *panoramaFreqControl, QObject* parent);
	~DataSourceController();

	void initSpectrumDataSource(QMap<QString, ITabWidget *> tabWidgetsMap);
	void initCorrelationDataSource(ICorrelationControllersContainer *correlationControllers,
								   ICorrelationControllersContainer *extCorrelationControllers);
	void initAnalysisDataSource(QMap<int, IAnalysisWidget *> analysisControllers);

	void deregisterAll();

private:
	QList<SpectrumDsView> m_spectrumDataSourceList;
	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;
	QList<AnalysisWidgetDataSource*> m_analysisDataSourcesList;


	TabManager* m_tabManager;
	IDbManager* m_dbManager;
	RpcFlakonClient* m_rpcFlakonClient;
	PanoramaFreqControl* m_panoramaFreqControl;

	QList<QThread*> m_threadSrcList;
};
