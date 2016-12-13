#pragma once

#include <QWidget>
#include "Tabs/LocationSetupWidgetController.h"

class BaseCorrelationWidget : public QWidget
{
	Q_OBJECT

public:
	BaseCorrelationWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~BaseCorrelationWidget();

	virtual void reset() = 0;
	virtual void setDataSetup(double bandwidth, int m_pointCount,
							  float *spectrum, float *spectrum_peak_hold,
							  float sko) = 0;

	virtual void permanentSetup(float *spectrum, float *spectrum_peak_hold,
								float sko) = 0;

	virtual void setLabel(const QString& label) = 0;

	void setLocationSetupWidget(LocationSetupWidgetController* controller);
	virtual bool isVisible() const;

protected:
	LocationSetupWidgetController* m_controller;

};

