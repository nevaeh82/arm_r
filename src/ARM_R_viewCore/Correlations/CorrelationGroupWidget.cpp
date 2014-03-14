#include "CorrelationGroupWidget.h"
#include "ui_CorrelationGroupWidget.h"

CorrelationGroupWidget::CorrelationGroupWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CorrelationGroupWidget)
{
	ui->setupUi(this);
	m_logger = Pw::Logger::PwLoggerFactory::Instance()->createLogger(LOGGERCLASSNAME(CorrelationGroupWidget));
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

	ui->correlationWidgetsContainer->insertWidget(ui->correlationWidgetsContainer->count(), widget->getWidget());	
	m_logger->debug(QString::number(ui->correlationWidgetsContainer->count()));
}

void CorrelationGroupWidget::clearWidgetContainer()
{
	foreach (ICorrelationWidget* widget , m_widgetList) {
		ui->correlationWidgetsContainer->removeWidget(widget->getWidget());
		widget->clear();
	}

	m_widgetList.clear();
	m_logger->debug(QString::number(ui->correlationWidgetsContainer->count()));
}
