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
	explicit LocationSetupWidget(int id, QWidget *parent = 0);
	~LocationSetupWidget();

	void setLocationData(const RdsProtobuf::ClientMessage_OneShot_Location& data);
	RdsProtobuf::ClientMessage_OneShot_Location getLocationData() const;

	int getAnalysisDuration() const;
	int getRecordDuration() const;

	void setPlatformList(const QStringList &list);

	void setDeviceEnableState(int dev, bool state);
	void setChannelEnableState(int dev, int channel, bool state);

	void setDeviceCommonState(const RdsProtobuf::System_SystemOptions& opt);

	void setTitle(const QString& title);

private:
	Ui::LocationSetupWidget *ui;

	QSignalMapper m_devSignalMap;
	QMap<quint32, QCheckBox*> m_cbDevMap;
	QMap<quint32, QLabel*> m_cbDevMapTitle;

	int m_id;

	void addDeviceEnableControl(QString platformTtile, int device);

public:
	void appendSolverResult(const QString& log);

private slots:
	void onDeviceEnable(int);

	void showError(QString str);
	void showConfirm(QString str);
	void showLocationError(QString str);

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
	void sendRdsData(QByteArray);

	void signalSettingsChanged();

	void signalRestartRds();
	void signalGetSystem();
};
