#ifndef PRM300CONTROLWIDGET_H
#define PRM300CONTROLWIDGET_H

#include <QWidget>

#include "Common/CommandMessage.h"
#include "Info/StationConfiguration.h"


namespace Ui {
class Prm300ControlWidget;
}

class Prm300ControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit Prm300ControlWidget(QWidget *parent = 0);
	~Prm300ControlWidget();

	void setData(quint16 freq, quint8 filter, quint8 att1, quint8 att2);

	Prm getPrmParams();

private:
	Ui::Prm300ControlWidget *ui;

	Prm m_params;

private slots:
	void slotAtt1Up();
	void slotAtt1Down();
	void slotAtt2Up();
	void slotAtt2Down();

	void setPrmParams();

signals:
	void signalOnSetParams();

    void signalOnEnableReceiver(bool);
};

#endif // PRM300CONTROLWIDGET_H
