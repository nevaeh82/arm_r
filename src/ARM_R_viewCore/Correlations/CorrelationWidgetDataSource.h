#ifndef CORRELATIONWIDGETDATASOURCE_H
#define CORRELATIONWIDGETDATASOURCE_H

#include <QObject>

#include "Interfaces/IRpcListener.h"
#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RPCClient.h"

class CorrelationWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT
private:
	IGraphicWidget* m_correlationWidget;

	ICommonComponents*	m_commonCorrelations;
	ITabManager*	m_tabManager;
	int	m_id;

	qreal	m_startxCor;
	bool	m_needSetup;

	//QMap<int, float*>	m_mapPeaksCorrelation;
	//QMap<int, float*>	m_mapSpectrumCorelation;
	//QMap<int, double>	m_mapBandwidthCorelation;

	float*	m_mapPeaksCorrelation;
	float*	m_mapSpectrumCorelation;
	double	m_mapBandwidthCorelation;

public:
	explicit CorrelationWidgetDataSource(IGraphicWidget*, ITabManager *tabManager, int id, QObject *parent);

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);

private:
	void setCorData(quint32 point2, QByteArray &vecFFTBA, bool isComplex);
};

#endif // CORRELATIONWIDGETDATASOURCE_H
