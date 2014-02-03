#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

namespace Ui {
class ControlPanelWidget;
}

class ControlPanelWidget : public QWidget
{
    Q_OBJECT
public:
	ControlPanelWidget(QWidget* parent = NULL);
	~ControlPanelWidget();

signals:
	void onStartFreqChanged(int value);
	void onEndFreqChanged(int value);
	void onCommonFreqChanged(int value);

	void onPanoramaChecked(bool isChecked);
	void onAutoSearchCheched(bool isChecked);

private:
	Ui::ControlPanelWidget*     ui;

};

#endif // CONTROLPANEL_H
