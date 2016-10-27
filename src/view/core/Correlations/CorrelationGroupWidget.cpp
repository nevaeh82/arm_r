#include <Logger/Logger.h>

#include "CorrelationGroupWidget.h"
#include "ui_CorrelationGroupWidget.h"

#define MAX_ROW_COUNT 6

CorrelationGroupWidget::CorrelationGroupWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CorrelationGroupWidget)
{
	ui->setupUi(this);
    this->setWindowTitle(tr("Correlations"));
}

CorrelationGroupWidget::~CorrelationGroupWidget()
{
	delete ui;
}

void CorrelationGroupWidget::insertCorrelationWidget(ICorrelationWidget *widget)
{
	if (NULL ==widget) {
		return;
	}

	m_widgetList.append(widget);

	int column = ui->correlationWidgetsContainer->count() / MAX_ROW_COUNT;
	int row = ui->correlationWidgetsContainer->count()%MAX_ROW_COUNT;

	ui->correlationWidgetsContainer->addWidget(widget->getWidget(), row, column, 1, 1);
}

void CorrelationGroupWidget::clearWidgetContainer()
{
	foreach (ICorrelationWidget* widget , m_widgetList) {
		ui->correlationWidgetsContainer->removeWidget(widget->getWidget());
		//widget->clear();
	}

	m_widgetList.clear();
    //log_debug(QString::number(ui->correlationWidgetsContainer->count()));
}

QGridLayout *CorrelationGroupWidget::getContainer()
{
    return ui->correlationWidgetsContainer;
}
