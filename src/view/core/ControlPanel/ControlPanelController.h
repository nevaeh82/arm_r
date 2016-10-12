#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QTextCodec>
#include <QMessageBox>
#include <QTimer>

#include "Interfaces/IController.h"
#include "Interfaces/IDbManager.h"
#include "ControlPanelWidget.h"
#include "Interfaces/IControlPanelController.h"
#include "Interfaces/IControlPanelListener.h"

#include "DBStation/IDBStation.h"

#include "Tabs/Rpc/RpcFlakonClientWrapper.h"
#include "Station.h"

#include "ListWhiteDialog.h"
#include "ListWhiteDialogController.h"


#include <QSettings>
#include <QCoreApplication>

#define INTERVAL 20
#define MINIMUM_FREQ 20
#define MAXIMUM_FREQ 7000

#include "Interfaces/ICorrelationListener.h"

class ControlPanelController : public QObject, public IControlPanelController, public ICorrelationListener, public IRpcListener
{
	Q_OBJECT

private:
	ControlPanelWidget* m_view;

	IDbManager* m_dbManager;
	IDBStation* m_dbStation;

	QTimer m_timer;
	QTimer m_timerCheck;

	int m_startFreq;
	int m_finishFreq;
	int m_currentFreq;

	int m_timerInterval;
	int m_timerCheckInterval;
	int m_mode;

	QList<StationsFrequencyAndBandwith> m_listOfFreqs;
	QList<StationsFrequencyAndBandwith>::Iterator m_itCheckMode;

    QList<StationsFrequencyAndBandwith> m_listOfFreqsBlack;
    QList<StationsFrequencyAndBandwith>::Iterator m_itScanMode;

	RpcFlakonClientWrapper* m_rpcFlakonClient;
	QMap<int, Station *> m_stationsMap;

	Station* m_mainStation;

	void init();

    bool slotIncCheckMode();

    LocationSetupWidgetController* m_setupController;
public:
	explicit ControlPanelController(QObject *parent = 0);
	virtual ~ControlPanelController();

	void appendView(ControlPanelWidget* view);

	void setDbManager(IDbManager* dbManager);
	void setDbStationController(IDBStation* dbStationController);
	void setRpcFlakonClient(RpcFlakonClientWrapper* rpcFlakonClient);
	void setMapStations(QMap<int, Station *> stationsMap);
	void setResponseFreq(quint32 freq);

	//From correlation Listener
	void onCorrelationStateChanged(const bool isEnabled);

	void setCorrelationFrequencyValue(double value);

	virtual void onMethodCalled(const QString &method, const QVariant &argument);

	void onSetSystem(int modeId);
	void onSetSystemStatus(bool isOn);

	int getCurrentWorkMode() const;

    void setLocationSetupController(LocationSetupWidgetController* controller);

signals:
	void signalSetComonFreq(int value);
	void setCorrelationStatus(QString correlationStatus);
	void setCorrelationStatusActive(bool isActive);
	void signalSetMode(int mode);

	void onSignalWorkMode(int, bool);
    void onSignalWorkModeToGui(int, bool);


public slots:

    void changeFrequency(int value);
    void onEnableCurMode(bool enable);
    void onEnableMode(int mode, bool enable);

private slots:
	void onPanoramaStateChangedSlot(bool isEnabled);
	void onAutoSearchStateChangedSlot(bool isEnabled);
    void onCommonFrequencyChangedSlot(int value);

	void onBandWidthChangedSlot(int start, int end);

	void slotManualMode();
	void slotScanMode(int start, int finish);
	void slotCheckMode();
	void slotViewMode();

	void slotChangeFreq();
	void slotCheckModeSetFreq();

	void slotDown1MHz();
	void slotDown10MHz();
	void slotDown100MHz();
	void slotUp1MHz();
	void slotUp10MHz();
	void slotUp100MHz();

	void changeCorrelationStatus(QString correlationStatus);
	void changeCorrelationStatusActive(bool isActive);

	void slotSetMode(int mode);

	void onSlotWorkMode(int mode, bool isOn);
    void onSlotWorkModeGui(int mode, bool isOn);
};

#endif // CONTROLPANELCONTROLLER_H
