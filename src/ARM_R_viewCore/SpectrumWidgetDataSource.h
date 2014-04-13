#ifndef SPECTRUMWIDGETDATASOURCE_H
#define SPECTRUMWIDGETDATASOURCE_H

#include <QObject>
#include <QVector>
#include <QPointF>

#include "Interfaces/IRpcListener.h"
#include "IGraphicWidget.h"
#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RpcPrmClient.h"

class SpectrumWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT

friend class GraphicsDataTest;

private:
	IGraphicWidget* m_spectrumWidget;

	int		m_pointCount;
	int		m_pointCountWhole;
	qreal	m_startx;
	qreal	m_startx_cor;
	bool	m_needSetup;
	bool	m_isPanoramaStart;
	bool	m_needSetupSpectrum;
	double	m_bandwidthSingleSample;

	float*	m_spectrum;
	float*	m_spectrumPeakHold;

	double	m_bandwidth;

	QList<qreal>	m_listStartx;

public:
	explicit SpectrumWidgetDataSource(IGraphicWidget*, QObject *parent = 0);
	virtual ~SpectrumWidgetDataSource();

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);

	void setPanorama(bool enabled, double start = 0.0f, double end = 0.0f);
	bool isPanoramaEnabled();

signals:
	void onMethodCalledSignal(QString, QVariant);

private slots:
	void onMethodCalledSlot(QString, QVariant);

private:
	void dataProccess(QVector<QPointF>& vecFFT, bool);
	void setBandwidth(double bandwidth);
	int findIndex(qreal startx);
};

#endif // SPECTRUMWIDGETDATASOURCE_H
