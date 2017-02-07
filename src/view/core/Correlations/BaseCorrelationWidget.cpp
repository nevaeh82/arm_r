#include "BaseCorrelationWidget.h"

BaseCorrelationWidget::BaseCorrelationWidget(QWidget *parent, Qt::WFlags flags):
    QWidget(parent, flags),
    m_limit(500)
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

void BaseCorrelationWidget::setNewSko(float val)
{
  m_limit = val;
}

