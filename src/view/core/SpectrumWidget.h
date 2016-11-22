#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QApplication>
#include <Common/Charts/qcustomplot.h>

#include <QWidget>

#include "Common/CommandMessage.h"
#include "SpectrumWidgetController.h"

#include "Prm300ControlWidget.h"
#include "Common/Charts/QColorCustomPlot.h"

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

	QwtPlot* m_qwtSonogram;
	QwtPlotGLCanvas* m_qwtSonogramGL;
	QBrush *m_texture;

	QCustomPlot* m_plot;
	QCPItemPixmap* m_plotPixmap;

    AnalysisResultWidget* m_analysisResultWidget;

public:
	SpectrumWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SpectrumWidget();

	void setAutoSearch(bool);
	void setPanorama(bool);

	void setSpectrumName(const QString& name);
	QString getSpectrumName() const;    

	Q_MG_SpectrumInterface* getGraphicsWidget();
	QwtPlot*       getSonogramWidget();

	void setControlPrmState(bool state);

	Prm300ControlWidget* getPrm300Widget();

	void sonogramUpdate(const QPixmap& px);
	void setAnalysisDetectedData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData &msg);

    AnalysisResultWidget* getAnalysisResultWidget();

public slots:
	void slotSetEnableSpactrum(bool state);
	void slotEnableKM(bool state);
    void recognize();
    void setNoSignal(bool);

private slots:
	void slotSetWorkMode(int mode, bool isOn);
	void onSlotChangedViewPort(double start_hz, double start_Y, double end_hz, double end_Y);
	void onZoomSonogram(double start_hz, double end_hz, bool replot = false);

signals:
	void signalEnableKM(bool state);

	void setPanoramaSignal(bool);
	void setAutoSearchSignal(bool);
	void selectionTypeChangedSignal(bool);
	void requestDataSignal(bool);

	void setShowPeaksSignal(bool);
	void setShowControlPRM(bool);
};

#endif // GRAPHICWIDGET_H
