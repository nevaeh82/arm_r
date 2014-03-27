#ifndef PRM300CONTROLWIDGET_H
#define PRM300CONTROLWIDGET_H

#include <QWidget>

#include "Common/CommandMessage.h"


namespace Ui {
class Prm300ControlWidget;
}

class Prm300ControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit Prm300ControlWidget(QWidget *parent = 0);
	~Prm300ControlWidget();

	void setData(int freq, int filter, int att1, int att2);

private:
	Ui::Prm300ControlWidget *ui;

private slots:
	void slotAtt1Up();
	void slotAtt1Down();
	void slotAtt2Up();
	void slotAtt2Down();

signals:
	void signalSetAtt1Value(int value);
	void signalSetAtt2Value(int value);
	void signalSetFilter(int index);

};

#endif // PRM300CONTROLWIDGET_H
