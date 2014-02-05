#ifndef CORRELATIONGROUPWIDGET_H
#define CORRELATIONGROUPWIDGET_H

#include <QWidget>

namespace Ui {
class CorrelationGroupWidget;
}

class CorrelationGroupWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CorrelationGroupWidget(QWidget *parent = 0);
	~CorrelationGroupWidget();

private:
	Ui::CorrelationGroupWidget *ui;
};

#endif // CORRELATIONGROUPWIDGET_H
