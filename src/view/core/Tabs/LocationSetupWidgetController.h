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

class LocationSetupWidgetController : public QObject, public IController<LocationSetupWidget>,
									 public IRpcListener
{
	Q_OBJECT
public:
	explicit LocationSetupWidgetController(QObject *parent = 0);
	virtual ~LocationSetupWidgetController();

	void setLocationSetup(const RdsProtobuf::ClientMessage_OneShot_Location &data);
	void setPlatformList(const QStringList& platformList);
	void setDeviceEnableState(int dev, bool state);
	void setSpectrumSelection(double start, double end);
	void setDevicesState(RdsProtobuf::System_SystemOptions opt);

	RdsProtobuf::ClientMessage_OneShot_Location getCurrentLocation() const;
	bool getReceiveSpectrums() const;

private:
	LocationSetupWidget* m_view;

	RdsProtobuf::ClientMessage_OneShot_Location m_locationMessage;
	RdsProtobuf::ClientMessage_OneShot_Analysis m_analysisMessage;
	RdsProtobuf::ClientMessage_OneShot_Record m_recordMessage;

	QTimer m_locationTimer;
	int m_plotCounter;
	int m_incomePlotCounter;
	bool m_isStartLocation;

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

signals:
	void onMethodCalledSignal(QString, QVariant);

	void sendRdsData(QByteArray);
	void onSignalUpdate();

	void analysisChannelChanged(int);
	void signalPlotComplete();

	void signalSelectionUpdate();

private slots:
	void onMethodCalledSlot(QString method, QVariant data);

	void slotOnSetCommonFreq(int freq);
	void slotOnDeviceEnable(int, bool enable);

	void slotPlotDrawComplete();
	void slotPlotDrawCompleteInternal();
};

