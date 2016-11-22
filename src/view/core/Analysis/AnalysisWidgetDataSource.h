#pragma once

#include <QObject>

#include "Interfaces/IRpcListener.h"
#include "Interfaces/ITabManager.h"
#include "IGraphicWidget.h"

#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RpcPrmClient.h"

#include "RDSExchange.h"

class AnalysisWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT

private:
	IGraphicWidget* m_correlationWidget;

	//ICorrelationControllersContainer*	m_commonCorrelations;
	ITabManager*	m_tabManager;
	int m_type;

	qreal	m_startxCor;
	bool	m_needSetup;

	float*	m_mapPeaksCorrelation;
	float*	m_mapSpectrumCorelation;
	double	m_mapBandwidthCorelation;

	QTimer*	correlationStateTimer;

public:
	explicit AnalysisWidgetDataSource(IGraphicWidget*, ITabManager *tabManager, int id, QObject *parent);
	virtual ~AnalysisWidgetDataSource();

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);

private:
	void setCorData(int type, const QVector<QPointF>& points);

signals:
	void onMethodCalledSignal(QString, QVariant);

private slots:
	void onMethodCalledSlot(QString, QVariant);
};
