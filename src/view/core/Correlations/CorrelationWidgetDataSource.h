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

#include "RDSExchange.h"

class CorrelationWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT

private:

	//ICorrelationControllersContainer*	m_commonCorrelations;
	ITabManager*	m_tabManager;
	int	m_id1;
	int	m_id2;

	qreal	m_startxCor;
	bool	m_needSetup;

	float*	m_mapPeaksCorrelation;
	float*	m_mapSpectrumCorelation;
	double	m_mapBandwidthCorelation;

	QTimer*	correlationStateTimer;
	QList<ICorrelationListener*> m_correlationListeners;

	ICorrelationWidget *m_corrGraphWgt;

public:
	explicit CorrelationWidgetDataSource(ITabManager *tabManager, int id1, int id2, QObject *parent);
	virtual ~CorrelationWidgetDataSource();

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);
	void onCorrelationStateChanged(const bool isEnabled);

	void registerCorrelationReceiver(ICorrelationListener* obj);
	void deregisterCorrelationReceiver(ICorrelationListener* obj);
	void setCorrGraphWidget(ICorrelationWidget* wgt);

private:
	void setCorData(quint32 point1, quint32 point2, const QVector<QPointF>& points, float veracity);

	void getIds(const int pos, const int chCnt, int &id1, int &id2);
	bool findPlot(const int channelCount, const::google::protobuf::RepeatedPtrField<::RdsProtobuf::ServerMessage_OneShotData_LocationData_Plot> &plots, RdsProtobuf::ServerMessage_OneShotData_LocationData_Plot &outPlot);
	bool findConvolution(const::google::protobuf::RepeatedPtrField<RdsProtobuf::Convolution> &convList, RdsProtobuf::Convolution &convolution);
signals:
	void onMethodCalledSignal(QString, QVariant);

private slots:
	void correlationTimerOff();

	void onMethodCalledSlot(QString, QVariant);
};

#endif // CORRELATIONWIDGETDATASOURCE_H
