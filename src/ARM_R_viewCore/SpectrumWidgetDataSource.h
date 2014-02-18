#ifndef SPECTRUMWIDGETDATASOURCE_H
#define SPECTRUMWIDGETDATASOURCE_H

#include <QObject>
#include <QVector>
#include <QPointF>

#include "Interfaces/IRpcListener.h"
#include "IGraphicWidget.h"
#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RPCClient.h"

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

	//QList< QList<QPointF> > m_pointsList;
	//QList<QPointF> m_spectrumPeakHoldList;

public:
	explicit SpectrumWidgetDataSource(IGraphicWidget*, QObject *parent = 0);

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);

	void setPanorama(bool enabled, double start = 0.0f, double end = 0.0f);

private:
	void dataProccess(QVector<QPointF>& vecFFT, bool isComplex);
	void setBandwidth(double bandwidth);
	int findIndex(qreal startx);
};

#endif // SPECTRUMWIDGETDATASOURCE_H
