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

    void signalManualMode();
    void signalScanMode(int start, int finish);
    void signalCheckMode();
    void signalViewMode();

private slots:
	void onSetCommonFrequencySlot();
	void onSetBandWidthSlot();
    void slotChangeMode(int index);


};

#endif // CONTROLPANEL_H
