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
#include "Interfaces/ICorrelationListener.h"
#include "IGraphicWidget.h"
#include "Tabs/ITabSpectrum.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "Prm300ControlWidgetController.h"

#include "Tabs/RPC/RpcPrmClient.h"

#include "DBStation/DBStationController.h"

#include "SignalDetectedDialog.h"

#define TO_MHZ	1000000.0
#define TO_MHZ2	1000


class SpectrumWidget;

class SpectrumWidgetController : public QObject, public ISpectrumWidget, public IController<SpectrumWidget>
		, public ICorrelationListener
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
	double  m_bandwidhtTemp;
	double	m_centerFreqDefModulation;

	bool	m_peakVisible;

	double	m_threshold;
	int		m_rett;

	Q_MG_SpectrumInterface* m_graphicsWidget;

	Prm300ControlWidgetController* m_prm300WidgetController;

	RpcPrmClient* m_rpcClient;

	IDbManager* m_dbManager;
	DBStationController* m_dbStationController;

	bool nextClearState;
	SpectrumSelection tmpSelection;

	bool m_spectrumShow;

	double m_overthreshold;

	bool correlationFlag;

public:
	explicit SpectrumWidgetController(QObject *parent = 0);
	virtual ~SpectrumWidgetController();

	void appendView(SpectrumWidget* view);

	void setTab(ITabSpectrum*);
	void setId(const int);
	void setSpectrumName(const QString&);

	void setFFTSetup(float* spectrum, float* spectrum_peak_hold);

	QString getSpectrumName() const;
	QWidget *getWidget() const;

	bool isGraphicVisible();
	quint32 getId();
	void setZeroFrequency(double val);
	void setVisible(const bool isVisible);

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setSelection(double start, double end);

	virtual void setDbManager(IDbManager* dbManager);
	virtual void setDbStationController(DBStationController* controller);

	void setup();
	void setControlPrmState(bool state);

	void onDataArrived(const QString& method, const QVariant& arg);

	void setRpcPrmClient(RpcPrmClient* rpcClient);

	void onCorrelationStateChanged(const bool isEnabled);

private:
	void init();

	void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setSignal(float* spectrum, float* spectrum_peak_hold);
	void setDefModulation(QString modulation);
	void setLabelName(QString base, QString second);
	void setDetectedAreasUpdate(const QByteArray& vec);
	void setSpectrumShow(bool state);

signals:
	void doubleClickedSignal(int);
	void signalChoosedThreshold(double y);
	void signalAddSelToLists(int id);
	void signalCurSelChanged(int);
	void signalRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);
	void signalSpectrumEnable(bool state);

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
	void clearLabels();

	void slotSelectionCleared();
	void slotSelectionFinished(double x1, double y1, double x2, double y2);
	void slotSelectionFinishedRedLine(double y);

	void slotIsShowContextMenu();
	void slotDoubleClicked(double, double);

	void slotShowPeaks(bool);
	void slotShowControlPRM(bool);
};

#endif // SPECTRUMWIDGETCONTROLLER_H
