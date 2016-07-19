#pragma once

#include <QWidget>
#include <QDialog>
#include <QByteArray>
#include <QScrollBar>
#include <QString>
#include <QListWidget>

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

private:
	Ui::LocationSetupWidget *ui;

public:
	void appendSolverResult(const QString& log);

private slots:
	void onAddRangeDet();
	void onRemoveRangeDet();

	void onAddRangeCor();
	void onRemoveRangeCor();

signals:
	void onSendSignal();
	void onSignalUpdate();
	void onSignalSet();

	void onSignalUpdateDet();
	void onSignalSetDet();

	void onSignalUpdateCor();
	void onSignalSetCor();

};
