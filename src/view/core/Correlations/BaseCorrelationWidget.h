#pragma once

#include <QWidget>
#include <QQueue>
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

	virtual void setupDoppler(const QQueue<double>& data) = 0;

	virtual void setLabel(const QString& label) = 0;

	void setLocationSetupWidget(LocationSetupWidgetController* controller);
	virtual bool isVisible() const;

	virtual void setDopplerVisible(bool) = 0;

protected:
	LocationSetupWidgetController* m_controller;

    float m_limit;

private slots:
    void setNewSko(float val);

};
