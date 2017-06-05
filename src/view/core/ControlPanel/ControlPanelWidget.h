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
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QSound>

#include "RDSExchange.h"

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

	QString m_borderStyle;

	QTimer* m_alarmTimer;

	QSound* m_alarmSound;
	QTime m_soundTime;

protected:
	void paintEvent(QPaintEvent *);

public:
	ControlPanelWidget(QWidget* parent = NULL);
	~ControlPanelWidget();

	void changeCorrelationStatus(QString correlationValue);
	void changeCorrelationStatusActive(const bool isActive);

	void changeQualityStatus(const int status, bool isMoving = false, float freq = 0);
	void setStartFreq(int val);
	void setfinishFreq(int val);
	void setSystemMerge(bool state);
	void setMode(int val);
	void setPanorama(bool state);

	void setReceiveSpectrums(bool);
	void setCentralFreqValue(double freq);
	void setSleepMode(bool val);

	void setLocationSettings(RdsProtobuf::ClientMessage_OneShot_Location msg);



	bool getParoramaState(double& start, double& end);

	void updatePanorama(const bool enable, const double& start, const double& end);

	void setSolverConnectState(bool b);

	bool sleepMode() const;


	int getFreqSBValue();

	void applyManualMode();

	void alarmAimVisible(bool val);

	void showPanoramaControl(bool isOn);
signals:
	void commonFreqChangedSignal(double value);
	void onPanoramaEnable(bool, int, int);

	void autoSearchCheckedSignal(bool isChecked);

	void signalSetStartFreq(int);
	void signalSetEndFreq(int);
	void signalChangeMode(int);
	void signalPanorama(bool);
	void signalManualMode();
	void signalScanMode(int start, int finish);
	void signalCheckMode();
	void signalViewMode();
	void signalViewAreaMode();
	void signalViewAreaDopplerMode();

	void signalDown1Mhz();
	void signalDown10Mhz();
	void signalDown100Mhz();
	void signalUp1Mhz();
	void signalUp10Mhz();
	void signalUp100Mhz();

	void signalReceiveSpectrums(bool);
	void signalConvolution(bool);
	void signalDoppler(bool);
	void signalHumps(bool);

	void signalSystemMerge(bool);

	void onSleepMode(bool);

private slots:
	void onSetCommonFrequencySlot();
	void slotChangeMode(int index);

	void onSetPanorama(bool on);


	void showError(QString str);
	void showConfirm(QString);
	void showLocationError(QString str);

	void slotAlarmTimeout();

public slots:
	void slotChangeCommonFreq(double value);


};

#endif // CONTROLPANEL_H
