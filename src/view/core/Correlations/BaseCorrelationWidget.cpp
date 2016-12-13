#include "BaseCorrelationWidget.h"

BaseCorrelationWidget::BaseCorrelationWidget(QWidget *parent, Qt::WFlags flags):
    QWidget(parent, flags)
{
}

BaseCorrelationWidget::~BaseCorrelationWidget()
{
}

void BaseCorrelationWidget::setLocationSetupWidget(LocationSetupWidgetController *controller)
{
	m_controller = controller;
}

bool BaseCorrelationWidget::isVisible() const
{
	return QWidget::isVisible();
}

