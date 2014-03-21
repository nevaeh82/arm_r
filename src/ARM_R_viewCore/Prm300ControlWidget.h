#ifndef PRM300CONTROLWIDGET_H
#define PRM300CONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class Prm300ControlWidget;
}

class Prm300ControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit Prm300ControlWidget(QWidget *parent = 0);
	~Prm300ControlWidget();

private:
	Ui::Prm300ControlWidget *ui;
};

#endif // PRM300CONTROLWIDGET_H
