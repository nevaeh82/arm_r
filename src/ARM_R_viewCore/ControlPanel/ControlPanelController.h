#ifndef CONTROLPANELCONTROLLER_H
#define CONTROLPANELCONTROLLER_H

#include <QObject>

#include "Interfaces/IController.h"
#include "Interfaces/IDbManager.h"
#include "ControlPanelWidget.h"
#include "Interfaces/IControlPanelController.h"
#include "Interfaces/IControlPanelListener.h"

#include "DBStation/IDBStation.h"

#include "Tabs/Rpc/RpcFlakonClient.h"
#include "Station.h"

#include "ListWhiteDialog.h"
#include "ListWhiteDialogController.h"

#include <QMessageBox>

#include <QTimer>

#define INTERVAL 20

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

	RpcFlakonClient* m_rpcFlakonClient;
	QMap<int, Station *> m_stationsMap;

	Station* m_mainStation;


public:
	explicit ControlPanelController(QObject *parent = 0);
	virtual ~ControlPanelController();

	void appendView(ControlPanelWidget* view);

	void setDbManager(IDbManager* dbManager);
	void setDbStationController(IDBStation* dbStationController);
	void setRpcFlakonClient(RpcFlakonClient* rpcFlakonClient);
	void setMapStations(QMap<int, Station *> stationsMap);


signals:

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

};

#endif // CONTROLPANELCONTROLLER_H
