#include <Logger/Logger.h>

#include "CorrelationGroupColumnWidget.h"

#define MAX_ROW_COUNT 6

CorrelationGroupColumnWidget::CorrelationGroupColumnWidget(QWidget *parent) :
    CorrelationGroupWidget(parent)
{
}

CorrelationGroupColumnWidget::~CorrelationGroupColumnWidget()
{
	//log_debug("<<<<<<<<");
}

void CorrelationGroupColumnWidget::insertCorrelationWidget(ICorrelationWidget *widget)
{
	if (NULL ==widget) {
		return;
	}

	m_widgetList.append(widget);

    int row = getContainer()->count();

	getContainer()->addWidget(widget->getWidget(), row, 0, 1, 1);
}
