#pragma once

#include <QWidget>
#include <QDialog>
#include <QByteArray>
#include <QScrollBar>
#include <QString>
#include <QListWidget>
#include <QCheckBox>
#include <QLabel>

#include <QSignalMapper>

#include "RdsPacket.pb.h"

namespace Ui {
class LocationSetupWidget;
}

class LocationSetupWidget : public QDialog
{
	Q_OBJECT

public:
	explicit LocationSetupWidget(QWidget *parent = 0);
	~LocationSetupWidget();

	void setLocationData(const RdsProtobuf::Location& data);
	RdsProtobuf::Location getLocationData() const;

	void setDetectorData(const RdsProtobuf::Detector &data);
	RdsProtobuf::Detector getDetectorData() const;

	void setCorrectionData(const RdsProtobuf::Correction &data);
	RdsProtobuf::Correction getCorrectionData() const;

	void setAnalysisData(const RdsProtobuf::Analysis &data);
	RdsProtobuf::Analysis getAnalysisData() const;

	int getAnalysisChannel() const;
	void setAnalysisChannelCount(int cnt);

	void setPlatformList(const QStringList &list);

	void setDeviceEnableState(int dev, bool state);
	void setWorkMode(int mode);

	void onSpectrumLocationSelection(float bandwidth, float shift);
	void onSpectrumAnalysisSelection(double start, double end);

private:
	Ui::LocationSetupWidget *ui;

	QSignalMapper m_devSignalMap;
	QMap<quint32, QCheckBox*> m_cbDevMap;
	QMap<quint32, QLabel*> m_cbDevMapTitle;

	void addDeviceEnableControl(QString platformTtile, int device);

public:
	void appendSolverResult(const QString& log);

private slots:
	void onAddRangeDet();
	void onRemoveRangeDet();

	void onAddRangeCor();
	void onRemoveRangeCor();

	void onDeviceEnable(int);

signals:
	void onSendSignal();
	void onSignalUpdate();
	void onSignalSet();

	void onSignalUpdateDet();
	void onSignalSetDet();

	void onSignalUpdateCor();
	void onSignalSetCor();

	void onSignalUpdateAnalysis();
	void onSignalSetAnalysis();

	void analysisChannelChanged(int);

	void onSignalDeviceEnable(int, bool);
};
