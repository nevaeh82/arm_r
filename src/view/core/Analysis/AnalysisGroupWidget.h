#pragma once

#include <QWidget>
#include "AnalysisWidget.h"
#include "AnalysisWidgetController.h"

namespace Ui {
class AnalysisGroupWidget;
}

class AnalysisGroupWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AnalysisGroupWidget *ui;

	QList<IAnalysisWidget*> m_widgetList;

public:
	explicit AnalysisGroupWidget(QWidget *parent = 0);
	~AnalysisGroupWidget();

	void insertAnalysisWidget(IAnalysisWidget *widget);
	void clearWidgetContainer();
};
