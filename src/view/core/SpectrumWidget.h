#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QApplication>
#include <Common/Charts/qcustomplot.h>

#include <QWidget>
#include <QList>

#include "Common/CommandMessage.h"
#include "SpectrumWidgetController.h"

#include "Prm300ControlWidget.h"
#include "Common/Charts/qcustomplot.h"

#include "Analysis/AnalysisResultWidget.h"

#include <qwt_plot.h>
#include <qwt_plot_glcanvas.h>

namespace Ui {
class SpectrumWidget;
}

class SpectrumWidget : public QWidget
{
	Q_OBJECT
private:
	Ui::SpectrumWidget*	ui;

	SpectrumWidgetController* m_controller;

	QCustomPlot* m_plot;
	QCPItemPixmap* m_plotPixmap;

	void initCustomPlot(QCustomPlot *plot);
public:
	SpectrumWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SpectrumWidget();

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setSpectrumName(const QString& name);
	QString getSpectrumName() const;

	Q_MG_SpectrumInterface* getGraphicsWidget();

	void setControlPrmState(bool state);

	Prm300ControlWidget* getPrm300Widget();

	void sonogramUpdate(const QPixmap& px);
	void setAnalysisDetectedData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData &msg);

	bool isSonogram();
	bool isThreshold() const;
	bool isGlobalThreshold() const;

	void setThresholdValue(const int);

public slots:
	void slotSetEnableSpactrum(bool state);
	void slotEnableKM(bool state);
	void setNoSignal(bool);
	void screenshotSpectrum(double val);

private slots:
	void slotSetWorkMode(int mode, bool isOn);
	void onSlotChangedViewPort(double start_hz, double start_Y, double end_hz, double end_Y);
	void onZoomSonogram(double start_hz, double end_hz, bool replot = false);
	void onThresholdToggled(bool);

signals:
	void signalEnableKM(bool state);

	void setPanoramaSignal(bool);
	void setAutoSearchSignal(bool);
	void selectionTypeChangedSignal(bool);
	void requestDataSignal(bool);

	void setShowPeaksSignal(bool);
	void setShowControlPRM(bool);

	void signalManualThreshold(double);
	void onGlobalThresholdToggled(bool);

protected:
	void paintEvent(QPaintEvent *);
};

#endif // GRAPHICWIDGET_H
