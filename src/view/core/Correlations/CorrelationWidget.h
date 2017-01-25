#ifndef CORRELATIONWIDGET_H
#define CORRELATIONWIDGET_H

#include <QWidget>
#include <Common/Charts/qcustomplot.h>

#include "BaseCorrelationWidget.h"
#include "CorrelationWidgetController.h"

namespace Ui {
class CorrelationWidget;
}

class CorrelationWidget : public BaseCorrelationWidget
{
	Q_OBJECT

private:
	Ui::CorrelationWidget*	ui;

	QCustomPlot* m_plot;

	void initCustomPlot(QCustomPlot *plot);

public:
	CorrelationWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~CorrelationWidget();

	virtual void reset();
	virtual void setDataSetup(double bandwidth, int m_pointCount,
							  float *spectrum, float *spectrum_peak_hold,
							  float sko);

	virtual void permanentSetup(float *spectrum, float *spectrum_peak_hold,
								float sko);

	virtual void setupDoppler(const QQueue<double> &data);

	virtual void setLabel(const QString& label);
	void paintEvent(QPaintEvent *);

	void setDopplerVisible(bool);
};

#endif // CORRELATIONWIDGET_H
