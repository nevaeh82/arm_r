#ifndef CORRELATIONWIDGETCONTROLLER_H
#define CORRELATIONWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>
#include "Interfaces/IController.h"
#include "Interfaces/ICorrelationWidget.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

class CorrelationWidget;

class CorrelationWidgetController : public QObject, public ICorrelationWidget, public IController<CorrelationWidget>
{
	Q_OBJECT
private:
	CorrelationWidget* m_view;

	double	m_bandwidth;
	int	m_pointCount;
	bool	m_isComplex;

	Q_MG_SpectrumInterface* m_graphicsWidget;

	QString	m_labelName;

public:
	explicit CorrelationWidgetController(QObject *parent = 0);

	bool isGraphicVisible();
	quint32 getId();
	void setZeroFrequency(double val);

	void appendView(CorrelationWidget *view);

	void onDataArrived(const QString& method, const QVariant& arg);

	void clear();
	QWidget* getWidget() const;
	void setVisible(const bool isVisible);

private:
	void setDataSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setData(float *spectrum, float *spectrum_peak_hold);
	void setLabelName(QString base, QString second);

};

#endif // CORRELATIONWIDGETCONTROLLER_H
