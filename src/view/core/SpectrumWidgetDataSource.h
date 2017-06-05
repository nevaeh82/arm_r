#ifndef SPECTRUMWIDGETDATASOURCE_H
#define SPECTRUMWIDGETDATASOURCE_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QMutex>

#include "Interfaces/IRpcListener.h"
#include "IGraphicWidget.h"
#include "Common/BaseDataSource.h"
#include "Tabs/RPC/RpcPrmClient.h"
#include "Interfaces/ITabManager.h"
#include "Db/DbManager.h"
#include "Tabs/Controls/PanoramaFreqControl.h"

#include "RDSExchange.h"

class SpectrumWidgetDataSourceTest;

class SpectrumWidgetDataSource : public BaseDataSource, public IRpcListener
{
	Q_OBJECT

	friend class SpectrumWidgetDataSourceTest;

private:
	IGraphicWidget* m_spectrumWidget;

	QString m_name;
	int m_id;

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

	qint32 m_workFreq;

	double m_currentFreq;
	double m_startFreq;
	double m_endFreq;
	double m_responseFreq;

	bool m_sleepModeProcess;

	ITabManager*		m_tabManager;

	int m_spectrumCounter;

	IDbManager* m_dbmanager;

	PanoramaFreqControl* m_panoramaFreqControl;
	QTime m_sonogramTime;

	QTimer* m_sleepModeTimer;
	QTimer* m_upTimer;

public:
	explicit SpectrumWidgetDataSource(IGraphicWidget* widget, QObject *parent = 0);
	virtual ~SpectrumWidgetDataSource();

	void onMethodCalled(const QString&, const QVariant&);
	void sendCommand(int);

	void setPanorama(bool enabled, double start = 0.0f, double end = 0.0f);
	bool isPanoramaEnabled();

	void setTabManager(ITabManager* manager);
	void setDBManager(IDbManager* manager);
	void setName(QString name);
	void setId(int id);

	void setPanoramaFreqControl(PanoramaFreqControl *control);
signals:
	void onMethodCalledSignal(QString, QVariant);

private slots:
	void onMethodCalledSlot(QString, QVariant);

	void slotRepeatSetFrequency();
	void slotChangeFreq();
	void onSleepModeSlot();
	void onUpdateFlag();


private:
	int dataProccess(QVector<QPointF>& vecFFT, bool);

	void setBandwidth(double bandwidth);
	void setBandwidth1(int size);

	int findIndex(qreal startx);

	bool startPanorama(bool start);


	void clearPeak();
	void setupPoints(const RdsProtobuf::ServerMessage_OneShotData_LocationData &data);
};

#endif // SPECTRUMWIDGETDATASOURCE_H
