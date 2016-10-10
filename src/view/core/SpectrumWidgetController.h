#ifndef SPECTRUMWIDGETCONTROLLER_H
#define SPECTRUMWIDGETCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QList>
#include <QVector>
#include <QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>

#include <qwt_plot.h>
#include <qwt_point_3d.h>
#include <qwt_color_map.h>
#include <qwt_plot_spectrocurve.h>
#include <qwt_plot_rasteritem.h>
#include <qwt_plot_svgitem.h>

#include "Interfaces/IController.h"
#include "Interfaces/ISpectrumWidget.h"
#include "Interfaces/ICorrelationListener.h"
#include "IGraphicWidget.h"
#include "Tabs/ITabSpectrum.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"
#include "Common/Charts/qcustomplot.h"
#include "Common/Charts/QCPLinearColorMap.h"
#include "Common/Charts/ColorGraph.h"
#include "Common/Charts/QColorCustomPlot.h"

#include "Prm300ControlWidgetController.h"

#include "Tabs/RPC/RpcPrmClient.h"

#include "DBStation/DBStationController.h"

#include "SignalDetectedDialog.h"
#include "Analysis/AnalysisResultWidget.h"

#include "ControlPanel/ControlPanelController.h"

#include "Common/Charts/QwtColorMap.h"
#include "Tabs/LocationSetupWidgetController.h"

#include "RDSExchange.h"

#define TO_MHZ	1000000.0
#define TO_MHZ2	1000.0


class SpectrumWidget;

class SpectrumWidgetController : public QObject, public ISpectrumWidget, public IController<SpectrumWidget>
		, public ICorrelationListener
{
	Q_OBJECT
private:
	SpectrumWidget* m_view;
    AnalysisResultWidget* m_analysisResult;

	bool	m_autoSearch;
	double	m_current_frequency;

	quint32	m_id;
	quint32 m_platformId;
	quint32 m_channelId;

	quint32 m_analysisChannel;

	ITabSpectrum*	m_tab;

	double	m_bandwidth;
	int		m_pointCount;
	bool	m_isComplex;

	QMutex	m_mux;
	bool	m_enableCorrelation;
	QMenu*	m_graphicsContextMenu;

	double	m_centerFreqSelTemp;
	double  m_bandwidhtTemp;
	double	m_centerFreqDefModulation;

	bool	m_peakVisible;

	double	m_threshold;
	int		m_rett;

	QVector<QPointF> m_pointVector;

	Q_MG_SpectrumInterface* m_graphicsWidget;

	//Qwt graph, its line and data
	QwtPlot      *m_sonogramWidget;
	QVector<QwtPoint3D> *m_qwtVector;
	QwtPlotSpectroCurve *m_qwtCurve;
	ColorMap* m_qwtColorMap;

	QMap<int, ColorGraph*> m_mapGraph;

    int m_timming;
    int m_timmingGlobal;
    int m_timmingCount;
    double m_timmingCurPos;

	Prm300ControlWidgetController* m_prm300WidgetController;

	RpcPrmClient* m_rpcClient;
    RpcFlakonClientWrapper* m_rpcFlakonClient;

	IDbManager* m_dbManager;
	DBStationController* m_dbStationController;
	ControlPanelController* m_controlPanelController;
    QCPLinearColorMap       m_mapColor;

	bool nextClearState;
	SpectrumSelection tmpSelection;

	bool m_spectrumShow;

	QList<double> m_overthreshold;

	bool correlationFlag;

	int m_controlPanelMode;
	bool m_stopFlag;

	SignalDetectedDialog* m_sigDialog;
	QString m_name;

	QFutureWatcher<void> m_sonogramWatcher;
	bool m_sonogramReady;
	QTime m_sonogramTime;

	QPixmap m_sonogramPixmap;
	QMutex m_sonogramMutex;

	LocationSetupWidgetController* m_setupController;

public:
	explicit SpectrumWidgetController(QObject *parent = 0);
	virtual ~SpectrumWidgetController();

	void appendView(SpectrumWidget* view);

	void setTab(ITabSpectrum*);
	void setId(const int);
	void setPlatformId(const int);
	void setChannelId(const int);

	void setSpectrumName(const QString&);

	void setFFTSetup(float* spectrum, float* spectrum_peak_hold);

	QString getSpectrumName() const;
	QWidget *getWidget() const;

	bool isGraphicVisible();

	quint32 getId();
	quint32 getPlatformId();
	quint32 getChannelId();

	void setZeroFrequency(double val);
	void setVisible(const bool isVisible);

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setSelection(double start, double end);

	virtual void setDbManager(IDbManager* dbManager);
	virtual void setDbStationController(DBStationController* controller);
	virtual void setControlPanelController(ControlPanelController* controller);

	void setup();
	void setControlPrmState(bool state);

	void onDataArrived(const QString& method, const QVariant& arg);

	void setRpcPrmClient(RpcPrmClient* rpcClient);
	void setRpcFlakonClient(RpcFlakonClientWrapper* rpcClient);

	void onCorrelationStateChanged(const bool isEnabled);

	void updateDBAreas();

	double getCenterSelection();

	Prm300ControlWidgetController *getPrmController();

	void setLocationSetupWidgetController(LocationSetupWidgetController* controller);

private:
	void init();

	void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setSignal(float* spectrum, float* spectrum_peak_hold);
	void setDefModulation(QString modulation);
	void setLabelName(QString base, QString second);
	void setDetectedAreasUpdate(const QByteArray& vec);
	void setSpectrumShow(bool state);

	void setDetectedAreas(int mode, const QList<StationsFrequencyAndBandwith>& list);


	void initGraph();
	void setSonogramSetup(QList<QList<float>> data);

signals:
	void doubleClickedSignal(int);
	void signalChoosedThreshold(double y);
	void signalAddSelToLists(int id);
	void signalCurSelChanged(int);
	void signalRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);
	void signalSpectrumEnable(bool state);
	void signalVisible(const bool);

	void onCorrelationStateChangedSignal(const bool isEnabled);

	void onSignalSetWorkMode(int, bool);
    void signalAddToList(QString name, double start, double bandwidth);

public slots:

private slots:
	void slotSetEnablePanorama(bool);
	void slotAutoSearch(bool);
	void slotSelectiontypeChanged(bool);
	void slotRequestData(bool);

	void addToWhiteList();
	void addToBlackList();
	void recognizeSignal();
	void toggleCorrelation();
	void toggleOffCorrelation();
	void clearLabels();

	void slotSelectionCleared();
	void slotSelectionFinished(double x1, double y1, double x2, double y2);
	void slotSelectionFinishedRedLine(double y);

	void slotIsShowContextMenu();
	void slotDoubleClicked(double, double);

	void slotShowPeaks(bool);
	void slotShowControlPRM(bool);

	void slotControlPanelMode(int mode);

	void onVisible(const bool b);

	void onCorrelationStateChangedSlot(const bool isEnabled);

	void onSonogramDataReady();

	void slotSetAnalysisChannel(int id);
	void setDetectedAreasUpdateOnPlot();

    void slotAddToList(double start, double end);

     void slotContinueAnalysis(bool);
};

#endif // SPECTRUMWIDGETCONTROLLER_H
