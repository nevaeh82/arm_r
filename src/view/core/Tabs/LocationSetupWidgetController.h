#pragma once

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>

#include "Interfaces/IController.h"

#include "LocationSetupWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

#include "RdsPacket.pb.h"
#include "SolverPacket1.pb.h"

class LocationSetupWidgetController : public QObject, public IController<LocationSetupWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit LocationSetupWidgetController(QObject *parent = 0);
	virtual ~LocationSetupWidgetController();

	void setLocationSetup(const RdsProtobuf::ClientMessage_OneShot_Location &data);
//	void setDetectorSetup(const RdsProtobuf::Detector& data);
//	void setCorrectionSetup(const RdsProtobuf::Correction &data);
//	void setAnalysisSetup(const RdsProtobuf::Analysis &data);
	void setAnalysisBandwidth(double center, double width);

	int getAnalysisWorkChannel() const;
	void setAnalysisChannelCount(int count);
	void setPlatformList(const QStringList& platformList);

	void setDeviceEnableState(int dev, bool state);
	void setSpectrumSelection(float bandwidth, float shift, double start, double end);

	void changeLocationFreqParams(float freq, float bandwidth, float shift);

	void setDevicesState(RdsProtobuf::System_SystemOptions opt);

	bool getReceiveSpectrums() const;

private:
	LocationSetupWidget* m_view;

	RdsProtobuf::ClientMessage_OneShot_Location m_locationMessage;
	RdsProtobuf::ClientMessage_OneShot_Analysis m_analysisMessage;
	RdsProtobuf::ClientMessage_OneShot_Record m_recordMessage;

	int m_workMode;

	bool m_receiveSpectrum;

public:
	void appendView(LocationSetupWidget* view);
	virtual void onMethodCalled(const QString& method, const QVariant& argument);
	LocationSetupWidget *getView();

	void requestLocation();

public slots:
	void slotShowWidget();
	void slotOnChangeWorkMode(int mode, bool);
	void slotSetReceiveSpectrums(bool receive);


signals:
	void onMethodCalledSignal(QString, QVariant);

	void sendRdsData(QByteArray);
	void onSignalUpdate();

	void analysisChannelChanged(int);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);
	void slotSendSettings();

	void slotOnUpdate();
	void slotOnSet();

	void slotOnUpdateDet();
	void slotOnSetDet();

	void slotOnUpdateCor();
	void slotOnSetCor();

	void slotOnUpdateAnalysis();
	void slotOnSetAnalysis();

	void slotOnSetCommonFreq(int freq);
	void slotOnDeviceEnable(int, bool enable);
};

