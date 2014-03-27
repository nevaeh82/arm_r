#ifndef SPECTRUMWIDGETCONTROLLER_H
#define SPECTRUMWIDGETCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QFile>

#include "Interfaces/IController.h"
#include "Interfaces/ISpectrumWidget.h"
#include "IGraphicWidget.h"
#include "Tabs/ITabSpectrum.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "Prm300ControlWidgetController.h"

#include "Tabs/RPC/RPCClient.h"

#define TO_MHZ	1000000

class SpectrumWidget;

class SpectrumWidgetController : public QObject, public ISpectrumWidget, public IController<SpectrumWidget>
{
	Q_OBJECT
private:
	SpectrumWidget* m_view;

	bool	m_autoSearch;
	double	m_current_frequency;

	quint32	m_id;

	ITabSpectrum*	m_tab;

	double	m_bandwidth;
	int		m_pointCount;
	bool	m_isComplex;

	QMutex	m_mux;
	bool	m_enableCorrelation;
	QMenu*	m_graphicsContextMenu;

	double	m_centerFreqSelTemp;
	double	m_centerFreqDefModulation;

	bool	m_peakVisible;

	double	m_threshold;
	int		m_rett;

	Q_MG_SpectrumInterface* m_graphicsWidget;

	Prm300ControlWidgetController* m_prm300WidgetController;

	RPCClient* m_rpcClient;

	const int TIMER_CLICK_DELAY;
	QTimer *m_clickDelay;
	bool flagDoubleClick;

public:
	explicit SpectrumWidgetController(QObject *parent = 0);

	void appendView(SpectrumWidget* view);

	void setTab(ITabSpectrum*);
	void setId(const int);
	void setSpectrumName(const QString&);

	void setFFTSetup(float* spectrum, float* spectrum_peak_hold);

	QString getSpectrumName() const;
	QWidget* getWidget() const;

	bool isGraphicVisible();
	quint32 getId();
	void setZeroFrequency(double val);
	void setVisible(const bool isVisible);

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setSelection(double start, double end);

	void setup();
	void setControlPrmState(bool state);

	void onDataArrived(const QString& method, const QVariant& arg);

	void setRpcClient(RPCClient* rpcClient);

private:
	void init();

	void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setSignal(float* spectrum, float* spectrum_peak_hold);
	void setDefModulation(QString modulation);
	void setLabelName(QString base, QString second);
	void setDetectedAreasUpdate(const QByteArray& vec);

signals:
	void doubleClickedSignal(int);
	void signalChoosedThreshold(double y);
	void signalAddSelToLists(int id);
	void signalCurSelChanged(int);
	void signalRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);

public slots:
	
private slots:
	void slotSetEnablePanorama(bool);
	void slotAutoSearch(bool);
	void slotSelectiontypeChanged(bool);
	void slotRequestData(bool);

	void slotCMAddWhiteList();
	void slotCMAddBlackList();
	void slotRecognizeSignal();
	void slotSSCorrelation();
	void slotClearLabels();

	void slotSelectionCleared();
	void slotSelectionFinished(double x1, double y1, double x2, double y2);
	void slotSelectionFinishedRedLine(double y);

	void slotIsShowContextMenu();
	void slotDoubleClicked(double, double);

	void slotShowPeaks(bool);
	void slotShowControlPRM(bool);

	void processClick();
};

#endif // SPECTRUMWIDGETCONTROLLER_H
