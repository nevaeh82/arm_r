#include <Logger/Logger.h>

#include "AnalysisGroupWidget.h"
#include "ui_AnalysisGroupWidget.h"

AnalysisGroupWidget::AnalysisGroupWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AnalysisGroupWidget)
{
	ui->setupUi(this);
}

AnalysisGroupWidget::~AnalysisGroupWidget()
{
	delete ui;
}

void AnalysisGroupWidget::insertAnalysisWidget(IAnalysisWidget *widget)
{
	if (NULL ==widget) {
		return;
	}

	m_widgetList.append(widget);

	ui->analysisWidgetsContainer->insertWidget(ui->analysisWidgetsContainer->count(), widget->getWidget());
	widget->getWidget()->show();
}

void AnalysisGroupWidget::clearWidgetContainer()
{
	foreach (IAnalysisWidget* widget , m_widgetList) {
		ui->analysisWidgetsContainer->removeWidget(widget->getWidget());
	}

	m_widgetList.clear();
}
