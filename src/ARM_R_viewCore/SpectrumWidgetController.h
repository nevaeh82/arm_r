#ifndef SPECTRUMWIDGETCONTROLLER_H
#define SPECTRUMWIDGETCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QMenu>
#include <QString>
#include <QMessageBox>

#include "Interfaces/IController.h"
#include "Interfaces/ISpectrumWidget.h"
#include "IGraphicWidget.h"
#include "Tabs/ITabSpectrum.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#define TO_MHZ	1000000

class SpectrumWidget;

class SpectrumWidgetController : public QObject, public ISpectrumWidget, public IController<SpectrumWidget>
{
	Q_OBJECT
private:
	SpectrumWidget* m_view;

	bool	m_autoSearch;
	double	m_current_frequency;

	int	m_id;
	float*	m_spectrumPeakHold;
	float*	m_spectrumPeakHoldCorr;

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

public:
	explicit SpectrumWidgetController(QObject *parent = 0);

	void appendView(SpectrumWidget* view);

	void setTab(ITabSpectrum*);
	void setId(const int);
	void setSpectrumName(const QString&);

	void setFFTSetup(float* spectrum, float* spectrum_peak_hold);

	QString getSpectrumName() const;
	QWidget* getWidget() const;

	void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setSignal(float* spectrum, float* spectrum_peak_hold);
	void setDefModulation(QString modulation);
	bool isGraphicVisible();
	void setLabelName(QString base, QString second);
	void setDetectedAreasUpdate(const QByteArray& vec);
	void setZeroFrequency(double val);

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setup();
	void setControlPrmState(bool state);

	void onDataArrived(const QString& method, const QVariant& arg);
	void onDataArrived(float*, float*, int, double, bool);
	void onDataArrived(float*, float*);
private:
	void init();

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

	void slotSelectionFinished(double x1, double y1, double x2, double y2);
	void slotSelectionFinishedRedLine(double y);

	void slotIsShowContextMenu();
	void slotDoubleClicked(double d1, double d2);

	void slotShowPeaks(bool);
	void slotShowControlPRM(bool);
};

#endif // SPECTRUMWIDGETCONTROLLER_H
