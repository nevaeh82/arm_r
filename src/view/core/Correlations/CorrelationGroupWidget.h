#ifndef CORRELATIONGROUPWIDGET_H
#define CORRELATIONGROUPWIDGET_H

#include <QWidget>
#include "CorrelationWidget.h"
#include "CorrelationWidgetController.h"

namespace Ui {
class CorrelationGroupWidget;
}

class CorrelationGroupWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::CorrelationGroupWidget *ui;

	QList<ICorrelationWidget*> m_widgetList;

public:
	explicit CorrelationGroupWidget(QWidget *parent = 0);
	~CorrelationGroupWidget();

	void insertCorrelationWidget(ICorrelationWidget *widget);
	void clearWidgetContainer();

    QGridLayout* getContainer();


};

#endif // CORRELATIONGROUPWIDGET_H
