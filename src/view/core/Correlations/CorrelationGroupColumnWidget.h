#pragma once

#include <QWidget>
#include "CorrelationWidget.h"
#include "CorrelationWidgetController.h"
#include "CorrelationGroupWidget.h"


class CorrelationGroupColumnWidget : public CorrelationGroupWidget
{
	Q_OBJECT

private:
	Ui::CorrelationGroupWidget *ui;

	QList<ICorrelationWidget*> m_widgetList;

public:
    explicit CorrelationGroupColumnWidget(QWidget *parent = 0);
    ~CorrelationGroupColumnWidget();

	void insertCorrelationWidget(int row, ICorrelationWidget *widget);


};

