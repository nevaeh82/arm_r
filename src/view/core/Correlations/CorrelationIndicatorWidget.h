#pragma once

#include <QWidget>

#include "BaseCorrelationWidget.h"
#include "CorrelationWidgetController.h"

namespace Ui {
class CorrelationIndicatorWidget;
}

class CorrelationIndicatorWidget : public BaseCorrelationWidget
{
	Q_OBJECT

private:
    Ui::CorrelationIndicatorWidget*	ui;

public:
    CorrelationIndicatorWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
    virtual ~CorrelationIndicatorWidget();

    virtual void reset();
    virtual void setDataSetup(double bandwidth, int m_pointCount,
                              float *spectrum, float *spectrum_peak_hold,
                              float sko);

    virtual void permanentSetup(float *spectrum, float *spectrum_peak_hold,
                                float sko);

    virtual void setLabel(const QString& label);

private:
    QPixmap* m_red;
    QPixmap* m_green;

    uint m_limit;
};
