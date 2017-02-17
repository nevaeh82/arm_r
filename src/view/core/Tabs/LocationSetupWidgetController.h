#pragma once

#include <QObject>
#include <QVariant>

#include <Rpc/RpcDefines.h>
#include <QTimer>

#include "Interfaces/IController.h"

#include "LocationSetupWidget.h"
#include "Interfaces/IRpcListener.h"
#include "RadiolocationInterface.h"

#include "RdsPacket.pb.h"
#include "SolverPacket1.pb.h"

#include "CPPacket.pb.h"

class LocationSetupWidgetController : public QObject, public IController<LocationSetupWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit LocationSetupWidgetController(QObject *parent = 0);
	virtual ~LocationSetupWidgetController();

	void setPlatformList(const QStringList& platformList);
	void setDeviceEnableState(int dev, bool state);
	void setChannelEnableState(int dev, int channel, bool state);
	void setSpectrumSelection(double start, double end);
	void setDevicesState(RdsProtobuf::System_SystemOptions opt);

	RdsProtobuf::ClientMessage_OneShot_Location getCurrentLocation() const;

	bool getReceiveSpectrums() const;
	bool getReceiveDopler() const;

	void setSleepMode(bool);

private:
	LocationSetupWidget* m_view;

	RdsProtobuf::ClientMessage_OneShot_Location m_locationMessage;
	RdsProtobuf::ClientMessage_OneShot_Analysis m_analysisMessage;
	RdsProtobuf::ClientMessage_OneShot_Record m_recordMessage;

	QTimer m_locationTimer;
	int m_plotCounter;
	int m_incomePlotCounter;
	bool m_isStartLocation;

	int m_locationTimerInterval;

	bool m_requestReady;
	bool m_sleepMode;

public:
	void appendView(LocationSetupWidget* view);
	void onMethodCalled(const QString& method, const QVariant& argument);
	LocationSetupWidget *getView();

public slots:
	void slotShowWidget();

	void slotSetReceiveSpectrums(bool receive);

	void requestLocation();
	void requestAnalysis(int channel);
	void requestLocationTimer();

	void setLocationState(bool state);

	void requestRecord(int channel);

	void slotOnSetCommonFreq(double freq);
	void slotOnSetCommonFreq(double freq, double bandwidth);

	void updateLocation(RdsProtobuf::ClientMessage_OneShot_Location msg);

signals:
	void onMethodCalledSignal(QString, QVariant);

	void sendRdsData(QByteArray);
    void sendCPPacketData(QByteArray);
	void onSignalUpdate();

	void analysisChannelChanged(int);
	void signalPlotComplete();

	void signalSelectionUpdate();
	void signalSettingsChanged();

	void onStateChanged();

	void solverConnectionState(bool);


	void signalNIIPPWorkStatus(QString, bool);

private slots:
	void onMethodCalledSlot(QString method, QVariant data);

	void slotOnDeviceEnable(int, bool enable);

	void slotPlotDrawComplete();
	void slotPlotDrawCompleteInternal();

	void slotSetConvolution(bool);
	void slotSetDoppler(bool);
	void slotSetHumps(bool);
	void slotWigdetSettingsChanged();

    void slotChangeMode(int);
    void slotPanorama(bool);

    void slotSetStartFreq(int);
    void slotSetEndFreq(int);

    void slotSystemMerge(bool);


	void onGetSystem();
	void onRestartRds();
};

