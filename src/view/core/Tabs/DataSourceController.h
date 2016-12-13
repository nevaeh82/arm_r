#pragma once

#include <QObject>

#include "SpectrumWidgetDataSource.h"
#include "Correlations/CorrelationWidgetDataSource.h"
#include "Analysis/AnalysisWidgetDataSource.h"

#include "Interfaces/ITabWidget.h"
#include "SpectrumWidget.h"
#include "Controls/PanoramaFreqControl.h"

#include "LocationSetupWidgetController.h"

class TabManager;

class DataSourceController : public QObject
{

	Q_OBJECT

	struct SpectrumDsView
	{
		ISpectrumWidget* wgt;
		SpectrumWidgetDataSource* ds;
	};

public:
	DataSourceController(TabManager *tabManager, IDbManager *dbManager, RpcFlakonClientWrapper *rpcClient, PanoramaFreqControl *panoramaFreqControl, QObject* parent);
	~DataSourceController();

	void initSpectrumDataSource(QMap<QString, ITabWidget *> tabWidgetsMap);
	void initCorrelationDataSource(ICorrelationControllersContainer *correlationControllers,
								   ICorrelationControllersContainer *extCorrelationControllers);
	void initAnalysisDataSource(QMap<int, IAnalysisWidget *> analysisControllers);

	void deregisterAll();

	void setLocationController(LocationSetupWidgetController* controller);

private:
	QList<SpectrumDsView> m_spectrumDataSourceList;
	QList<CorrelationWidgetDataSource*> m_correlationDataSourcesList;
	QList<AnalysisWidgetDataSource*> m_analysisDataSourcesList;


	TabManager* m_tabManager;
	IDbManager* m_dbManager;
	RpcFlakonClientWrapper* m_rpcFlakonClient;
	PanoramaFreqControl* m_panoramaFreqControl;

	QList<QThread*> m_threadSrcList;

	LocationSetupWidgetController* m_locationController;

public slots:
	void enablePanorama(bool on, int start, int end);
};
