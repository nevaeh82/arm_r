#pragma once

#include <QObject>
#include <QVariant>
#include "Interfaces/IController.h"
#include "Interfaces/IAnalysisWidget.h"

#include "modules/gui/multi_graphics/components_relation.h"
#include "modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"

class AnalysisWidget;

class AnalysisWidgetController : public QObject, public IAnalysisWidget, public IController<AnalysisWidget>
{
	Q_OBJECT
private:
	AnalysisWidget* m_view;

	double	m_bandwidth;
	int	m_pointCount;
	bool	m_isComplex;

	Q_MG_SpectrumInterface* m_graphicsWidget;

	QString	m_labelName;

	int m_type;

public:
	explicit AnalysisWidgetController(int type, QObject *parent = 0);

	bool isGraphicVisible();
	void setZeroFrequency(double val);

	void appendView(AnalysisWidget *view);

	void onDataArrived(const QString& method, const QVariant& arg);

	void clear();
	QWidget* getWidget() const;
	void setVisible(const bool isVisible);

	int getType() const;

	virtual quint32 getId();

private slots:
	void onDataArrivedLS(const QString method, const QVariant arg);
	void onVisible(const bool b);

private:
	void setDataSetup(float *spectrum, float *spectrum_peak_hold, int PointCount, double bandwidth, bool isComplex);
	void setData(float *spectrum, float *spectrum_peak_hold);
	void setLabelName(QString base, QString second);

signals:
	void signalonDataArrivedLS(const QString& method, const QVariant& arg);
	void signalOnVisible(const bool);

};
