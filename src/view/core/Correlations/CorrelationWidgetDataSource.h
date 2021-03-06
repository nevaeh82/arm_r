#ifndef CORRELATIONWIDGETDATASOURCE_H
#define CORRELATIONWIDGETDATASOURCE_H

#include <QObject>

#include "Interfaces/IRpcListener.h"
#include "Interfaces/ICorrelationControllersContainer.h"
#include "Interfaces/ITabManager.h"
#include "Interfaces/ICorrelationListener.h"
#include "IGraphicWidget.h"

#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RpcPrmClient.h"

class CorrelationWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT

private:
	IGraphicWidget* m_correlationWidget;

	//ICorrelationControllersContainer*	m_commonCorrelations;
	ITabManager*	m_tabManager;
	int	m_id;

	qreal	m_startxCor;
	bool	m_needSetup;

	float*	m_mapPeaksCorrelation;
	float*	m_mapSpectrumCorelation;
	double	m_mapBandwidthCorelation;

	QTimer*	correlationStateTimer;
	QList<ICorrelationListener*> m_correlationListeners;

public:
	explicit CorrelationWidgetDataSource(IGraphicWidget*, ITabManager *tabManager, int id, QObject *parent);

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);
	void onCorrelationStateChanged(const bool isEnabled);

	void registerCorrelationReceiver(ICorrelationListener* obj);
	void deregisterCorrelationReceiver(ICorrelationListener* obj);

private:
	void setCorData(quint32 point1, quint32 point2, const QVector<QPointF>& points, bool);

signals:
	void onMethodCalledSignal(QString, QVariant);

private slots:
	void correlationTimerOff();

	void onMethodCalledSlot(QString, QVariant);
};

#endif // CORRELATIONWIDGETDATASOURCE_H
