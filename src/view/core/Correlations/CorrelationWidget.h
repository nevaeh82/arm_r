#ifndef CORRELATIONWIDGET_H
#define CORRELATIONWIDGET_H

#include <QWidget>

#include "CorrelationWidgetController.h"

namespace Ui {
class CorrelationWidget;
}

class CorrelationWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::CorrelationWidget*	ui;

public:
	CorrelationWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	virtual ~CorrelationWidget();

//	Q_MG_SpectrumInterface* getGraphicsWidget();
};

#endif // CORRELATIONWIDGET_H
