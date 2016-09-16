#pragma once

#include <QWidget>

#include "AnalysisWidgetController.h"

namespace Ui {
class AnalysisWidget;
}

class AnalysisWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::AnalysisWidget*	ui;
	int m_type;

public:
	AnalysisWidget(int type, QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~AnalysisWidget();

	Q_MG_SpectrumInterface* getGraphicsWidget();
};
