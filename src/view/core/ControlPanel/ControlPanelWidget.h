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

	QPixmap* m_pmRoundRed;
	QPixmap* m_pmRoundGreen;

public:
	ControlPanelWidget(QWidget* parent = NULL);
	~ControlPanelWidget();

	void changeCorrelationStatus(QString correlationValue);
	void changeCorrelationStatusActive(const bool isActive);

	void changeQualityStatus(const int status);

	void onSetMode(int modeId);
	void onSetModeStatus(bool isOn);

	int onGetMode() const;

signals:
	void commonFreqChangedSignal(int value);
	void bandwidthChangedSignal(int start, int end);

	void panoramaCheckedSignal(bool isChecked);
	void autoSearchCheckedSignal(bool isChecked);

    void signalManualMode();
    void signalScanMode(int start, int finish);
    void signalCheckMode();
    void signalViewMode();

	void signalDown1Mhz();
	void signalDown10Mhz();
	void signalDown100Mhz();
	void signalUp1Mhz();
	void signalUp10Mhz();
	void signalUp100Mhz();
	void signalSetMode(int);

	void signalWorkMode(int, bool);

private slots:
	void onSetCommonFrequencySlot();
	void onSetBandWidthSlot();
	void slotChangeMode(int index);
	void slotOnWorkMode();
    void slotTurnWorkMode(bool val);

public slots:
	void slotChangeCommonFreq(int value);


};

#endif // CONTROLPANEL_H
