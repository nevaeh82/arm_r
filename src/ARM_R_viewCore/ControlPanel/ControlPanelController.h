#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>

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

#include <QMessageBox>

#include <QTimer>

#define INTERVAL 20
#define MINIMUM_FREQ 20
#define MAXIMUM_FREQ 7000

class ControlPanelController : public QObject, public IControlPanelController
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

	QList<StationsFrequencyAndBandwith> m_listOfFreqs;
	QList<StationsFrequencyAndBandwith>::Iterator m_itCheckMode;

	RpcFlakonClientWrapper* m_rpcFlakonClient;
	QMap<int, Station *> m_stationsMap;

	Station* m_mainStation;


public:
	explicit ControlPanelController(QObject *parent = 0);
	virtual ~ControlPanelController();

	void appendView(ControlPanelWidget* view);

	void setDbManager(IDbManager* dbManager);
	void setDbStationController(IDBStation* dbStationController);
	void setRpcFlakonClient(RpcFlakonClientWrapper* rpcFlakonClient);
	void setMapStations(QMap<int, Station *> stationsMap);


signals:
	void signalSetComonFreq(int value);

public slots:

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



};

#endif // CONTROLPANELCONTROLLER_H
