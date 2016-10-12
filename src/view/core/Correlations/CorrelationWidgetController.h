#ifndef CORRELATIONWIDGETCONTROLLER_H
#define CORRELATIONWIDGETCONTROLLER_H

#include <QObject>
#include <QVariant>
#include "Interfaces/IController.h"
#include "Interfaces/ICorrelationWidget.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

#include "BaseCorrelationWidget.h"

class CorrelationWidget;

class CorrelationWidgetController : public QObject, public ICorrelationWidget
{
	Q_OBJECT
private:
    BaseCorrelationWidget* m_view;

	double	m_bandwidth;
	int	m_pointCount;
	bool	m_isComplex;

	QString	m_labelName;

public:
	explicit CorrelationWidgetController(QObject *parent = 0);

	bool isGraphicVisible();
	quint32 getId();
	void setZeroFrequency(double val);

    void appendView(BaseCorrelationWidget *view);

	void onDataArrived(const QString& method, const QVariant& arg);

	void clear();
	QWidget* getWidget() const;
	void setVisible(const bool isVisible);

    virtual void setLabels(QString l1, QString l2);

private slots:
	void onDataArrivedLS(const QString method, const QVariant arg);
	void onVisible(const bool b);

private:
    void setDataSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex, float sko);
    void setData(float *spectrum, float *spectrum_peak_hold, float sko);
	void setLabelName(QString base, QString second);

signals:
	void signalonDataArrivedLS(const QString& method, const QVariant& arg);
	void signalOnVisible(const bool);

};

#endif // CORRELATIONWIDGETCONTROLLER_H
