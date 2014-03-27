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

private:
	Ui::ControlPanelWidget*     ui;

public:
	ControlPanelWidget(QWidget* parent = NULL);
	~ControlPanelWidget();

signals:
	void commonFreqChangedSignal(int value);
	void bandwidthChangedSignal(int start, int end);

	void panoramaCheckedSignal(bool isChecked);
	void autoSearchCheckedSignal(bool isChecked);

private slots:
	void onSetCommonFrequencySlot();
	void onSetBandWidthSlot();

};

#endif // CONTROLPANEL_H
