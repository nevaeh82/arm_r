#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QApplication>

#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QPoint>
#include <qmath.h>
#include <QMutex>
#include <QMenu>
#include <QAction>
#include <QSpinBox>
#include <QPushButton>

#include <QTimer>

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "Tabs/ITabSpectrum.h"
#include "IGraphicWidget.h"

#include "Common/CommandMessage.h"

namespace Ui {
class SpectrumWidget;
}

class SpectrumWidget : public QWidget, public IGraphicWidget
{
	Q_OBJECT
public:
	SpectrumWidget(QWidget *parent = 0, Qt::WFlags flags = 0, QString name = "", int id = 0);
	~SpectrumWidget();

	void setTab(ITabSpectrum* tab);
	void setId(const int id);
	void setSpectrumName(const QString& name);
	QString getSpectrumName() const;

private:
	Ui::SpectrumWidget*	ui;

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

public:
	Q_MG_SpectrumInterface* getSpectrumWidget();
	virtual void setSignalSetup(float* spectrum, float* spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	virtual void setSignal(float* spectrum, float* spectrum_peak_hold);
	virtual bool isGraphicVisible();
	virtual void setDefModulation(QString modulation);
	virtual void setLabelName(QString base, QString second);

	virtual void setDetectedAreasUpdate(QVector<QPointF> vec);
	virtual void setZeroFrequency(double val);

	void setup();
	void setCoontrolPrmState(bool state);

public slots:
	void slotSelectionFinished(double x1, double y1, double x2, double y2);
	void slotSelectionFinishedRedLine(double y);
	void slotSetCaption(QString name);
	void slotSetCorrelations(int id, const QVector<QPointF> vecFFT, const bool isComplex);
	void slotSetFFT(float* spectrum, float* spectrum_peak_hold);
	void slotSetFFTSetup(float* spectrum, float* spectrum_peak_hold);
	void slotSetDefModulation(QString modulation);

	void slotSetDetectedAreas(QVector<QPointF> vec);

private slots:
	void slotSetEnablePanorama(bool state);
	void slotAutoSearch(bool state);
	void slotSelectiontypeChanged(bool);
	void slotRequestData(bool state);
	void slotEnableKM(bool state);
	void slotIsShowContextMenu();
	void slotCMAddWhiteList();
	void slotCMAddBlackList();
	void slotRecognizeSignal();
	void slotSSCorrelation();
	void slotClearLabels();

	void slotShowPeaks(bool);
	void slotSetLabelName(QString base, QString second);

	void slotShowControlPRM(bool state);

	void slotDoubleClicked(double, double);

	void slotSetZeroFrequency(double val);

signals:
	void selected(double x1, double x2, double y1, double y2);
	void signalChoosedThreshold(double y);
	void signalCurSelChanged(int);
	void signalRequestData(unsigned int id, unsigned int type, int *data, unsigned int length);
	void signalEnableKM(bool state);

	/// id - is id of ilsts (1 - White list, 2 - Black list)
	void signalAddSelToLists(int id);

	void signalSetFFTSetup(float* spectrum, float* spectrum_peak_hold);
	void signalSetFFT(float* spectrum, float* spectrum_peak_hold);
	void signalSetDefModulation(QString modulation);

	void signalFinished();
	void signalNeedToUpdate();
	void signalSetLabelName(QString, QString);
	void signalSetControlPRMState(bool state);

	void doubleClickedSignal(int);

	void signalSetZeroFrequency(double val);
	void signalSetDetectedAreas(QVector<QPointF> vec);

private:
	virtual void mouseDoubleClickEvent ( QMouseEvent * event );
};

#endif // GRAPHICWIDGET_H
