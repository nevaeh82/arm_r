#include "CorrelationGroupWidget.h"
#include "ui_CorrelationGroupWidget.h"

CorrelationGroupWidget::CorrelationGroupWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CorrelationGroupWidget)
{
	ui->setupUi(this);
}

CorrelationGroupWidget::~CorrelationGroupWidget()
{
	delete ui;
}

void CorrelationGroupWidget::insertCorrelationWidget(CorrelationWidget *widget)
{
	if (NULL ==widget) {
		return;
	}

	m_widgetList.append(widget);

	ui->correlationWidgetsContainer->insertWidget(ui->correlationWidgetsContainer->count(), widget);
}

void CorrelationGroupWidget::clearWidgetContainer()
{
	foreach (CorrelationWidget* widget , m_widgetList) {

		ui->correlationWidgetsContainer->removeWidget(widget);

		widget->clear();
	}

	m_widgetList.clear();
}
