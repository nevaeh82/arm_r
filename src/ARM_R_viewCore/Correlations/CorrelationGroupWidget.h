#ifndef CORRELATIONGROUPWIDGET_H
#define CORRELATIONGROUPWIDGET_H

#include <QWidget>
#include "CorrelationWidget.h"

namespace Ui {
class CorrelationGroupWidget;
}

class CorrelationGroupWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::CorrelationGroupWidget *ui;

	QList<CorrelationWidget*> m_widgetList;

public:
	explicit CorrelationGroupWidget(QWidget *parent = 0);
	~CorrelationGroupWidget();

	void insertCorrelationWidget(CorrelationWidget* widget);
	void clearWidgetContainer();


};

#endif // CORRELATIONGROUPWIDGET_H
